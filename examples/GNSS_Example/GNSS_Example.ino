//
//    FILE: GNSS_Example.ino
//  AUTHOR: CocoLinx
// PURPOSE: test GNSS
//     URL: https://github.com/cocolinx/Arduino-nRF9151_Shield_Library

#include <CShield_v2.h>

#define URC_GNSS_EVT  CShield_v2::ACK_URC_GNSS_EVT
#define URC_GNSS_DATA CShield_v2::ACK_URC_GNSS_DATA

CShield_v2 test;
CShield_v2::UrcPacket _urcPacket;
CShield_v2::GnssData  _gnssData;

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

  Serial.print("set cfun(31)...");
  ret = test.set_cfun(31);
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

  delay(500);

  _urcPacket.gnss_data = _gnssData;

  Serial.print("gnss start(60 secs)...");
  ret = test.cx_gnss_control(0, 1, 0);
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
}

void loop() {
  static uint32_t start = millis();
  bool done = false;

  if(millis() - start < 60000)
  {
    ret = test.read_urc_pkt(&_urcPacket);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    if(ret & URC_GNSS_EVT)
    {
      Serial.print("gnss_service: ");
      Serial.println(_urcPacket.gnss_data.gnss_service);
      Serial.print("gnss_status: ");
      Serial.println(_urcPacket.gnss_data.gnss_status);
    }
    if(ret & URC_GNSS_DATA)
    {
      Serial.print("latitude: ");
      Serial.println(_urcPacket.gnss_data.latitude);
      Serial.print("longitude: ");
      Serial.println(_urcPacket.gnss_data.longitude);
      done = true;
      break;
    }
  }
  else
  {
    Serial.println("timeout");
    Serial.print("gnss stop...");
    ret = test.cx_gnss_stop();
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    else
    {
      Serial.println("okay");
      while(1);
    }
  }

  if(done) 
  {
    Serial.println("GNSS success");
    Serial.print("gnss stop...");
    ret = test.cx_gnss_stop();
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    else
    {
      Serial.println("okay");
      while(1);
    }
  }
  delay(1000);
}
