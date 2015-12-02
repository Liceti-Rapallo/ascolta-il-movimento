const int Sensor1TrigPin=7;
const int Sensor1EchoPin=6;
const int Sensor2TrigPin=3;
const int Sensor2EchoPin=2;

const int TargetOutputHz=5; //obiettivo in HZ

double MeasureMsecs;
double MillisStore[2];
double timeDelta;

int SmoothingValue; //da 1 a 30, media
int lastResults[30];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Sensor1TrigPin,OUTPUT);
  pinMode(Sensor1EchoPin,INPUT);
  pinMode(Sensor2TrigPin,OUTPUT);
  pinMode(Sensor2EchoPin,INPUT);
  
  MeasureMsecs=0;
  MillisStore[0]=0;
  MillisStore[1]=millis();
  timeDelta=0;

  setSmoothingSampleSize(5);
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
    
    MeasureMsecs=millis();
    
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

void setSmoothingSampleSize(int x){if(x<31&&x>0){SmoothingValue=x;}else{SmoothingValue=10;}}
double getDelta(int x,int y){return MillisStore[x]-MillisStore[y];}
void printDelta(int x,int y){Serial.print(" "); Serial.print(getDelta(x,y));}
void optimizeSampling(){
  int desideredMsec=(1000/TargetOutputHz)/2;
  //DA FINIRE
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(measure(Sensor1TrigPin,Sensor1EchoPin));
  MillisStore[0]=MeasureMsecs;
  printDelta(0,1);
  Serial.print(" ");
  Serial.print(measure(Sensor2TrigPin,Sensor2EchoPin));
  MillisStore[1]=MeasureMsecs;
  printDelta(1,0);
  Serial.print(" ");
  timeDelta=MillisStore[1]-MillisStore[0];
  Serial.println();
  //delay(20);
}
