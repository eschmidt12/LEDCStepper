#include <driver/ledc.h>
//#include <driver/pcnt.h>  //will be required for future implementation of position control
#include <TMCStepper.h>     //this example is built for TMC2209 drivers. update for the motor driver of your choice.
#include <LEDCStepper.h>

#define EN_PIN 26           //enable pin
#define DIR_PIN 33          //direction pin
#define STEP_PIN 25         //step pin
#define LED_CHANNEL 0       //LEDC channel
#define SERIAL_PORT Serial2 //serial port for UART communication to driver
#define DRIVER_ADDRESS 0    //driver address for UART communication
#define R_SENSE 0.11F       //sense resistor value, refer to TMCStepper documentation

TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

const long micsteps = 256;            //microstep value - 1 (full step) - 256 (max microstep) in powers of 2
const long stepsPerRev = 200;         //steps per revolution
long revolutionsPerSecond = 4;                         //revolutions per second
float freqUpdatePeriod = 10.0;        //delay between frequency step intervals during acceleration moves, in microseconds.
float rampTimeInSeconds = 1;          //ramp time to next speed

const int stepperCurrentRMS = 750;    //RMS stepper current

LEDCStepper stepper;

//////////////////setup/////////////////////

void setup() {
  stepper.connectToPins(STEP_PIN, DIR_PIN, LED_CHANNEL);
  stepper.setMicsteps(micsteps);
  stepper.setFreqUpdatePeriod(freqUpdatePeriod);

  Serial.begin(115200);
  Serial.println("Serial open");

  Serial2.begin(115200);

  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);             // Enable driver in hardware
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);             // Set direction

  driver.begin();                        // Init UART drivers
  driver.toff(5);                        // Enables driver in software
  driver.rms_current(stepperCurrentRMS); // Set motor RMS current
  driver.microsteps(micsteps);           // Set microsteps
  driver.en_spreadCycle(false);          // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);            // Needed for stealthChop
}

//////////////////loop/////////////////////

//
//non-blocking acceleration move - processAccelerate() can be called at any time
//multiple steppers can be controlled simultaneously
//call processAccelerate once per cycle, at least as often as your freqUpdatePeriod
//

//accelerate to revolutionsPerSecond value, wait 3 seconds, then decelerate to 1 RPS, wait 3 seconds, and repeat

void loop() {
  stepper.setupAccelerate(revolutionsPerSecond * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
  while (!stepper.motionComplete()) {
    stepper.processAccelerate();
  }
  delay(3000);
  stepper.setupAccelerate(1 * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
  while (!stepper.motionComplete()) {
    stepper.processAccelerate();
  }
  delay(3000);
}

//
//blocking acceleration move
//

//void loop() {
//  stepper.accelerate(revolutionsPerSecond * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  delay(3000);
//  stepper.accelerate(1 * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  delay(3000);
//}

//
//non-blocking acceleration move - processAccelerateSerialOut() can be called at any time: alternate loop code if you want to see the frequency calculations and set points during accelerations
//multiple steppers can be controlled simultaneously
//call processAccelerateSerialOut() once per cycle, at least as often as your freqUpdatePeriod
//

//void loop() {
//  Serial.println("Starting 4 rps accel");
//  stepper.setupAccelerate(revolutionsPerSecond * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  while (!stepper.motionComplete()) {
//    stepper.processAccelerateSerialOut();
//  }
//  Serial.println("Finished 4 rps accel");
//  delay(3000);
//  Serial.println("Starting 1 rps accel");
//  stepper.setupAccelerate(1 * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  while (!stepper.motionComplete()) {
//    stepper.processAccelerateSerialOut();
//  }
//  Serial.println("Finished 1 rps accel");
//  delay(3000);
//}


//
//blocking acceleration move: alternate loop code if you want to see the frequency calculations and set points during accelerations
//

//void loop() {
//  stepper.accelerateSerialOut(&Serial, revolutionsPerSecond * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  delay(3000);
//  stepper.accelerateSerialOut(&Serial, 1 * stepsPerRev, revolutionsPerSecond * stepsPerRev / rampTimeInSeconds);
//  delay(3000);
//}
