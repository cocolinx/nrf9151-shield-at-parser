//
//    FILE: MQTT_Example.ino
//  AUTHOR: CocoLinx
// PURPOSE: Test MQTT communication by publishing 30 messages and then stopping
//     URL: https://github.com/cocolinx/nrf9151-shield-at-parser

#include <CShield_v2.h>

#define URC_CEREG CShield_v2::ACK_URC_CEREG
#define URC_MQTT_EVT  CShield_v2::ACK_URC_MQTT_EVT
#define URC_MQTT_MSG  CShield_v2::ACK_URC_MQTT_MSG

CShield_v2 test;
CShield_v2::UrcPacket _urcPacket;

char mqtt_topic[128];
char mqtt_msg[512];

void setup() {
	int32_t ret;

	Serial.begin(115200);
	while (!Serial) ; //
	Serial.println();
	Serial.println("====== setup() start ======");

  Serial.print("cocolinx begin...");
  ret = test.begin();
  if(!ret)
  {
    Serial.println("error");
		Serial.println("halt forever...");
		while (1);
  } 
  else Serial.println("okay");

  ///////////////////// lte connection
  Serial.print("set cfun(1)...");
  ret = test.set_cfun(1);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("okay");
  }

  Serial.print("set cereg(1)...");
  ret = test.set_cereg(1);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("okay");
  }

  uint32_t start = millis();
  bool connected = false;
  while(millis() - start < 10000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    if(ret & URC_CEREG)
    {
      Serial.print("cereg: ");
      Serial.println(_urcPacket.cereg);
      if(_urcPacket.cereg == 5)
      {
        connected = true;
        break;
      }
    }
    delay(500);
  }
  
  if(!connected)
  {
    Serial.println("lte connection failed");
    Serial.println("halt forever");
    while (1);
  }
  else Serial.println("lte connection success");

  _urcPacket.mqtt_topic = mqtt_topic;
  _urcPacket.mqtt_msg = mqtt_msg;
  _urcPacket.mqtt_topic_size = sizeof(mqtt_topic);
  _urcPacket.mqtt_msg_size = sizeof(mqtt_msg);

  ///////////////////// mqtt config
  Serial.print("set mqtt cfg...");
  ret = test.cx_mqtt_cfg("cocolinx", 30, 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("okay");
  }

  ///////////////////// mqtt connect
  Serial.print("mqtt connect...");
  ret = test.cx_mqtt_con(1, NULL, NULL, "broker.emqx.io", 1883);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("okay");
  }

  start = millis();
  bool done = false;
  while(millis() - start < 5000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt evt result: ");
      Serial.println(_urcPacket.mqtt_result);
      if(_urcPacket.mqtt_evt_type == 0 && _urcPacket.mqtt_result == 0) 
      {
        done = true;
        break;
      }
    }
    delay(500);
  }

  if(!done)
  {
    Serial.println("mqtt connection failed");
    Serial.println("halt forever");
    while (1);
  }

  ///////////////////// mqtt subscribe
  Serial.print("mqtt topic \"cocolinx/test\" subscribe...");
  ret = test.cx_mqtt_sub("cocolinx/test", 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("okay");
  }

  start = millis();
  done = false;
  while(millis() - start < 5000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt evt result: ");
      Serial.println(_urcPacket.mqtt_result);
      if(_urcPacket.mqtt_evt_type == 7 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  if(!done) Serial.println("mqtt subscribe failed");
}

void loop() {
  static uint32_t pubCount = 0;
  int32_t ret;
  ///////////////////// mqtt publish
  Serial.print("mqtt topic \"cocolinx/test\" publish...");
  char mqttTx[] = "hello mqtt cocolinx~~";
  ret = test.cx_mqtt_pub("cocolinx/test", mqttTx, 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret); 
  }
  else
  {
    Serial.println("okay");
    pubCount++;
  }
  
  uint32_t start = millis();
  while(millis() - start < 5000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt evt result: ");
      Serial.println(_urcPacket.mqtt_result);
    }
    if(ret & URC_MQTT_MSG)
    {
      Serial.print("mqtt topic: ");
      Serial.println(_urcPacket.mqtt_topic);
      Serial.print("mqtt msg recv: ");
      Serial.println(_urcPacket.mqtt_msg);
      break;
    }
    delay(500);
  }

  if(pubCount >= 30)
  {
    ///////////////////// mqtt unsubscribe
    Serial.print("mqtt topic \"cocolinx/test\" unsubscribe...");
    ret = test.cx_mqtt_unsub("cocolinx/test");
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    else
    {
      Serial.println("okay");
    }
    delay(1000);

    ///////////////////// mqtt disconnect
    Serial.print("mqtt disconnect...");
    ret = test.cx_mqtt_disconnect();
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
    }
    else
    {
      Serial.println("okay");
    }

    _urcPacket.mqtt_topic = NULL;
    _urcPacket.mqtt_msg = NULL;

    Serial.println("MQTT test finished");
    while(1);
  }

  delay(10000);
}
