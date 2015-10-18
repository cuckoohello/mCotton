// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   2  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIV2); // you can change this clock speed but DI

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void) {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
    Serial.println(F("\r\nUnable to retrieve the IP Address!"));
    return false;
  } 
  else {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

bool voCC3000_get_dhcp() {
  bool cache;

  for(int a=0;a<15;a++) {
    Serial.println(F("Request DHCP"));
    if(!cc3000.checkDHCP()) {
      cache=false;
      delay(1000); // ToDo: Insert a DHCP timeout!
    } 
    else {
      cache=true;
      break;
    }
  }

  if(!cache)
    return false;
  else {
    for(int a=0;a<2;a++) {
      /* Display the IP address DNS, Gateway, etc. */
      if (! displayConnectionDetails()) {
        cache=false;
        delay(500);
      }
      else {
        cache=true;
        break;
      }
    }
    return cache ? true :false;
  }
}


bool voCC3000_get_server_ip() {
  ip = 0;
  // Try looking up the website's IP address
  Serial.print(WEBSITE); 
  Serial.print(F(" -> "));
  for(int a=0;a<2;a++) {
    if (! ip) {
      Serial.println(F("\r\n While ing "));
      if (! cc3000.getHostByName(WEBSITE, &ip)) 
        Serial.println(F("Couldn't resolve!"));
      delay(500);
    }
    else
      break;
  }
  Serial.println(cc3000.getHostByName(WEBSITE, &ip));
  cc3000.printIPdotsRev(ip);
  Serial.println(F("  "));
  return ip ? true :false;
}

bool voCC3000_reconnect() {
  cc3000.reboot();
  //----------------------------------------------------------
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //----------------------------------------------------------
  if(!voCC3000_get_dhcp())
    return false;
  else {
    if(!voCC3000_get_server_ip())
      return false;
    else 
      return true;
  } 
}

bool voCC3000_ping(int a) {
  Serial.println("**************************post to Service******************************************");

  www = cc3000.connectTCP(ip, WEBSITEPORT);
  Serial.println(F("Connecting"));
  if (www.connected()) {
    Serial.println(F("Connected"));

    www.print("POST ");
    www.print(WEBPAGE);
    www.println(" HTTP/1.1");
    www.print(F("Host: ")); 
    www.println(WEBSITE);
    www.print("Accept: *");
    www.print("/");
    www.println("*");


    dataToSend="{\"device_id\":";
    dataToSend+="\""+device_id+"\"";
    dataToSend+=",\"Temperature\":";
    dtostrf(sensor_tem,1,2,buf); 
    dataToSend+="\""+String(buf)+"\"";
    dataToSend+=",\"Humidity\":";
    dtostrf(sensor_hum,1,2,buf); 
    dataToSend+="\""+String(buf)+"\"";
    dataToSend+=",\"Light\":";
    dtostrf(sensor_lux,1,2,buf); 
    dataToSend+="\""+String(buf)+"\"";
    dataToSend+=",\"Air Pressure\":";
    dtostrf(sensor_pre,1,2,buf); 
    dataToSend+="\""+String(buf)+"\"";
    dataToSend+="}";

    Serial.println(dataToSend);

    www.println("Content-Type: application/json");
    www.print("Content-Length: ");
    www.println(dataToSend.length());

    www.println();

    www.print(dataToSend);

    Serial.println(F("Sent"));

    return true;
  }
  else  
    return false;
}


bool voCC3000_buttonPost(int a) {
  www = cc3000.connectTCP(ip, WEBSITEPORT);
  Serial.println(F("Connecting"));
  if (www.connected()) {
    Serial.println(F("Connected"));

    www.print("POST ");
    www.print(WEBUTTONPAGE);
    www.println(" HTTP/1.1");
    www.print(F("Host: ")); 
    www.println(WEBSITE);
    www.print("Accept: *");
    www.print("/");
    www.println("*");

    dataToSend="{\"device_id\":";
    dataToSend+="\""+device_id+"\"";
    dataToSend+=",\"control_name\":";
    dataToSend+="\"Status\"";
    dataToSend+=",\"control_value\":";
    dataToSend+="\""+stateValue+"\"";
    dataToSend+="}";

    Serial.println(dataToSend);

    www.println("Content-Type: application/json");
    www.print("Content-Length: ");
    www.println(dataToSend.length());

    www.println();

    www.print(dataToSend);

    Serial.println(F("Button State Sent"));

    return true;
  }
  else  
    return false;
}




void voCC3000_rec() {
  Serial.println(F("---------Data start-------------"));

  //returnStateData="";
  unsigned long lastRead = millis();
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read(); 
      returnStateData+=c;    
      lastRead = millis();
    }
  }
  www.close();

  statusChange=false;
  Serial.print("all:");
  Serial.println(returnStateData);

  start=returnStateData.indexOf('{');
  returnStateData=returnStateData.substring(start,returnStateData.length());

  // returnStateData.toCharArray(json, returnStateData.length());
  // JsonObject& root = jsonBuffer.parseObject(json);


  start=returnStateData.indexOf("\"Status\":");

  if(start>0) {

    // if (!root.success()) {
    //   Serial.println("parseObject() failed");
    //   //return;
    // }
    // statusChar = root["Status"];

    end=returnStateData.indexOf("\"}");

    returnStateData=returnStateData.substring(start+10,end);

    Serial.println(returnStateData);
    //Serial.println(statusChar);

    //stateValue=statusChar;
    stateValue=returnStateData;

    if(stateValue=="true") {
      Serial.println("ONONON");
      s_data='1';
      digitalWrite(ledPin, HIGH);

    } 
    else {
      Serial.println("OFFOFFOFF");
      s_data='0';
      digitalWrite(ledPin, LOW);
    }

  }

  returnStateData="";

  // for(int i=0; i<200; i++) {
  //   json[i]=NULL;
  // }

  Serial.println(F("---------Data end-------------"));
}



void setup_wifi() {
  Serial.println(F("Hello, CC3000!\n")); 

  Serial.print("Free RAM: "); 
  Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin()) {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  } 
  else
    osd_setup(1,"begin...");

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  else
    osd_setup(2,"Initializing...");

  Serial.println(F("Connected!"));

  Serial.println(F("Request DHCP"));
  delay(1000);

  while (!cc3000.checkDHCP()) {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
  /* Display the IP address DNS, Gateway, etc. */
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  osd_setup(4,"Init is ok");
  delay(1000);


  if(!voCC3000_get_server_ip()) {
    volcdsetup("4/5 server ERROR",10,15);
    delay(1000);
    Serial.println("\n 4/5 server ERROR");
  } 
  else {
    volcdsetup("4/5 server OK",10,15);
    delay(1000);
    Serial.println("\n 4/5 server OK");

    //----------------------------------------------------------
    volcdsetup("5/5 pos/rec READY",10,15);

    if(voCC3000_ping(0)) {
      NET_WEBSITE_sta = true;
    }  
    else {
      NET_WEBSITE_sta = false;
      Serial.println(F("\n pos ERROR"));
    }

    voCC3000_rec();

    if(NET_WEBSITE_sta) {
      volcdsetup("5/5 pos/rec OK",10,15);
      delay(1000);
      Serial.println("\n 5/5 pos/rec OK");
    } 
    else {
      volcdsetup("5/5 pos/rec ERROR",10,15);
      delay(1000);
      Serial.println("\n 5/5 pos/rec ERROR");
    }

  }

  if(!NET_WEBSITE_sta) {
    volcdsetup("---reconnect---",10,15);
    Serial.println("\n ---reconnect---");

    if(voCC3000_reconnect()) {
      volcdsetup("--recon. OK--",10,15);
      Serial.println("\n --recon. OK--");
    } 
    else {
      volcdsetup("-recon. ERROR-",10,15);
      delay(2000);
      Serial.println("\n -recon. ERROR-");
    }
  }
}
