//
//    FILE: FullTest_Example.ino
//  AUTHOR: CocoLinx
// PURPOSE: test all functions in library
//     URL: https://github.com/cocolinx/nrf9151-shield-at-parser

#include <CShield_v2.h>

#define TEST_LTE_PLMN_SELECT 	CShield_v2::PLMN_SKT

#define URC_NONE      CShield_v2::ACK_URC_NONE
#define URC_MQTT_EVT  CShield_v2::ACK_URC_MQTT_EVT
#define URC_MQTT_MSG  CShield_v2::ACK_URC_MQTT_MSG
#define URC_GNSS_EVT  CShield_v2::ACK_URC_GNSS_EVT
#define URC_GNSS_DATA CShield_v2::ACK_URC_GNSS_DATA

#define TEST_INTERVAL_SECONDS 180

CShield_v2 test;
CShield_v2::UrcPacket _urcPacket;
CShield_v2::GnssData  _gnssData;

bool sample_modem_info()
{
  int32_t ret;

  Serial.println("*** sample_modem_info() ***");	

  Serial.print("read manufacturer...");
  char manufacturer[32];
  ret = test.get_cgmi(manufacturer, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("manufacturer: ");
    Serial.println(manufacturer);
  }

  Serial.print("read model...");
  char model[32];
  ret = test.get_cgmm(model, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("model: ");
    Serial.println(model);
  }
    
  Serial.print("read revision...");
  char revision[32];
  ret = test.get_cgmr(revision, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("revision: ");
    Serial.println(revision);
  }

  Serial.print("read imei...");
  char imei[32];
  ret = test.get_imei(imei, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("imei: ");
    Serial.println(imei);
  }

  Serial.print("read uuid...");
  char uuid[64];
  ret = test.get_uuid(uuid, 64);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("uuid: ");
    Serial.println(uuid);
  }

  Serial.println("sample_modem_info done!");
  return true;
}

bool sample_lte_connection()
{
  int32_t ret;

  Serial.println("*** sample_lte_connection() ***");	

  Serial.print("set cops...");
  ret = test.set_cops(1, 2, TEST_LTE_PLMN_SELECT);
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

  Serial.print("set cfun...");
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

  delay(1000);

  Serial.print("set cereg...");
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

  delay(1000);

  Serial.print("read cfun...");
  int32_t fun;
  ret = test.get_cfun(&fun);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("cfun: ");
    Serial.println(fun);
  }

  delay(1000);
  
  uint32_t start = millis();
  bool connected = false;
  while(millis() - start < 10000)
  {
    Serial.print("read cereg...");
    int32_t stat;
    ret = test.get_cereg(&stat);
    if(ret < 0)
    {
      Serial.print("error: ");
      Serial.println(ret);
      return false;
    }
    else 
    {
      Serial.println("okay");
      Serial.print("get_cereg: ");
      Serial.println(stat);
      if(stat == 5) break;
    }
    delay(1000);  
  }


  Serial.print("read oper...");
  int32_t oper;
  ret = test.get_cops(&oper);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("oper: ");
    Serial.println(oper);
  }


  Serial.print("read rsrp, rsrq...");
  int32_t rsrp, rsrq;
  ret = test.get_cesq(&rsrq, &rsrp);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("rsrp_dbm: ");
    Serial.println(rsrp);
    Serial.print("rsrq_dbm: ");
    Serial.println(rsrq);
  }

  Serial.print("read band...");
  int32_t band;
  ret = test.get_band(&band);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("band: ");
    Serial.println(band);
  }

  Serial.print("read imsi...");
  char imsi[32];
  ret = test.get_imsi(imsi, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("imsi: ");
    Serial.println(imsi);
  }

  Serial.print("read iccid...");
  char iccid[32];
  ret = test.get_iccid(iccid, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else 
  {
    Serial.println("okay");
    Serial.print("iccid: ");
    Serial.println(iccid);
  }

  Serial.println("sample_lte_connection done!");
  return true;
}

bool sample_date_time()
{
  int32_t ret;

  Serial.println("*** sample_date_time() ***");	

  Serial.print("read time...");
  char time[32];
  ret = test.get_cclk(time, 32);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else
  {
    int yy, MM, dd, hh, mm, ss, offset;
    Serial.println("okay");
    sscanf(time, "%d/%d/%d,%d:%d:%d+%d", &yy, &MM, &dd, &hh, &mm, &ss, &offset);

    Serial.print("year: ");
    Serial.println(2000 + yy);
    Serial.print("month: ");
    Serial.println(MM);
    Serial.print("date: ");
    Serial.println(dd);
    Serial.print("time: ");
    Serial.print(hh + (offset * 15) / 60);
    Serial.print(":");
    Serial.print(mm);
    Serial.print(":");
    Serial.println(ss);
  }

  Serial.println("sample_date_time done!");
  return true;
}

bool sample_udp()
{
  int32_t ret;

  Serial.println("*** sample_udp() ***");	

  int32_t handle;
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

  Serial.println("sample_udp done!");
  return true;
}

bool sample_tcp()
{
  int32_t ret;

  Serial.println("*** sample_tcp() ***");	

  int32_t handle;
  Serial.print("open tcp socket...");
  ret = test.cx_socket(1, 1, 0, &handle);
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

  Serial.print("connect to cocolinx tcp echo server...");
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

  Serial.print("tcp send...");
  char tcpTx[] = "hello tcp cocolinx~~";
  ret = test.cx_send(handle, 0, tcpTx, strlen(tcpTx));
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

  Serial.print("tcp recv data(max 15 secs)...");
  char tcpRx[32];
  int32_t rxcnt;
  rxcnt = test.cx_recv(handle, 0, 15, tcpRx, sizeof(tcpRx) - 1);
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
    tcpRx[rxcnt] = '\0';
    Serial.println(tcpRx);
  }

  Serial.print("tcp close...");
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

  Serial.println("sample_tcp done!");
  return true;

}

bool sample_mqtt()
{
  int32_t ret;
  char mqtt_topic[128];
  char mqtt_msg[512];

  Serial.println("*** sample_mqtt() ***");	

  _urcPacket.mqtt_topic = mqtt_topic;
  _urcPacket.mqtt_msg = mqtt_msg;
  _urcPacket.mqtt_topic_size = sizeof(mqtt_topic);
  _urcPacket.mqtt_msg_size = sizeof(mqtt_msg);

  Serial.print("set mqtt cfg...");
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

  Serial.print("mqtt connect...");
  ret = test.cx_mqtt_con(1, NULL, NULL, "broker.emqx.io", 1883);
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

  if(!done) return false;

  Serial.print("mqtt client info read...");
  int32_t status, port;
  char url[32];
  ret = test.cx_get_mqtt_con(&status, url, sizeof(url), &port, NULL);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else
  {
    Serial.println("okay");
    Serial.print("status: ");
    Serial.println(status);
    if(status != 0)
    {
      Serial.print("url: ");
      Serial.println(url);
      Serial.print("port: ");
      Serial.println(port);
    }
  }

  Serial.print("mqtt topic \"cocolinx/test\" subscribe...");
  ret = test.cx_mqtt_sub("cocolinx/test", 0);
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

  if(!done) return false;

  Serial.print("mqtt topic \"cocolinx/test\" publish...");
  char mqttTx[] = "hello mqtt cocolinx~~";
  ret = test.cx_mqtt_pub("cocolinx/test", mqttTx, 0);
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
      done = true;
      break;
    }
    delay(500);
  }

  if(!done) return false;

  Serial.print("mqtt topic \"cocolinx/test\" unsubscribe...");
  ret = test.cx_mqtt_unsub("cocolinx/test");
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
      Serial.print("mqtt evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt evt result: ");
      Serial.println(_urcPacket.mqtt_result);
      if(_urcPacket.mqtt_evt_type == 8 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  if(!done) return false;

  Serial.print("mqtt disconnect...");
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
      Serial.print("mqtt evt type: ");
      Serial.println(_urcPacket.mqtt_evt_type);
      Serial.print("mqtt evt result: ");
      Serial.println(_urcPacket.mqtt_result);
      if(_urcPacket.mqtt_evt_type == 1 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  if(!done) return false;

  _urcPacket.mqtt_topic = NULL;
  _urcPacket.mqtt_msg = NULL;
  
  Serial.println("sample_mqtt done!");
  return true;
}

bool sample_mqtt_tls()
{
  int32_t ret;

  Serial.println("*** sample_mqtt_tls() ***");	

  char mqtt_topic[128];
  char mqtt_msg[512];

  _urcPacket.mqtt_topic = mqtt_topic;
  _urcPacket.mqtt_msg = mqtt_msg;
  _urcPacket.mqtt_topic_size = sizeof(mqtt_topic);
  _urcPacket.mqtt_msg_size = sizeof(mqtt_msg);

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

  if(!done) return false;

  Serial.print("mqtt_tls client info read...");
  int32_t status, port, sec_tag;
  char url[32];
  ret = test.cx_get_mqtt_con(&status, url, sizeof(url), &port, &sec_tag);
  if(ret < 0)
  {
    Serial.print("error: ");
    Serial.println(ret);
    return false;
  }
  else
  {
    Serial.println("okay");
    Serial.print("status: ");
    Serial.println(status);
    if(status != 0)
    {
      Serial.print("url: ");
      Serial.println(url);
      Serial.print("port: ");
      Serial.println(port);
      Serial.print("sec_tag: ");
      Serial.println(sec_tag);
    }
  }

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

  if(!done) return false;

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
    }
    if(ret & URC_MQTT_MSG)
    {
      Serial.print("mqtt_tls topic: ");
      Serial.println(_urcPacket.mqtt_topic);
      Serial.print("mqtt_tls msg recv: ");
      Serial.println(_urcPacket.mqtt_msg);
      done = true;
      break;
    }
    delay(500);
  }

  if(!done) return false;

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
      if(_urcPacket.mqtt_evt_type == 8 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  if(!done) return false;

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
      if(_urcPacket.mqtt_evt_type == 1 && _urcPacket.mqtt_result == 0)
      {
        done = true;
        break;
      } 
    }
    delay(500);
  }

  _urcPacket.mqtt_topic = NULL;
  _urcPacket.mqtt_msg = NULL;

  Serial.println("sample_mqtt_tls done!");
  return true;
}

bool sample_rs485()
{
  int32_t ret;

  Serial.println("*** sample_rs485() ***");	

  Serial.print("rs485 enable...");
  ret = test.cx_rs485_enable(115200);
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

  Serial.print("rs485 tx...");
  char rs485Tx[] = "hello rs485 cocolinx~~";
  ret = test.cx_rs485_tx(rs485Tx, strlen(rs485Tx));
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

  Serial.print("rs485 rx...");
  char rs485Rx[32];
  int32_t rxcnt;
  rxcnt = test.cx_rs485_rx(rs485Rx, sizeof(rs485Rx));
  if(rxcnt < 0)
  {
    Serial.print("error: ");
    Serial.println(rxcnt);
    return false;
  }
  else
  {
    Serial.println("okay");
    rs485Rx[rxcnt] = '\0';
    Serial.print("recv data: ");
    Serial.println(rs485Rx);
  }

  Serial.println("sample_rs485 done!");
  return true;
}

bool sample_gnss()
{
  int32_t ret;

  Serial.println("*** sample_gnss() ***");	

  _urcPacket.gnss_data = _gnssData;

  Serial.print("set cfun 31...");
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

  uint32_t start = millis();
  bool done = false;
  while(millis() - start < 60000)
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
      break;
    }
    delay(1000);
  }

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
  }

  Serial.println("sample_gnss done!");
  return true;
}


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
}

void loop() {
	static uint32_t millisPrev = -(1000 * 60 * 5); // start first test on first loop
	static uint32_t testCount = 0;

	int32_t ret;
	uint32_t testIntervalMs = (1000 * TEST_INTERVAL_SECONDS);

  if(testIntervalMs < 30000) testIntervalMs = 30000;

  if((millis() - millisPrev) >= testIntervalMs)
  {
    testCount++;

		Serial.println();
		Serial.print("====== test start [");
		Serial.print(testCount);
		Serial.println("] ======");

    sample_modem_info();
    sample_lte_connection();
    sample_date_time();
    sample_udp();
    sample_tcp();
    sample_mqtt();
    sample_mqtt_tls();
    sample_rs485();
    sample_gnss();

    Serial.println("===== test done =====");
    millisPrev = millis();
  }
}
