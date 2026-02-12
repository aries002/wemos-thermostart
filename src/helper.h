#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifndef AP_SSID
#define AP_SSID "ESP AP"
#endif

bool ICACHE_FLASH_ATTR conect_wifi(String ssid = "",String password = "", bool client = false, bool fallback = true){
  if(WiFi.status() == WL_CONNECTED){
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.print("Memutuskan hubungan WIFI.");
    int timeout1 = 0;
    delay(100);
    while (WiFi.status() == WL_CONNECTED)
    {
      Serial.print(".");
      delay(1000);
      timeout1++;
    }
    Serial.println();
    
  }
  if(ssid == ""){
    WiFi.mode(WIFI_AP);
    Serial.println("WiFi diset ke mode Aksess point");
    WiFi.softAP(AP_SSID);
    Serial.print("AP SSID = ");
    Serial.println(AP_SSID);
  }else if (!client)
  {
    WiFi.mode(WIFI_AP);
    Serial.println("WiFi diset ke mode Aksess point");
    WiFi.softAP(ssid, password);
    Serial.print("AP SSID  = ");
    Serial.println(ssid);
    Serial.print("Password = ");
    Serial.println(password);
  }else if (client && ssid != "")
  {
    WiFi.mode(WIFI_STA);
    Serial.println("WiFi diset ke mode client");
    Serial.print("Menyambungkan ke ");
    Serial.print(ssid);
    WiFi.begin(ssid,password);
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout <= 30)
    {
      Serial.print(".");
      delay(1000);
      timeout++;
    }
    Serial.println();
    if(timeout >= 30 || WiFi.status() != WL_CONNECTED){
      Serial.println("Koneksi WiFi gagal!\n(Timeout)");
      if(fallback){
        Serial.println("Mengaktifkan backup WiFi AP");
        WiFi.softAP(AP_SSID);
        Serial.print("AP SSID = ");
        Serial.println(AP_SSID);
        Serial.print("Alamat IP = ");
        Serial.println(WiFi.softAPIP());
      }
      return false;
    }
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Terkoneksi!");
      Serial.print("Alamat IP = ");
      Serial.println(WiFi.localIP());
    }
  }
  return true;
  
  
}

String ICACHE_FLASH_ATTR buat_token(int panjang = 20){
  char karakter[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  String letter;
  // randomSeed(analogRead(0));
  for(int i = 0; i < panjang; i++){
    letter = letter + karakter[random(0,61)];
  }
  return letter;
}

void ICACHE_FLASH_ATTR i2c_scanner(){
  byte error, address;
  int nDevices;
  Serial.println("Scanning i2c devices...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    // Serial.println(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}
