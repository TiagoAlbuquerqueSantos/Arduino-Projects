
// https://www.youtube.com/watch?v=eu8N4_dykBw&list=PLGs0VKk2DiYyn0wN335MXpbi3PRJTMmex&index=33

#include <DHT.h>

#define DHTPIN 11
#define DHTTYPE DHT11

DHT myDHT(DHTPIN, DHTTYPE);

#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

int br=1152000;
int setupdelay = 1000;
int measdelay = 1000;

float tempC;
float tempF;
float hum;
float HIF;
float HIC;

String message;

int butPin = 10;
int butVal = 1;
int butValOld = 1;

void setup() {
    Serial.begin(br);
    myDHT.begin();
    delay(setupdelay);
    pinMode(butPin, INPUT_PULLUP);
    matrix.begin();
}

void loop() {
    butVal = digitalRead(butPin);

    if (butVal == 0 && butValOld == 1){

    tempC = myDHT.readTemperature(false);
    tempF = myDHT.readTemperature(true);
    hum = myDHT.readHumidity();
    
    if (isnan(tempC) || isnan(tempF) || isnan(hum)) {
        Serial.println("Falha ao ler dados do DHT11");
        return;
    }

    HIC = myDHT.computeHeatIndex(tempC, hum, false);
    HIF = myDHT.computeHeatIndex(tempF, hum, true);
    message = "Temp: "+String(tempC)+" deg C "+String(tempF)+"deg F" +String(hum)
    Serial.println(message);

    matrix.beginDraw();
    matrix.textScrollSpeed(75);
    matrix.textFont(Font_5X7);
    matrix.beginText(0, 1, 255, 0, 0);
    matrix.println(message);
    matrix.endText(SCROLL_LEFT);
    matrix.endDraw();

delay(measdelay);
}
butValOld = butVal;
}
