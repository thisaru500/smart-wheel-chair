 int trigPin = 9;
 int echoPin = 10;
 int relay = 11;


long duration;
int distance;
int safetyDistance;

void setup() {
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
pinMode(relay, OUTPUT);
Serial.begin(9600); 
}

void loop() {

digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;

safetyDistance = distance;
if (safetyDistance <= 90){
  digitalWrite(relay, HIGH);
}
else{
  digitalWrite(relay, LOW);
  
}

Serial.print("Distance: ");
Serial.println(distance);
}
