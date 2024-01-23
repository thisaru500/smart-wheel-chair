
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

int mrw1 = 4;
int mrw2 = 5;
int mrw3 = 6;
int mrw4 = 7;

VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 8; 
int t = 1500; //Left Right delay
int s = 8000; // Forward and Backward delay

#define On        (0)
#define Forward   (1)
#define Backward  (2)
#define Left      (3)
#define Right     (4)
#define Stop      (5)
 
void printSignature(uint8_t *buf, int len)
{
  
  for(int i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(mrw1, OUTPUT);
  pinMode(mrw2, OUTPUT);
  pinMode(mrw3, OUTPUT);
  pinMode(mrw4, OUTPUT);

  digitalWrite(mrw1,LOW);
  digitalWrite(mrw2,LOW);
  digitalWrite(mrw3,LOW);
  digitalWrite(mrw4,LOW);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)On) >= 0){
    Serial.println("On");
  }
  
  if(myVR.load((uint8_t)Forward) >= 0){
    Serial.println("Forward");
  }

  if(myVR.load((uint8_t)Backward) >= 0){
    Serial.println("Backward");
  }

  if(myVR.load((uint8_t)Left) >= 0){
    Serial.println("Left");
  }

  if(myVR.load((uint8_t)Right) >= 0){
    Serial.println("Right");
  }

  if(myVR.load((uint8_t)Stop) >= 0){
    Serial.println("Stop");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
     case On:
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);

     digitalWrite(led,HIGH);
     delay(2000);
     digitalWrite(led,LOW);

    

     
        break;
        
    case Forward:
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,HIGH);
     digitalWrite(mrw3,HIGH);
     digitalWrite(mrw4,LOW);

      delay(s);
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);
      
     digitalWrite(led,HIGH);
     delay(2000);
     digitalWrite(led,LOW);
  
       break;

       case Backward:
     digitalWrite(mrw1,HIGH);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,HIGH);
     
       
     
        
       break;

         case Left:
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,HIGH);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);
         
       delay(t);

     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);

   digitalWrite(led,HIGH);
     delay(2000);
     digitalWrite(led,LOW);
        
       break;

          case Right:
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,HIGH);
     digitalWrite(mrw4,LOW); 

        delay(t);

     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);

     digitalWrite(led,HIGH);
     delay(2000);
     digitalWrite(led,LOW);
       break;

          case Stop:
     digitalWrite(mrw1,LOW);
     digitalWrite(mrw2,LOW);
     digitalWrite(mrw3,LOW);
     digitalWrite(mrw4,LOW);

     digitalWrite(led,HIGH);
     delay(2000);
     digitalWrite(led,LOW);
     
       break;
       
      default:
        Serial.println("Record function undefined");
        break;
    }
    printVR(buf);
  }
}
