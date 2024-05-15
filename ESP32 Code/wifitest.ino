
#include "WiFi.h"
#include <AsyncUDP.h>
#include "BluetoothSerial.h"
#include <bits/stdc++.h>
#include <cstring>
#include <DHT11.h>
#include <Ultrasonic.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <SmoothProgress.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>




using namespace std;







Adafruit_MPU6050 mpu;
int movement = 0;

int eeer = 0;


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


string result;
//-----------------------------------------------------------------------------------start mic







#include <arduinoFFT.h>

const int SOUND_SENSOR_PIN = 34;
const int SAMPLES = 512;                 // Must be a power of 2
const double SAMPLING_FREQUENCY = 1000;  // Hz, must be less than 10000 due to ADC
int SOUND_THRESHOLD = 1850;              // Example threshold in decibel

// Constants for detectSoundPattern
const int MIN_SPIKES = 120;            // Minimum number of spikes to consider for a crying pattern
const int MAX_SPIKES = 200;           // Maximum number to avoid continuous noise
const double SPIKE_THRESHOLD = 15.0;  // Threshold for detecting a spike in intensity

// Constants for isBabyCrying
const double BABY_CRY_FREQ_MIN = 250.0;   // Minimum frequency for baby cry (in Hz)
const double BABY_CRY_FREQ_MAX = 550.0;   // Maximum frequency for baby cry (in Hz)
const double INTENSITY_THRESHOLD = 1870;  // Minimum intensity to consider
const int MATCH_THRESHOLD = 55;          // Number of matches within the frequency range to consider as baby crying

double vReal[SAMPLES];
double vImag[SAMPLES];
//arduinoFFT<double> FFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);


double readDecibelLevel() {
  int analogValue = analogRead(SOUND_SENSOR_PIN);
  return double(analogValue);  // Convert analog value to double
}

bool detectSoundPattern() {
  double lastIntensity = 0.0;
  int spikeCount = 0;
  bool isSpike = false;

  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(SOUND_SENSOR_PIN);
    double currentIntensity = vReal[i];

    if (!isSpike && currentIntensity > lastIntensity + SPIKE_THRESHOLD) {
      isSpike = true;
    } else if (isSpike && currentIntensity < lastIntensity) {
      spikeCount++;
      isSpike = false;
    }
    lastIntensity = currentIntensity;
    delay(20);
  }

  // Debug: Print spike count
   Serial.print("Spike Count: ");
   Serial.println(spikeCount);

  return spikeCount >= MIN_SPIKES && spikeCount <= MAX_SPIKES;
}

bool isBabyCrying(double frequencies[], double intensities[], int count) {
  int targetFrequencyMatches = 0;
  for (int i = 0; i < count; i++) {
    if (frequencies[i] >= BABY_CRY_FREQ_MIN && frequencies[i] <= BABY_CRY_FREQ_MAX && intensities[i] > INTENSITY_THRESHOLD) {
      // Debug: Print frequency and intensity
      // Serial.print("Frequency: ");
      // Serial.print(frequencies[i]);
      // Serial.print(" Intensity: ");
      // Serial.println(intensities[i]);
      targetFrequencyMatches++;
    }
  }

  // Debug: Print target frequency matches
   Serial.print("Target Frequency Matches: ");
   Serial.println(targetFrequencyMatches);

  return targetFrequencyMatches > MATCH_THRESHOLD;
}




















































//------------------------------------------------------------end mic




IPAddress connecteddev[10];
long long timee[10] = { 0 };



void add(IPAddress ip) {
  for (int i = 0; i < 10; i++) {
    if (connecteddev[i] == IPAddress()) {
      timee[i] = millis() / 1000;
      connecteddev[i] = ip;
      break;
    }
  }
}




bool check() {
  for (int i = 0; i < 10; i++) {

    if ((millis() / 1000) - timee[i] >= 25) {
      connecteddev[i] = IPAddress();
    }
  }


  bool found = false;
  for (int i = 0; i < 10; i++) {
    if (connecteddev[i] != IPAddress()) {
      found = true;
      break;
    }
  }

  if (found) {
    return true;
  } else {
    return false;
  }
}



// LCD variables
#define rs 4
#define en 32
#define d4 33
#define d5 27
#define d6 23
#define d7 19
#define BAR_STYLES_IN_PROGMEM
#include <BarStyleVT.h>
#include <BarStyleV2.h>
#include <BarStyle0.h>

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LCD dispA(lcd, inPROGMEM(barStyleVT));

SmoothProgressBar spb1(dispA, 2, 5, 1, 0);
SmoothProgressBar spb2(dispA, 2, 15, 1, 1);



//DHT sensor ----------------------------------
DHT11 dht11(13);



//ultrasonic variables --------------------------------------
#define TRIGGER_PIN 5
#define ECHO_PIN 18

float duration_us, distance_cm;





//connections variables -----------------------------
BluetoothSerial SerialBT;

AsyncUDP udp;


String ssid = "";
String password = "";

IPAddress deviceip;
int deviceport;







const int rxPin = 16;
const int txPin = 17;



void setup() {
Serial1.setPins( rxPin,txPin);
Serial1.begin(115200);
Serial.begin(115200);
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(100);



  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);












  SerialBT.disconnect();



  //microphone variables
  pinMode(SOUND_SENSOR_PIN, INPUT);  // Set sound sensor pin as input







  // delete old config
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPdisconnect(false);
  delay(1000);

  //  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);


  //------------------------------------------
  //lcd --------------------




  //-----------------------------------





  SerialBT.begin("ESP32test");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");




  // Serial.println(WiFi.localIP());
  // //test if it work even when we do it before begin the wifi
  // if (udp.listen(6859)) {

  //   udp.onPacket([](AsyncUDPPacket packet) {
  //   Serial.print("data : ");
  //   Serial.write(packet.data(), packet.length());
  //   Serial.println();
  //   });
  // }



  double dl = readDecibelLevel();

  // Debug: Print decibel level
  Serial.print("Decibel Level: ");
  Serial.println(dl);
  long long sum;
int i = 0;
  if (i < 20) {
    sum += dl;
    i++;
    delay(10);
  } else {
    i = 0;
    SOUND_THRESHOLD = sum / 20;
    sum = 0;
  }
}

enum whatToDo {
  makeAWifi,
  ConnectToWifi,
  stillNotSure
};



















whatToDo wd = stillNotSure;
enum connectionState {
  SETUP,    //this is new device in the app
  RESETUP,  //edit sittengs in the app
  CONNECT,  // connect to existening device in the app // it will be the same as setup  -----------------------------------
  RUNNING,
  reconnect  //in the page of getting data
};
connectionState connState = SETUP;

enum whatToInput {
  SSID,
  PASSWORD,
  WAIT
};
whatToInput in = WAIT;
String data;
char i = 0;
bool connected;
int BMW = 0;
void loop() {
  mDetect();
  temp();
  humidity();

  // if(SerialBT.available())
  // {
  // Serial.println(SerialBT.readStringUntil('\n'));
  // }
  if (Serial.available()) {
    //  const  char* xm = Serial.readString().c_str();
    String xm = Serial.readString();
    byte buffer[1024] = {};
    for (int i = 0; i < xm.length(); i++) {
      buffer[i] = xm[i];
    }
    // SerialBT.write(xm)

    udp.write(buffer, xm.length());
  }


  if(Serial1.available()>=2)
  {
  BMW =  Serial1.parseInt();
  Serial1.read();
  }

  switch (wd) {



    case ConnectToWifi:

      //this is the page that will handle the when connecting to existing wifi u will still make other enum and make a condition and this code will be part of its options
      switch (connState) {
        case SETUP:
          if (SerialBT.available()) {
            switch (in) {
              Serial.println(in);

              case SSID:
                ssid = SerialBT.readStringUntil('\n');
                Serial.println("im in ssid");

                in = PASSWORD;
                break;
              case PASSWORD:
                password = SerialBT.readStringUntil('\n');
                connected = true;
                Serial.println("im in pass");



                connectWifi();




                break;
              case WAIT:
                String inpu = SerialBT.readStringUntil('\n');
                if (inpu == "0") {
                  in = SSID;

                } else if (inpu == "1") {
                  in = PASSWORD;

                } else if (inpu == "2") {
                  in = WAIT;
                }

                break;
            }
          }
          break;

        case RUNNING:

          if (check()) {
            mai();
          } else {

            Serial.println("no devices connected");
            delay(2000);
          }

          delay(10);



          break;

          // case CONNECT:

          //   break;


        case reconnect:
          delay(1);
          break;
      }
      break;

    case makeAWifi:
      Serial.println("in make a wifi case");

      switch (connState) {
        case SETUP:
          Serial.print("in make a wifi, setup case  ");
          Serial.println(connState);

          // if (SerialBT.available()) {


          // }
          switch (in) {

            case SSID:
              ssid = SerialBT.readStringUntil('\n');
              Serial.println("im in ssid");
              Serial.println(ssid);


              in = PASSWORD;
              break;
            case PASSWORD:
              password = SerialBT.readStringUntil('\n');
              connected = true;
              Serial.println("im in pass");
              Serial.println(password);





              makeWiFi();




              break;
            case WAIT:
              Serial.println("in make a wifi,setup ,waiting for ssid and password");


              String inpu = SerialBT.readStringUntil('\n');
              Serial.println(inpu);

              if (inpu == "0") {
                Serial.println("sssisssssddd");

                in = SSID;

              } else if (inpu == "1") {
                in = PASSWORD;

              } else if (inpu == "2") {
                in = WAIT;
              }


              break;


              // microphonelevel  tempreture  humidity  ultrasonic
              //data = mic +" "+ temp +" "+ hum + " "+ ultrasonic;
              //udp.broadcast( "{mic}");
          }


          break;
        case RUNNING:
          if (check()) {
            mai();
            


            //  slcd();
          } else {

            Serial.println("no devices connected from make a wifi");
            delay(2000);
            // slcd();
          }

          delay(10);


          break;
      }
    case stillNotSure:

      if (SerialBT.available()) {
        int input = SerialBT.readStringUntil('\n').toInt();
        switch (input) {
          case 0:
            wd = makeAWifi;
            Serial.println("in make a wifi dicidsion");
            break;
          case 1:
            wd = ConnectToWifi;

            break;
          default:
            wd = stillNotSure;

            break;
        }
      }
      break;
  }
}

// void loop() {
//   // for(int i = 0;i<20;i++)
//   // {

//   // }


//   delay(1000);
// }

void mai() {

  string x[6];
  string names[6];
  names[0] = "mic ";
  names[1] = "distance ";
  names[2] = "temp ";
  names[3] = "humidity ";
  names[4] = "moved ";
  names[5] = "bpm ";
  x[0] = to_string(mic());
  x[1] = to_string(distance());
  x[2] = to_string(temp());
  x[3] = to_string(humidity());
  x[4] = to_string(movement);
  x[5] = to_string(BMW);

  result = names[0] + x[0] + "\n" + names[1] + x[1] + "\n" + names[2] + x[2] + "\n" + names[3] + x[3] + "\n" + names[4] + x[4] + "\n"  + names[5] + x[5] + "\n";



  // udp.write((uint8_t*)buffer, result.size());
  //Serial.println("sent");
  // temp();
  // humidity();
  // mic();
  // distance();
  // delay(3000);
}





//LM393 microphone sound sensor
//use digital or analog bin , vcc and ground

//the idea here is to use the michrophone sensor to inform you for example or do any thing such as lightning a led as in this code if the sound value get up on a specific value
float s_s = 0.034;
#define c_t_i 0.393701;


long duration;
float distenceCm;


int mic() {

  double decibelLevel = readDecibelLevel();

  // Debug: Print decibel level
  // Serial.print("Decibel Level: ");
  //Serial.println(decibelLevel);

  if (decibelLevel > SOUND_THRESHOLD) {
    if (detectSoundPattern()) {

      // Perform FFT and intensity analysis
      FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.compute(FFT_FORWARD);
      FFT.complexToMagnitude();

      double frequencies[SAMPLES / 2];
      double intensities[SAMPLES / 2];
      for (int i = 0; i < SAMPLES / 2; i++) {
        frequencies[i] = (i * SAMPLING_FREQUENCY) / SAMPLES;
        intensities[i] = vReal[i];
      }
      if (isBabyCrying(frequencies, intensities, SAMPLES / 2)) {
        Serial.println("Baby is crying!");
        return 1;
      } else {
        Serial.println("No baby crying detected.");
        return 2;
      }
    } else {
      Serial.println("No significant crying pattern detected.");
      return 0;
    }

  } else {
    Serial.println("No sound above the sound threshold");
    return 4;
  }
  // delay(100); // Wait for a second before next reading
}



float distance() {


  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN, HIGH);

  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  distenceCm = duration * s_s / 2;


  Serial.print("distance ");
  Serial.println(distenceCm);

  if(distenceCm < 58)
  {
    return 1;
  }
  else
  {

  return 0;
  }
Serial.print("distance ");
  Serial.println(distenceCm);
}

int temp() {
  int temp = dht11.readTemperature();

  Serial.println(temp);
  Serial1.print(temp);
  Serial1.print(",");
  //  dacWrite(25, temp);

  return temp;
}

int humidity() {
  int humidity = dht11.readHumidity();

    Serial.println(humidity);


    Serial1.println(humidity);
  
  // dacWrite(26, humidity);
  return humidity;
}



IPAddress local_ip(192, 168, 4, 1);  // Customize if needed
IPAddress subnet(255, 255, 255, 0);  // Standard subnet mask


void makeWiFi() {

  WiFi.softAPConfig(local_ip, local_ip, subnet);

  WiFi.softAP(ssid.c_str(), password.c_str());
  WiFi.softAPdisconnect(true);

  // Configure new SSID and password
  const char* new_ssid = "Your_New_SSID";
  const char* new_password = "Your_New_Password";

  // Start SoftAP with new credentials
  WiFi.softAP(ssid.c_str(), password.c_str());
  Serial.println(ssid.c_str());
  Serial.println(password.c_str());



  delay(1000);
  Serial.println("making WiFi..");




  if (connected) {


    SerialBT.println("1");
    delay(1000);
    SerialBT.println("192.168.4.1");


    Serial.println("made the WiFi network");
    if (udp.listen(6859)) {
      udp.onPacket([](AsyncUDPPacket packet) {
        if (connState == RUNNING) {


          deviceip = packet.remoteIP();
          deviceport = packet.remotePort();
          Serial.println(deviceip);
          Serial.println(deviceport);


          Serial.print("data : ");
          Serial.write(packet.data(), packet.length());
          //udp.writeTo(packet.data(), packet.length(), deviceip, deviceport);
          // packet.println("please");
          Serial.println();


          const char* buffer;
          buffer = result.c_str();


          udp.writeTo((uint8_t*)buffer, result.size(), deviceip, deviceport);
          add(deviceip);
          // packet.write((uint8_t*)buffer, result.size());


          //  AsyncUDPPacket packet2 = packet;
          //   int x =packet2.parseInt();


          // if (x == 1 && connState == RUNNING) {
          // Serial.println("before");
          //  mai(packet);
          // Serial.println("after");
          // }
          // vTaskDelay(1);



          /*
                    Serial.println("here1");
                      uint8_t* packetData = packet.data();
                      size_t packetLength = packet.length();  // Assuming packet.length() returns the length of the packet

                      // Create an array to save the packet data
                      uint8_t savedPacket[packetLength];

                    Serial.println("here2");



                    Serial.println(packetLength);
                      int res = 0;
                       for (size_t i = packetLength-1; i >= 0; --i) {
                       res+=  ( (int)packetData[i]-48) * pow(10,packetLength -i-1) ;  // Cast to int for stream insertion
                      }
                     // Serial.println(res);
                     
                    

                      if (connState == RUNNING && packetLength < 5) {


                       // SOUND_THRESHOLD = res;
                      Serial.print("changing sound threshold done succesfully  ");
                      Serial.println(SOUND_THRESHOLD);
                      } else {
                      Serial.println("data lenght exceeded");
                      }

                      */
        }
      });
    }


    in = WAIT;
    connState = RUNNING;
  } else {
    Serial.print("failed connecting :(");

    SerialBT.println("0");

    in = WAIT;
  }
}



void connectWifi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    i++;
    if (i > 40) {

      i = 0;
      connected = false;
      break;
    }

    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  if (connected) {

    Serial.println("Connected to the WiFi network");



    SerialBT.println("1");
    delay(1000);
    SerialBT.println(WiFi.localIP());
    if (udp.listen(6859)) {
      udp.onPacket([](AsyncUDPPacket packet) {
        if (connState == RUNNING) {


          deviceip = packet.remoteIP();
          deviceport = packet.remotePort();
          Serial.println(deviceip);
          Serial.println(deviceport);


          Serial.print("data : ");
          Serial.write(packet.data(), packet.length());
          //udp.writeTo(packet.data(), packet.length(), deviceip, deviceport);
          // packet.println("please");
          Serial.println();


          const char* buffer;
          buffer = result.c_str();


          udp.writeTo((uint8_t*)buffer, result.size(), deviceip, deviceport);
          add(deviceip);
          // packet.write((uint8_t*)buffer, result.size());


          //  AsyncUDPPacket packet2 = packet;
          //   int x =packet2.parseInt();


          // if (x == 1 && connState == RUNNING) {
          // Serial.println("before");
          //  mai(packet);
          // Serial.println("after");
          // }
          // vTaskDelay(1);



          /*
                    Serial.println("here1");
                      uint8_t* packetData = packet.data();
                      size_t packetLength = packet.length();  // Assuming packet.length() returns the length of the packet

                      // Create an array to save the packet data
                      uint8_t savedPacket[packetLength];

                    Serial.println("here2");



                    Serial.println(packetLength);
                      int res = 0;
                       for (size_t i = packetLength-1; i >= 0; --i) {
                       res+=  ( (int)packetData[i]-48) * pow(10,packetLength -i-1) ;  // Cast to int for stream insertion
                      }
                     // Serial.println(res);
                     
                    

                      if (connState == RUNNING && packetLength < 5) {


                       // SOUND_THRESHOLD = res;
                      Serial.print("changing sound threshold done succesfully  ");
                      Serial.println(SOUND_THRESHOLD);
                      } else {
                      Serial.println("data lenght exceeded");
                      }

                      */
        }
      });
    }


    in = WAIT;
    connState = RUNNING;
  } else {
    Serial.print("failed connecting :(");

    SerialBT.println("0");

    in = WAIT;
  }
}










void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  if (connState == RUNNING && wd == ConnectToWifi) {

    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.disconnect(true);
    SerialBT.disconnect();
    connState = SETUP;
    in = WAIT;
    connectWifi();
  }
}













void mDetect() {

  if (mpu.getMotionInterruptStatus() && millis() / 1000 - eeer >= 5) {
    eeer = millis() / 1000;

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");

    movement = 1;

  } else if (millis() / 1000 - eeer >= 10) {
    movement = 0;
  }

  delay(10);
}













// void display_temp_humidity() {
//   lcd.setCursor(0, 0);
//   lcd.print("Temp");
//   int level = 4 * temp() - 80;
//   spb1.showProgress(level);
//   lcd.setCursor(0, 1);
//   lcd.print(temp());
//   lcd.print((char)223);

//   // Humidity

//   lcd.setCursor(7, 0);
//   lcd.print("Humidity");
//   int h = 1.5 * humidity() - 60;
//   spb2.showProgress(h);
//   lcd.setCursor(7, 1);
//   lcd.print((int)humidity());
//   lcd.print("%");
// }




// void slcd() {
//   int t = (int)temp();
//   int h = (int)humidity();

//   lcd.setCursor(0, 0);
//   lcd.print("Temp");
//   spb1.showProgress(t);
//   lcd.setCursor(0, 1);
//   lcd.print(t);
//   lcd.print((char)223);

//   lcd.setCursor(7, 0);
//   lcd.print("Humidity");
//   lcd.setCursor(7, 1);
//   lcd.print(h);
//   spb2.showProgress(h);
// }
