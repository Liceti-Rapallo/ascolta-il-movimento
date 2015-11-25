const int Sensor1TrigPin=7;
const int Sensor1EchoPin=6;

const int SmoothingValue=20;
int lastResults[SmoothingValue];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2400);
  pinMode(Sensor1TrigPin,OUTPUT);
  pinMode(Sensor1EchoPin,INPUT);
}
 
long measure(int TrigPin,int EchoPin){
  int cm;
  for (int i=0;i<SmoothingValue;i++)
  {
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    int duration = pulseIn(EchoPin,HIGH,18000);
    if (!(duration==0)){
      lastResults[i]=duration/58;
    }else
    {
      lastResults[i]=0;
    }
  }
  int sum=0;
  int valid=0;
  for (int i=0;i<SmoothingValue;i++)
  {
    if ((!(lastResults[i]==0))&&(lastResults[i]>5)){
      sum+=lastResults[i];
      valid++;
    }
  }
  cm=sum/valid;
  if (cm>300 && valid<(((float)SmoothingValue)*0.70)){
    return -1;
  } else {
    return cm;
  }
}
int incomingByte=0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(measure(Sensor1TrigPin,Sensor1EchoPin));
  //Serial.print(" | ");
  //Serial.println(measure(Sensor1TrigPin,Sensor1EchoPin));
  delay(500);
}
