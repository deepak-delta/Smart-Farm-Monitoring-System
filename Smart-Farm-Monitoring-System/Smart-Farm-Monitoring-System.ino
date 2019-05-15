//author : Deepak D Krishna


#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>


char auth[] = "your auth key";


// Set password to "" for open networks.
char ssid[] = "your network ssid";
char pass[] = "your network password";

#define DHTPIN 2    // Digital pin 4


#define soilMoisterPin A0
#define soilMoisterVcc D8
int soilMoister = 0;
#define PUMP_PIN D6


#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;


//###########################################################################

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature


}

//###############################################################################
void getSoilMoisterData(void)
{
  soilMoister = 0;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);
  int N = 1;
  for(int i = 0; i < N; i++)
  {
    soilMoister += analogRead(soilMoisterPin);
    delay(150);
  }
  digitalWrite (soilMoisterVcc, LOW);
  soilMoister = soilMoister/N;

  soilMoister =(1024-soilMoister);
  //soilMoister =(100-(soilMoister*100));

  Serial.println(soilMoister);
  Blynk.virtualWrite(V7, soilMoister);

  }
//**********************************************************************************
//**********************************************************************************
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(soilMoisterVcc, OUTPUT);
  digitalWrite (soilMoisterVcc, LOW);

  dht.begin();


  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, getSoilMoisterData);
}


void autoControlPlantation(void)
{
  if (soilMoister <= 100)
  {
    turnPumpOn();
  }

}

void turnPumpOn()
{
  digitalWrite(PUMP_PIN , HIGH);
  Serial.println("PUMP ON");
  delay(3000);
  digitalWrite(PUMP_PIN , LOW);
  Serial.println("PUMP OFF");

}
//#################################################################################
void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
  autoControlPlantation();


}

//################################################################################

