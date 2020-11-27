/*
      ******************************************************************
      *                                                                *
      *      LEDCStepper Frequency Generator Stepper Motor Driver      *
      *                                                                *
      *               Copyright (c) Evan R. Schmidt, 2020              *
      *                                                                *
      ******************************************************************

  LEDCStepper - Library for controlling stepper motors with step/dir pins using the LEDC frequency generator peripheral

  Created by Evan Schmidt, November 27, 2020

  MIT License

  Copyright (c) 2020 Evan Schmidt

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished
  to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "Arduino.h"
#include "driver/ledc.h"
#include "LEDCStepper.h"

LEDCStepper::LEDCStepper()
{
  //
  // initialize constants
  //
  stepPin = 0;
  dirPin = 0;
  ledChannel = 0;
  pwmResolution = 3;			//1-13 bit PWM duty cycle. Lower resolution allows for higher frequencies. 40 MHz step rate capable with 1 bit resolution, defaulted to 50% duty cycle. 5kHz step rate capable with 13 bit full PWM duty cycle resolution. Full duty cycle resolution is not required for this application. Recommend leaving this at ~3 bit resolution unless further duty cycle adjustment is required.
  pwmDutyCycle = 3;			//0-n value for PWM duty cycle based on pwmResolution bits. Default is 3 (range 0-7) or 50%, you shouldn't need to change this typically.
  freqUpdatePeriod = 10.0; 		//Time between step frequency updates during acceleration in microseconds
  micsteps = 16;			//Microstep setting for motor driver
  fullStepsPerRevolution = 200;		//Not implemented yet
  fullStepsPerMillimeter = 25;		//Not implemented yet
  currentPosition_InFullSteps = 0;
  currentSpeed_InFullStepsPerSecond = 0.0;
  desiredSpeed_InFullStepsPerSecond = 200.0;
  acceleration_InFullStepsPerSecondPerSecond = 200.0;
}

void LEDCStepper::connectToPins(byte stepPinNumber, byte dirPinNumber, byte ledChannelNumber)
{
  stepPin = stepPinNumber;
  dirPin = dirPinNumber;
  ledChannel = ledChannelNumber;		//LEDC Channel number
  ledcSetup(ledChannel, 0, pwmResolution);	//Bind to LEDC Channel, set initial frequency to 0, set pwmResolution
  ledcAttachPin(stepPin, ledChannel);
  ledcWrite(ledChannel, pwmDutyCycle);		//Initialize write at 0 Hz, 50% duty cycle
}

void LEDCStepper::setMicsteps(unsigned int micstepsVal)
{
  micsteps = micstepsVal;
}

void LEDCStepper::setFreqUpdatePeriod(float freqUpdatePeriodVal)
{
  freqUpdatePeriod = freqUpdatePeriodVal;
}

void LEDCStepper::setPWMDutyCycle(byte pwmDutyCycleVal)
{
  pwmDutyCycle = pwmDutyCycleVal;
}

void LEDCStepper::accelerate(float desiredSpeedInFullStepsPerSecond, float accelerationInFullStepsPerSecondPerSecond)
{
  desiredSpeed_InFullStepsPerSecond = desiredSpeedInFullStepsPerSecond;
  acceleration_InFullStepsPerSecondPerSecond = accelerationInFullStepsPerSecondPerSecond;
  long currentFrequency = round(currentSpeed_InFullStepsPerSecond * micsteps);
  long targetFrequency = round(desiredSpeed_InFullStepsPerSecond * micsteps);
  float rampTimeInUS = abs(desiredSpeed_InFullStepsPerSecond - currentSpeed_InFullStepsPerSecond) / accelerationInFullStepsPerSecondPerSecond * 1000;
  long freqIntervals = round(rampTimeInUS / freqUpdatePeriod);
  long frequencyIntervalInHz = round((targetFrequency - currentFrequency) / freqIntervals);
  for (int i = 1; i <= freqIntervals; i++) {
    currentFrequency = currentFrequency + frequencyIntervalInHz;
    ledcSetup(ledChannel, currentFrequency, pwmResolution);
    ledcWrite(ledChannel, pwmDutyCycle);
    delay(round(rampTimeInUS / freqIntervals));
  }
  ledcSetup(ledChannel, targetFrequency, pwmResolution);
  ledcWrite(ledChannel, pwmDutyCycle);
  currentSpeed_InFullStepsPerSecond = desiredSpeed_InFullStepsPerSecond;
}

void LEDCStepper::accelerateSerialOut(Stream * SerialPort, float desiredSpeedInFullStepsPerSecond, float accelerationInFullStepsPerSecondPerSecond)
{
  desiredSpeed_InFullStepsPerSecond = desiredSpeedInFullStepsPerSecond;
  acceleration_InFullStepsPerSecondPerSecond = accelerationInFullStepsPerSecondPerSecond;
  long currentFrequency = round(currentSpeed_InFullStepsPerSecond * micsteps);
  long targetFrequency = round(desiredSpeed_InFullStepsPerSecond * micsteps);
  float rampTimeInUS = abs(desiredSpeed_InFullStepsPerSecond - currentSpeed_InFullStepsPerSecond) / accelerationInFullStepsPerSecondPerSecond * 1000;
  long freqIntervals = round(rampTimeInUS / freqUpdatePeriod);
  long frequencyIntervalInHz = round((targetFrequency - currentFrequency) / freqIntervals);
  SerialPort->print("currentFrequency is: ");
  SerialPort->println(currentFrequency);
  SerialPort->print("targetFrequency is: ");
  SerialPort->println(targetFrequency);
  SerialPort->print("freqIntervals is: ");
  SerialPort->println(freqIntervals);
  SerialPort->println("frequency delta is: ");
  SerialPort->println(targetFrequency - currentFrequency);
  SerialPort->println("un-rounded frequency interval is: ");
  SerialPort->println((targetFrequency - currentFrequency) / freqIntervals);
  SerialPort->print("frequencyIntervalInHz is: ");
  SerialPort->println(frequencyIntervalInHz);
  for (int i = 1; i <= freqIntervals; i++) {
    currentFrequency = currentFrequency + frequencyIntervalInHz;
    ledcSetup(ledChannel, currentFrequency, pwmResolution);
    ledcWrite(ledChannel, pwmDutyCycle);
    SerialPort->print("currentFrequency is: ");
    SerialPort->println(currentFrequency);
    delay(round(rampTimeInUS / freqIntervals));
  }
  ledcSetup(ledChannel, targetFrequency, pwmResolution);
  ledcWrite(ledChannel, pwmDutyCycle);
  currentSpeed_InFullStepsPerSecond = desiredSpeed_InFullStepsPerSecond;
}