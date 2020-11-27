/*
      ******************************************************************
      *                                                                *
      *                Header file for LEDCStepper.cpp                 *
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

#ifndef LEDCStepper_h
#define LEDCStepper_h

#include <Arduino.h>
#include <stdlib.h>

class LEDCStepper
{
  public:
    LEDCStepper();
    void setMicsteps(unsigned int micstepsVal);
    void setFreqUpdatePeriod(float freqUpdatePeriodVal);
    void setPWMDutyCycle(byte pwmDutyCycleVal);
    void connectToPins(byte stepPinNumber, byte dirPinNumber, byte ledChannelNumber);
    void accelerate(float desiredSpeedInFullStepsPerSecond, float accelerationInFullStepsPerSecondPerSecond);
    void accelerateSerialOut(Stream * SerialPort, float desiredSpeedInFullStepsPerSecond, float accelerationInFullStepsPerSecondPerSecond);
  private:
    byte stepPin;
    byte dirPin;
    byte ledChannel;
    byte pwmResolution;
    byte pwmDutyCycle;
    float freqUpdatePeriod;
    unsigned int micsteps;
    unsigned int fullStepsPerRevolution;
    unsigned int fullStepsPerMillimeter;
    long currentPosition_InFullSteps;
    float currentSpeed_InFullStepsPerSecond;
    float desiredSpeed_InFullStepsPerSecond;
    float acceleration_InFullStepsPerSecondPerSecond;
};

#endif