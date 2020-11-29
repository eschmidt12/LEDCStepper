# LEDCStepper

This is a stepper motor control library for ESP32, using the LEDC frequency generator peripheral. It can be used with any motor controller with a step/direction interface. It is capable of extremely high step rates (~40MHz theoretical) based on the maximum frequency output of the ESP32 LEDC module. This allows for high speed motor control at high microstep rates, while maintaining low processor load compared to bit-banging methods.

Ultimately I'd like this library to be fully capable of position and speed control, but currently it is in proof-of-concept stage and has only rudimentary speed control with acceleration. For accurate position control, this method will require a feedback loop to the PCNT module.

## License
Copyright (c) 2020 E. Schmidt - Licensed under the MIT license.
