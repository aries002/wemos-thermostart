#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHTC3.h"
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

#include "konfig.h"
#include "web_pages.h"
#include "helper.h"

int toleransi = 1;
float setTemperature = 0;
float curentTemperature = 0;
float curentHummidity = 0;
bool status_kipas = false;
bool status_pemanas = false;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();


AsyncWebServer server(80);

bool shtc3_stat = true;



void read_sensors(){
  // read the humidity and temperature from
  sensors_event_t hummidity, temp;
  shtc3.getEvent(&hummidity, &temp);
  curentHummidity = hummidity.relative_humidity;
  curentTemperature = temp.temperature;
  // status_kipas = (digitalRead(KIPAS) == NYALA)? true:false;
  // status_pemanas = (digitalRead(PEMANAS) == NYALA)? true:false;
#ifdef DEBUG
  Serial.print("Temperatur : ");
  Serial.println(temp.temperature);
  Serial.print("Kelembapan : ");
  Serial.println(hummidity.relative_humidity);
#endif
}

void kipas(bool status){
  if(status == status_kipas){
    return;
  }
  if(status){
    Serial.println("Menyalakan kipas.");
    digitalWrite(KIPAS, NYALA);
    status_kipas = true;
  }else{
    Serial.println("Mematikan kipas");
    digitalWrite(KIPAS, MATI);
    status_kipas = false;
  }
}

void pemanas(bool status){
  if(status == status_pemanas){
    return;
  }
  if(status){
    Serial.println("Menyalakan pemanas");
    
    kipas(true);
    delay(1000);
    digitalWrite(PEMANAS, NYALA);
    status_pemanas = true;
  }else{
    Serial.println("Mematikan pemanas");
    digitalWrite(PEMANAS, MATI);
    status_pemanas = false;
  }
}

void thermostat(){
  // mematikan pemanas jika temperatur set ke 0
  if(setTemperature == 0 && status_pemanas){
    pemanas(false);
    delay(5000);
    kipas(false);
    return;
  }

  if(curentTemperature >= setTemperature + toleransi && status_pemanas){
    // matikan pemanas
    pemanas(false);
  }else if(curentTemperature <= setTemperature - toleransi && !status_pemanas){
    // nyalakan pemanas
    pemanas(true);
  }
}

String web_processor(const String& var){

  if(var == "TEMPERATUR"){
    String temp = String(curentTemperature);
    return temp;
  }else if(var == "KELEMBAPAN"){
    String humid = String(curentHummidity);
    return humid;
  }else if (var == "SET_TEMPERATURE"){
    String set_temp = String(setTemperature);
    return set_temp;
  }else if(var == "KIPAS_ON" && status_kipas){
    return "selected";
  }else if(var == "KIPAS_OFF" && !status_kipas){
    return "selected";
  }
  return String("");
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void web_setup(){

  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){
    // Serial.println("POST REQUESTED!!");
    if(request->hasParam("temp", true)){
      String input = request->getParam("temp",true)->value();
      Serial.print("Temperature set to ");
      Serial.println(input);
      setTemperature = input.toFloat();
    }
    if(request->hasParam("fan", true)){
      bool status = (request->getParam("fan",true)->value() == "on")?true:false;
      Serial.print("Kipas set to ");
      Serial.println(status);
      kipas(status);
    }
    request->send(200, "text/html", index_html, web_processor);
  });

  server.on("/config", HTTP_ANY, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", config_html, web_processor);
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    String response = "";
    if(request->hasParam("var")){
      String var = request->getParam("var")->value();
      if(var == "temperatur"){
        response = String(curentTemperature);
      }else if(var =="kelembapan"){
        response = String(curentHummidity);
      }else if(var == "status_kipas"){
        response = (status_kipas)?"ON":"OFF";
      }else if(var == "status_pemanas"){
        response = (status_pemanas)?"ON":"OFF";
      }
    }
    request->send(200,"text/html", response);
  });

  server.onNotFound(notFound);
  server.begin();
  Serial.println("Server started!");
}





void setup() {
  Serial.begin(BAUD_RATE);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.println("Memulai sistem...");
  pinMode(PIN_OUT1, OUTPUT);
  pinMode(PIN_OUT2, OUTPUT);
  digitalWrite(PIN_OUT1, MATI);
  digitalWrite(PIN_OUT2, MATI);

  // start sensor
  Wire.begin();
  delay(10);
  i2c_scanner();
  delay(100);

  if(! shtc3.begin()){
    shtc3_stat = false;
    Serial.println("Sensor error!");
  }
  read_sensors();

  conect_wifi(WIFI_SSID,WIFI_PASSWORD, true);
  web_setup();
  delay(100);

}

unsigned long last_run = 0;
void loop() {
  // put your main code here, to run repeatedly:

  // Run program on interval
  if(millis() >= last_run + RUN_INTERVAL){
    last_run = millis();
    // baca sensor
    read_sensors();
    // if(shtc3_stat){
    // run logic termostart
    thermostat();
    // }
  }

}