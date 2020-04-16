#include <Hercules.h>
#include <math.h>

//Define Constants (old)
//int TENSPEEDONEMETER = 5681;
//int TWENTYSPEEDONEMETER = 3052;
//double TWENTY_IN_METERS_PER_SECOND = 0.327654;

int rampDelay(int power) {
  //returns a proportional ramping delay...
  //eg ramp to 50% = 1000 ms , ramp to 100% = 2000ms
  return abs(power) * 20;
}

double speedAt(int power){
  //returns the speed in meters per second for a given power
  //    power arc calculated with x = ay^2 + by + c
  //    where x is input power (0-100) and y is measured speed in Meters/Second
  double a = 27.8963;
  double b = 51.8997;
  double c = 0;
  double ret = 0.0;
  if (power >=0 && power <=100) {
    ret = (-b+sqrt(pow(b,2)-(4*a*c)+(4*a*(double)power)))/(2*a);
    //double y2 = (-b-sqrt(pow(b,2)-(4*a*c)+(4*a*(double)power)))/(2*a);
    //ret = max(y1, y2);
  }
  return ret;
}

void ramp(int startPower, int endPower, int time) {
  //ramps the motor power up or down over a set time (in milliseconds)
  int steps = abs(endPower - startPower);
  if (endPower > startPower) {  // speed up
    for (int i = startPower; i <= endPower; i++) {
      MOTOR.setSpeedDir(i, DIRF);
      delay(time / steps);
    }//end for
  } else if (startPower > endPower) {  // slow down
    for (int i = startPower; i >= endPower; i = i + ((endPower - startPower) / steps)) {
      MOTOR.setSpeedDir(i, DIRF);
      delay(time / steps);
    }//end for
  }//end if
}//end ramp

double rampDistance(int startPower, int endPower, int time) {
  //returns the distance in Meters travelled while ramping from one speed
  //to another over a set time (in milliseconds) using Riemann sum
  int steps = abs(endPower - startPower);
  double dist = 0.0;
  for (int i = startPower; i <= endPower; i++) {
    //distance in 1 step = time in seconds * speed in Meters/sec
    dist += (((double)time/1000)/(double)steps) * speedAt(i);
  }
  return(dist);
}

void drive(int power, double distance) {
  //calculate the distance travelled during ramping up and down
  double distanceRemaining = distance;
  distanceRemaining -= rampDistance(0, power, rampDelay(power)); //speed up
  distanceRemaining -= rampDistance(power, 0, rampDelay(power)); //slow down
  int delayTime = 1000 * ((1 / speedAt(power)) * distanceRemaining);
  ramp(0, power, rampDelay(power));
  delay(delayTime);
  ramp(power, 0, rampDelay(power));
}

void turnRight() {
  MOTOR.setSpeedDir1(30, DIRF);
  MOTOR.setSpeedDir2(30, DIRR);
  delay(620);
  MOTOR.setSpeedDir(0, DIRF);
}

void turn(float degrees, int power) {
  int delayConstant = 2500;  //this is hardcoded 360 degree turn at 30 speed
  int delayTime = (int)(degrees * delayConstant / 360);
  MOTOR.setSpeedDir1(30, DIRF);
  MOTOR.setSpeedDir2(30, DIRR);
  delay(delayTime);
  MOTOR.setSpeedDir(0, DIRF);

}


void runCircuit(int power, int stepDelay, int loopDelay) {
  drive(power,1);
  delay(stepDelay);
  turn(90,20);
  delay(stepDelay);
  drive(power,2);
  delay(stepDelay);
  turn(90,20);
  delay(stepDelay);
  drive(power,1);
  delay(stepDelay);
  turn(90,20);
  delay(stepDelay);
  drive(power,2);
  delay(stepDelay);
  turn(90,20);
  delay(loopDelay);
}



void setup()
{

  MOTOR.begin();
  Serial.begin(9600);
  Serial.print("Serial begin \n");

}


void loop()
{
  turn(360, 20);
  delay(3000);

  //runCircuit(20, 2000, 10000);

}
