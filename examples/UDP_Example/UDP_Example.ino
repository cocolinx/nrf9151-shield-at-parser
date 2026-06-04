//
//    FILE: UDP_Example.ino
//  AUTHOR: CocoLinx
// PURPOSE: test UDP socket communication
//     URL: https://github.com/cocolinx/Arduino-nRF9151_Shield_Library

#include <CShield_v2.h>

#define URC_CEREG CShield_v2::ACK_URC_CEREG

CShield_v2 test;
CShield_v2::UrcPacket _urcPacket;

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
}

void loop() {
  int32_t ret, handle;
  Serial.print("open udp socket...");
  ret = test.cx_socket(1, 2, 0, &handle);
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

  Serial.print("connect to cocolinx udp echo server...");
  ret = test.cx_connect(handle, "echo.cocolinx.com", 7777, NULL);
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

  Serial.print("udp send...");
  char udpTx[] = "hello udp cocolinx~~";
  ret = test.cx_send(handle, 0, udpTx, strlen(udpTx));
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

  Serial.print("udp recv data(max 15 secs)...");
  char udpRx[32];
  int32_t rxcnt;
  rxcnt = test.cx_recv(handle, 0, 15, udpRx, sizeof(udpRx) - 1);
  if(rxcnt < 0)
  {
    Serial.print("error: ");
    Serial.println(rxcnt);
    return false;
  }
  else
  {
    Serial.println("okay");
    Serial.print("recv> ");
    udpRx[rxcnt] = '\0';
    Serial.println(udpRx);
  }

  Serial.print("udp close...");
  ret = test.cx_close(handle);
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
