#Arduino Stepper Speedometer

##Wire it up!
* The type of Arduino board does not matter, nor do you need to insert it into a breadboard.
* Your 'sense' circuit used to detect speed may be different than the one shown below.  The code is defaulted to have the sensing circuit pull the Arduino sense pin down to ground, vs. up to 5V, to trigger a speed/motion detect.
![speedometer-circuit](http://cdaringe.net/img/speedometer_.png)

##Configure & Upload!
* Open speed.ino
* Adjust the #defines to match your speedometer hardware
* Upload
* Go!

##Notes
* A speedometer may be better suited for a servo motor, over a stepper, given the ability to map pulses to a fixed motor angle
* This implementation does not home the dial on startup.  Thus, if there is a power loss during activity, the user must reset the dial to its 0 position