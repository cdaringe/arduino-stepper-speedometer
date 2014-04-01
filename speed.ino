/*
 * Simple Speedometer
 * Chris Dieringer
 * March 2014
 */
volatile int pulseCount = 0;
int sensepin = 2;
int dirpin = 11;
int steppin = 12;

#define MAXSPEED 9000 //mm/s (9000mm/s ~20mph)
#define MAXSPEEDANGLE 180 //max range of the dial
#define MINSPEEDANGLE 0 //Keep at 0
#define STEPSPERREV (200*8) //consult your motor and stepper driver board.  Scale to compensate for microsteps.  Easy driver with no addons defaults to 8 microsteps/step
#define WHEELDIA 25.4 //mm
#define NUMSENSORS 1 //i.e., if you may have 2 wheel magnets on a magnetic wheel sensor
const double DISTANCE_PER_PULSE = WHEELDIA*3.1415926/NUMSENSORS; //mm
#define SPEEDBUFFSIZE 10 //smoothing constant. larger:less-responsive, smaller:more-responsive. Recommend 3-20
#define T_INTERVAL 100 // (milliseconds) time duration for each capturing period.  Recommend as low as possible, so long as you can get substantial pulses within that duration
// NOTE : It will take SPEEDBUFFSIZE * T_INVERVAL time for the speedometer to return to 0 after the object's motion has stopped
#define MOTORDELAYMICRO 200 //delay between pulses for motor (microseconds).  You should make this as small as feasible, with a ~30-50 safety time added.  Start at 200, by 10's/20's up or down
const double PULSESTOMAXSPEED = (MAXSPEED*(T_INTERVAL/1000))/DISTANCE_PER_PULSE;

double interval_end;  //marks the end of a T_INTERVAL period of data collection
int speedCounts[SPEEDBUFFSIZE]; //array of speed/pulse counts
int currentStep = 0;



void setup() {
  Serial.begin(38400);
  Serial.println("Speedometer logging started...");
  Serial.print("Pulses to max speed: ");
  Serial.println(PULSESTOMAXSPEED);
  pinMode(sensepin, INPUT);
  digitalWrite(sensepin,HIGH);
  pinMode(dirpin, OUTPUT);
  pinMode(steppin, OUTPUT);
  attachInterrupt(0, pulseSense, FALLING);
  interval_end = millis() + T_INTERVAL;
}



void pulseSense() {
  ++pulseCount;  
}



void loop() {
  if (millis() > interval_end) {
    shiftArray(speedCounts,SPEEDBUFFSIZE);
    speedCounts[0] = pulseCount;
    pulseCount = 0;
    interval_end += T_INTERVAL;
    moveDial();
  }
}



//shifts all array elements to the right
//element of array[0] set to 0
void shiftArray(int* arr, int arrSize) {
  for (int k = arrSize-1; k > 0; k--) 
  {   
    arr[k]=arr[k-1];
  }
  arr[0]=0;
  return;
}


/**
 * convert speed data to dial position, and command dial to that position
 */
void moveDial() {

   double approxDestStep = 0;
   double currSpeed;
   for (int i = 0; i<SPEEDBUFFSIZE; ++i) {
     currSpeed = speedCounts[i]/PULSESTOMAXSPEED;
     if (currSpeed >= MAXSPEED) currSpeed = MAXSPEED;
     approxDestStep += map(currSpeed, 
                            0, 
                            MAXSPEED, 
                            MINSPEEDANGLE, 
                            MAXSPEEDANGLE)
                            /SPEEDBUFFSIZE; //creates average of past speeds
   }
   int destStep = (int) approxDestStep;
  
   //Uncomment below to monitor your motion vitals
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
