#include <Time.h>
#define TIME_HEADER "T"
#define TIME_REQUEST 7
const int tempPin = A0;
const int piezoPin = A1;
const int lightPin = A2;
const int PTHRESHOLD = 5;
const int LTHRESHOLD = 30;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  setSyncProvider(requestSync);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    Serial.print("Time set");
    processSyncMessage();
  }
  if(timeStatus() == timeSet){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  
  int tempVal = analogRead(tempPin);
  int piezoVal = analogRead(piezoPin);
  int lightVal = analogRead(lightPin);
  
  //Print time
  timeStamp();

  float tempVolt = (tempVal/1024.0)*5.0;
  float temp = (tempVolt -0.5)*100;
  Serial.print(", Temperature: ");
  Serial.print(temp);
  
  if (lightVal >= LTHRESHOLD){
    Serial.print(", Lights: ON");
  } else {
    Serial.print(", Lights: OFF");
  }
  
  if (piezoVal >= PTHRESHOLD){
    Serial.println(", Heater: ON");
  } else {
    Serial.println(", Heater: OFF");
  }
  
  delay(60000);
}

void timeStamp(){
  time_t t = now();
  Serial.print("Time: ");
  if(hour(t)<10){
    serial.print(" ");
  }
  Serial.print(hour(t));
  Serial.print(":");
  if(minute(t)<10){
    serial.print(" ");
  }
  Serial.print(minute(t));
  Serial.print(" ");
  Serial.print(day(t));
  Serial.print("/");
  Serial.print(month(t));
  Serial.print("/");
  Serial.print(year(t));
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


