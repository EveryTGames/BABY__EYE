#include <LiquidCrystal.h>
#include <SmoothProgress.h>





#include <Wire.h>
#include <MAX30105.h>
#include <heartRate.h>

#include <OneWire.h>
#include <DallasTemperature.h>


MAX30105 particleSensor;
const int DataPin = 4;
OneWire onewire(DataPin);
DallasTemperature sensor_temp(&onewire);
int temperatureC = 0;
int sensorpin = 2;
long lastBeat = 0;         // Time at which the last beat occurred
float beatsPerMinute = 0;  // Variable to hold the calculated beats per minute
byte rates[4];             // Array to hold last few heart rate readings for averaging
byte rateSpot = 0;         // Current position in the rate array
const int RATE_SIZE = 4;   // Total number of readings to take for average
float beatAvg = 0;         // Variable to hold the average heart rate










#define SWITH_LED 8
#define rs 2
#define en 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7
#define BAR_STYLES_IN_PROGMEM
#include <BarStyleVT.h>
#include <BarStyleV2.h>
#include <BarStyle0.h>

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LCD dispA(lcd, inPROGMEM(barStyleVT));

SmoothProgressBar spb1(dispA, 2, 5, 1, 0);
SmoothProgressBar spb2(dispA, 2, 15, 1, 1);



int humidityee = 0;
int  tempee = 0;


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  dispA.begin();
  delay(2000);
  lcd.clear();




 // pinMode(sensorpin, INPUT);
  /*/
 sensor_temp.begin();      
 /*/


  // if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {  // Use default I2C port, 400kHz speed
  //   Serial.println("MAX30102 was not found. Please check wiring/power.");
  //   while (1)
  //     ;
  // }
  // particleSensor.setup();                     // Configure sensor with default settings
  // particleSensor.setPulseAmplitudeRed(0x0A);  // Set Red LED amplitude
  // particleSensor.setPulseAmplitudeGreen(0);   // Turn off Green LED


}


void loop() {
 // brm();
  if(Serial.available() >= 4 )
  {
    Serial.println("in");
   tempee =  Serial.parseInt();
   Serial.read();
      humidityee =  Serial.parseInt();
   Serial.read();
  }
  display_temp_humidity();
}



void brm() {


  int sensorvalue = digitalRead(sensorpin);
  //  delay(1000);

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    // We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;  // Store this reading in the array
      rateSpot %= RATE_SIZE;                     // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++) {
        beatAvg += rates[x];
      }
      beatAvg /= RATE_SIZE;
    }
  }











  // Read the raw IR and Red LED values
  int32_t redValue = particleSensor.getRed();

  // Calculate the SpO2 value
  float ratio = (float)redValue / (float)irValue;
  float spo2Value = 104 - 17 * ratio;
  if (spo2Value < 70) {
    spo2Value = 70;
  } else if (spo2Value > 100) {
    spo2Value = 100;
  }

  // Print the values
  //lcd.print("IR: ");
  //Serial.print(irValue);
  if (irValue < 50000) {
    spo2Value = 0.00;
  }
  // Serial.print(", Red: ");
  // Serial.print(redValue);

  if (sensorvalue == HIGH) {
    if (spo2Value == 0) {
      Serial.println(0);

    } else {

      if (beatAvg < 60 || beatAvg > 150) {
        // lcd.clear();
        //       lcd.setCursor(0,0);
        //       lcd.print("BPM:prosessing..");
        //       lcd.setCursor(1,1);
        //       lcd.print("SpO2:");
        //       lcd.print(spo2Value);
        Serial.println(0);

      }


      else {
        // lcd.clear();
        // lcd.setCursor(0,0);
        // lcd.print("BPM=");
        // lcd.print(beatAvg);
        // lcd.setCursor(1,1);
        // lcd.print("SpO2:");
        // lcd.print(spo2Value);
        Serial.println(beatAvg);
      }
    }
  } else {
    // lcd.clear();
    // lcd.setCursor(0,0);
    //       sensor_temp.requestTemperatures();
    //       temperatureC = sensor_temp.getTempCByIndex(0);
    //       lcd.print("temp:");
    //       lcd.print((int) (temperatureC+5));
    //       delay(500);
  }
}








int temp() {



  // Serial.println(temp);
  return tempee;
}

int humidity() {

  // Serial.println(humidity);
  return humidityee;
}




void display_temp_humidity() {
  lcd.setCursor(0, 0);
  lcd.print("Temp");
  // int level = 4 * temp() - 80;
  // spb1.showProgressPct(level);
   lcd.setCursor(0, 1);
  lcd.print(temp());
  lcd.print((char)223);

  // Humidity

  lcd.setCursor(7, 0);
  lcd.print("Humidity");
  // int h = 1.5 * humidity() - 60;
  // spb2.showProgressPct(h);
  lcd.setCursor(7, 1);
  lcd.print((int)humidity());
  lcd.print("%");
}