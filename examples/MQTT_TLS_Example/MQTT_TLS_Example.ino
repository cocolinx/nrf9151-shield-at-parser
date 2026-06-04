//
//    FILE: MQTT_TLS_Example.ino
//  AUTHOR: CocoLinx
// PURPOSE: test MQTT TLS communication
//     URL: https://github.com/cocolinx/Arduino-nRF9151_Shield_Library

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
    return false;
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
    return false;
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
      return false;
    }
    if(ret & URC_CEREG)
    {
      Serial.print("cereg: ");
      Serial.println(_urcPacket.cereg);
      if(_urcPacket.cereg == 1)
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
  Serial.print("set mqtt_tls cfg...");
  ret = test.cx_mqtt_cfg("cocolinx", 30, 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else
  {
    Serial.println("okay");
  }

  ///////////////////// mqtt connect
  Serial.print("mqtt_tls connect...");
  ret = test.cx_mqtt_con_secure(1, NULL, NULL, "broker.emqx.io", 8883, 5555);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else
  {
    Serial.println("okay");
  }

  uint32_t start = millis();
  bool done = false;
  while(millis() - start < 5000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt_tls evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt_tls evt result: ");
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
    Serial.println("mqtt_tls connection failed");
    Serial.println("halt forever");
    while (1);
  }

  ///////////////////// mqtt subscribe
  Serial.print("mqtt_tls topic \"cocolinx/test/tls\" subscribe...");
  ret = test.cx_mqtt_sub("cocolinx/test/tls", 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
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
      return false;
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt_tls evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt_tls evt result: ");
      Serial.println(_urcPacket.mqtt_result);
      if(_urcPacket.mqtt_evt_type == 7 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  if(!done) Serial.println("mqtt_tls subscribe failed");
}

void loop() {
  static uint32_t pubCount = 0;

  ///////////////////// mqtt publish
  Serial.print("mqtt_tls topic \"cocolinx/test/tls\" publish...");
  char mqttTx[] = "hello mqtt_tls cocolinx~~";
  ret = test.cx_mqtt_pub("cocolinx/test/tls", mqttTx, 0);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret); 
    return false;
  }
  else
  {
    Serial.println("okay");
    pubCount++;
  }
  
  start = millis();
  while(millis() - start < 5000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    if(ret & URC_MQTT_EVT)
    {
      Serial.print("mqtt_tls evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt_tls evt result: ");
      Serial.println(_urcPacket.mqtt_result);
    }
    if(ret & URC_MQTT_MSG)
    {
      Serial.print("mqtt_tls topic: ");
      Serial.println(_urcPacket.mqtt_topic);
      Serial.print("mqtt_tls msg recv: ");
      Serial.println(_urcPacket.mqtt_msg);
      break;
    }
    delay(500);
  }

  if(pubCount >= 10)
  {
    ///////////////////// mqtt unsubscribe
    Serial.print("mqtt_tls topic \"cocolinx/test/tls\" unsubscribe...");
    ret = test.cx_mqtt_unsub("cocolinx/test/tls");
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    else
    {
      Serial.println("okay");
    }
    delay(1000);

    ///////////////////// mqtt disconnect
    Serial.print("mqtt_tls disconnect...");
    ret = test.cx_mqtt_disconnect();
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    else
    {
      Serial.println("okay");
    }

    _urcPacket.mqtt_topic = NULL;
    _urcPacket.mqtt_msg = NULL;

    Serial.println("MQTT_TLS test finished");
    while(1);
  }
}
