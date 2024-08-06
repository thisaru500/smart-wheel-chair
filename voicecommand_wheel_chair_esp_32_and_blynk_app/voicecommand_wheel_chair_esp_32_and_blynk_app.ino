#include <HardwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/**        
 *         
 *       
  Connection
  ESP32    VoiceRecognitionModule
   16   ------->     TX
   17   ------->     RX
   12   ------->     Trig (Ultrasonic)
   14   ------->     Echo (Ultrasonic)
*/
int mrw1 = 4;
int mrw2 = 5;
int mrw3 = 18;
int mrw4 = 19;

// for forward left and right indicators

int forward_led = 23;
int left_led = 22;
int right_led = 1;
int stop_led = 3;

// Ultrasonic sensor pins
const int trigPin = 12;
const int echoPin = 14;

HardwareSerial mySerial(2); // Use UART2
VR myVR(&mySerial); // Use the hardware serial

uint8_t records[7]; // save record
uint8_t buf[64];

int t = 1500; // Left Right delay
int s = 8000; // Forward delay

#define Forward   (1)
#define Left      (3)
#define Right     (4)
#define Stop      (5)

long duration;
int distance;

// Blynk credentials
char auth[] = "YourAuthToken";
char ssid[] = "YourSSID";
char pass[] = "YourPassword";

void setup() {
  /** initialize */
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(mrw1, OUTPUT);
  pinMode(mrw2, OUTPUT);
  pinMode(mrw3, OUTPUT);
  pinMode(mrw4, OUTPUT);

  pinMode(forward_led, OUTPUT);
  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);
  pinMode(stop_led, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(mrw1, LOW);
  digitalWrite(mrw2, LOW);
  digitalWrite(mrw3, LOW);
  digitalWrite(mrw4, LOW);

  digitalWrite(forward_led, LOW);
  digitalWrite(left_led, LOW);
  digitalWrite(right_led, LOW);
  digitalWrite(stop_led, LOW);
    
  if(myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }

  if(myVR.load((uint8_t)Forward) >= 0) {
    Serial.println("Forward");
  }

  if(myVR.load((uint8_t)Left) >= 0) {
    Serial.println("Left");
  }

  if(myVR.load((uint8_t)Right) >= 0) {
    Serial.println("Right");
  }

  if(myVR.load((uint8_t)Stop) >= 0) {
    Serial.println("Stop");
  }

  // Blynk setup
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  int ret = myVR.recognize(buf, 50);
  if(ret > 0) {
    switch(buf[1]) {
     case Forward:
       if (getDistance() > 30) { // Only move forward if no obstacle is detected
         digitalWrite(forward_led, HIGH);
         digitalWrite(stop_led, LOW);
          
         digitalWrite(mrw1, HIGH);
         digitalWrite(mrw2, LOW);
         digitalWrite(mrw3, HIGH);
         digitalWrite(mrw4, LOW);

         delay(s);

         digitalWrite(forward_led, LOW);
         digitalWrite(mrw1, LOW);
         digitalWrite(mrw2, LOW);
         digitalWrite(mrw3, LOW);
         digitalWrite(mrw4, LOW);
       }
       break;
       
     case Left:
       digitalWrite(stop_led, LOW); 
       digitalWrite(left_led, HIGH);
       digitalWrite(mrw1, HIGH);
       digitalWrite(mrw2, LOW);
       digitalWrite(mrw3, LOW);
       digitalWrite(mrw4, LOW);
         
       delay(t);

       digitalWrite(left_led, LOW);
       digitalWrite(mrw1, LOW);
       digitalWrite(mrw2, LOW);
       digitalWrite(mrw3, LOW);
       digitalWrite(mrw4, LOW);

       break;

     case Right:
       digitalWrite(stop_led, LOW);  
       digitalWrite(right_led, HIGH);     
       digitalWrite(mrw1, LOW);
       digitalWrite(mrw2, LOW);
       digitalWrite(mrw3, LOW);
       digitalWrite(mrw4, HIGH); 

       delay(t);
       
       digitalWrite(right_led, LOW);
       digitalWrite(mrw1, LOW);
       digitalWrite(mrw2, LOW);
       digitalWrite(mrw3, LOW);
       digitalWrite(mrw4, LOW);

       break;

     case Stop:
       digitalWrite(stop_led, HIGH);
       digitalWrite(forward_led, LOW);
       digitalWrite(left_led, LOW);
       digitalWrite(right_led, LOW);
       digitalWrite(mrw1, LOW);
       digitalWrite(mrw2, LOW);
       digitalWrite(mrw3, LOW);
       digitalWrite(mrw4, LOW);
       break;
       
     default:
       Serial.println("Record function undefined");
       break;
    }
    /** voice recognized */
    printVR(buf);
  }
}

int getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  return distance;
}

// Blynk virtual pin handlers
BLYNK_WRITE(V1) {
  int command = param.asInt();
  switch(command) {
    case 1: // Forward
      if (getDistance() > 30) {
        digitalWrite(forward_led, HIGH);
        digitalWrite(stop_led, LOW);

        digitalWrite(mrw1, HIGH);
        digitalWrite(mrw2, LOW);
        digitalWrite(mrw3, HIGH);
        digitalWrite(mrw4, LOW);

        delay(s);

        digitalWrite(forward_led, LOW);
        digitalWrite(mrw1, LOW);
        digitalWrite(mrw2, LOW);
        digitalWrite(mrw3, LOW);
        digitalWrite(mrw4, LOW);
      }
      break;
    case 2: // Left
      digitalWrite(stop_led, LOW);
      digitalWrite(left_led, HIGH);
      digitalWrite(mrw1, HIGH);
      digitalWrite(mrw2, LOW);
      digitalWrite(mrw3, LOW);
      digitalWrite(mrw4, LOW);

      delay(t);

      digitalWrite(left_led, LOW);
      digitalWrite(mrw1, LOW);
      digitalWrite(mrw2, LOW);
      digitalWrite(mrw3, LOW);
      digitalWrite(mrw4, LOW);
      break;
    case 3: // Right
      digitalWrite(stop_led, LOW);
      digitalWrite(right_led, HIGH);
      digitalWrite(mrw1, LOW);
      digitalWrite(mrw2, LOW);
      digitalWrite(mrw3, LOW);
      digitalWrite(mrw4, HIGH);

      delay(t);

      digitalWrite(right_led, LOW);
      digitalWrite(mrw1, LOW);
      digitalWrite(mrw2, LOW);
      digitalWrite(mrw3, LOW);
      digitalWrite(mrw4, LOW);
      break;
    case 4: // Stop
      digitalWrite(stop_led, HIGH);
      digitalWrite(forward_led, LOW);
      digitalWrite(left_led, LOW);
      digitalWrite(right_led, LOW);
      digitalWrite(mrw1, LOW);
      digitalWrite(mrw2, LOW);
      digitalWrite(mrw3, LOW);
      digitalWrite(mrw4, LOW);
      break;
    default:
      Serial.println("Undefined command");
      break;
  }
}
