/*
 * Simple Speedometer
 * Chris Dieringer
 * March 2014
 */
volatile int pulseCount = 0;
int sensepin = 2;
int dirpin = 11;
int steppin = 12;

#define MAXSPEED 180
#define MINSPEED 0
#define PULSECOUNTMAXSPEED (8) //number of pulses to map to max speed
#define SPEEDBUFFSIZE 10

#define MOTORDELAYMICRO 200 //delay between pulses for motor.  You should make this as small as possible.  Start here

double interval_end;
int speedCounts[SPEEDBUFFSIZE]; //array of speed/pulse counts
int interval_t = 100; //ms

int currentStep = 0;



void setup() {
  Serial.begin(38400);
  Serial.println("Go go gadget speedometer!");
  pinMode(sensepin, INPUT);
  digitalWrite(sensepin,HIGH);
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
  attachInterrupt(0, pulseSense, FALLING);
  interval_end = millis() + interval_t;
}



void pulseSense() {
  ++pulseCount;  
}

void loop() {
  if (millis() > interval_end) {
    shiftArray(speedCounts,SPEEDBUFFSIZE);
    speedCounts[0] = pulseCount;
    pulseCount = 0; //reset counter
    interval_end += interval_t;
    moveDial();
  }
}

//shifts all elements to the right
//element of array[0] set to 0
void shiftArray(int* arr, int arrSize) {
  for (int k = arrSize-1; k > 0; k--) 
  {   
    arr[k]=arr[k-1];
  }
  arr[0]=0;
  return;
}

void moveDial() {
   //convert speed to position on dial

   //int destStep = map(speedCounts[0],0,PULSECOUNTMAXSPEED,MINSPEED,MAXSPEED);
   double approxDestStep =0;
   for (int i = 0; i<SPEEDBUFFSIZE; ++i) {
     approxDestStep += map(speedCounts[i],0,PULSECOUNTMAXSPEED,MINSPEED,MAXSPEED)/SPEEDBUFFSIZE; //creates average of past speeds
   }
   int destStep = (int) approxDestStep;
      

   //Serial.print("curr: "); Serial.print(currentStep);Serial.print(" // dest: "); Serial.print(destStep);Serial.print(" // pulses: "); Serial.println(speedCounts[0]);
   
    //set direction of move
    int stepInc = 0;
    if (destStep > currentStep) {
      digitalWrite(dirpin, LOW);
      stepInc = 1;
    } else {
      digitalWrite(dirpin, HIGH);
      stepInc = -1;
    }
   
    while (currentStep != destStep)
    {
      digitalWrite(steppin, LOW); 
      digitalWrite(steppin, HIGH); // easydriver acts on HIGH
      delayMicroseconds(MOTORDELAYMICRO);
      currentStep += stepInc;
    }
}
