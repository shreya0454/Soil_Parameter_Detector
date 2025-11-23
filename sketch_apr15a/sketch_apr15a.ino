#define BLYNK_TEMPLATE_ID "TMPL3VZbgjTYJ"
#define BLYNK_TEMPLATE_NAME "mp project"
#define BLYNK_AUTH_TOKEN "zNIC_Dp4ZZAruq6-TZgz-o3rPPoGx-_O"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "zNIC_Dp4ZZAruq6-TZgz-o3rPPoGx-_O";
char ssid[] = "shreya's A35";     
char pass[] = "gawadethegreat"; 

BlynkTimer timer;

#define DHTPIN 4        
#define DHTTYPE DHT11   
#define PH_SENSOR_PIN A0 
#define RELAY_PIN D3     

int pinValue; 
DHT dht(DHTPIN, DHTTYPE);

float ph(float voltage) {
  return (7 + ((2.5 - voltage) / 0.18));
}

BLYNK_WRITE(V4) {
  pinValue = param.asInt();
  if (pinValue == 1) {
    digitalWrite(RELAY_PIN, HIGH); 
  } else {
    digitalWrite(RELAY_PIN, LOW); 
  }
}

void sendSensorData() {
  if (pinValue == 1) {
    
    float phValue = 0;

    for (int i = 0; i < 10; i++) {
      phValue += analogRead(PH_SENSOR_PIN);
      delay(10);
    }

    phValue /= 10;

    float voltage = (phValue * 5.0 / 1023.0);
    
    float pH = -6.91 * voltage + 35.88;

    Serial.println(voltage);
    Serial.print("pH= ");
    Serial.print(pH);
    Serial.print(" | ");
    Serial.println(ph(voltage)); 
    Blynk.virtualWrite(V3, pH); 
    Blynk.virtualWrite(V0, -1); 
    Serial.print("pH Value: ");
    Serial.println(pH);
  } else {
    
    float value = analogRead(A0);
    value = map(value, 400, 1023, 100, 0);

    Blynk.virtualWrite(V0, value); 
    Blynk.virtualWrite(V3, -1); 
    Serial.print("Soil Moisture : ");
    Serial.print(value);
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V1, t);    
  Blynk.virtualWrite(V2, h);    
  Serial.print(" | Temperature : ");
  Serial.print(t);
  Serial.print(" | Humidity : ");
  Serial.println(h);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(PH_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 
  timer.setInterval(1000L, sendSensorData); 
}

void loop() {
  Blynk.run();
  timer.run();
}