# LEDCStepper

## Summary

This is a stepper motor control library for ESP32, using the LEDC frequency generator peripheral. It can be used with any motor controller with a step/direction interface, and can control multiple stepper objects simultaneously. It is capable of extremely high step rates (~40MHz theoretical) based on the maximum frequency output of the ESP32 LEDC module. This allows for high speed motor control at high microstep rates, while maintaining low processor load compared to bit-banging methods.

Ultimately I'd like this library to be fully capable of position and speed control, but currently it is in proof-of-concept stage and has only rudimentary speed control with acceleration. For accurate position control, this method will require a feedback loop to the PCNT module.

## Documentation

Documentation to come...

## Blocking vs Non-Blocking

The accelerate() functions are blocking functions, but are very simple; plug in your desired speed and acceleration and the function will take care of everything. It will block other functions as it uses delay() so you will not be able to simultaneously control multiple steppers or execute other code during acceleration.

The setupAccelerate() and processAccelerate() functions are non blocking but require some additional sketch logic. To set up an acceleration, call the setupAccelerate() function with your desired speed and acceleration. Then, call processAccelerate() at least once per freqUpdatePeriod until the acceleration is complete. You can monitor the state of the acceleration using motionComplete().

Using the non-blocking functions will allow you to control multiple stepper objects simultaneously.

## License
Copyright (c) 2020 E. Schmidt - Licensed under the MIT license.
