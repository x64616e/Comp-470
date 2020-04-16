#include <Hercules.h>
void setup()
{
MOTOR.begin();
}
void loop()
{
MOTOR.setSpeedDir(30, DIRF);
delay(3000);
MOTOR.setSpeedDir(0, DIRF);
delay(500);
MOTOR.setSpeedDir(30, DIRR);
delay(3000);
MOTOR.setSpeedDir(0, DIRF);
delay(500);
}


