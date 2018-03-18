#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2,1,0,4,5,6,7,3,POSITIVE);

long minDist=10;
int buzzer=12;

struct distSensor {
  int trigPin;
  int echoPin;
};

struct distSensor ds[4];

void setup() {
  // put your setup code here, to run once:
  ds[0].trigPin=10;
  ds[0].echoPin=11;
  ds[1].trigPin=8;
  ds[1].echoPin=9;
  ds[2].trigPin=6;
  ds[2].echoPin=7;
  ds[3].trigPin=4;
  ds[3].echoPin=5;
  
  for (int i=0;i<4;i++){
    pinMode(ds[i].trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(ds[i].echoPin, INPUT); // Sets the echoPin as an Input
  }

  lcd.begin(16,2);
  lcd.clear();
  lcd.print("hello, world");
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Prints the distance on the Serial Monitor
  for(int i=0;i<4;i++){
    Serial.print("Distance sensor ");Serial.print(i);Serial.print(": ");
    Serial.println(getDistanceForSensor(ds[i]));
  }
  Serial.println();
  if (isTooClose())
  {
    ringBuzzerFor(getMinDist());
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}

long getDistanceForSensor(distSensor ds)
{
  long duration;
  int distance;
  // Clears the trigPin
  digitalWrite(ds.trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ds.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ds.trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ds.echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}


int isTooClose()
{
  for (int i=0;i<4;i++)
  {
    if(getDistanceForSensor(ds[i])<=minDist)
    {
      return 1;  
    }
  }
  return 0;
}

void ringBuzzerFor(long dist)
{
  int ringDuration=dist/minDist*1000;
  digitalWrite(buzzer, HIGH);
  delay(150);
  digitalWrite(buzzer, LOW);
  delay(ringDuration);
}


long getMinDist()
{
  long minDist=getDistanceForSensor(ds[0]);
  for(int i=1;i<4;i++)
  {
    if (minDist>getDistanceForSensor(ds[i]))
    {
      minDist=getDistanceForSensor(ds[i]);  
    }
  }
  return minDist;
}



