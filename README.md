#Arduino Stepper Motor Speedometer
This code enables you to create a speedometer with a simple arduino and stepper driver circuit.  It is applied in the most basic of implementations.  Say you are hypothetically measuring a vehicle's speed, and you have a magnetic sensor on the wheel.  The speedometer code adds how many times that sensor ticks as the wheel spins, calculates your speed from that value, and maps that to a speedometer dial position!  It does basic (average) smoothing as well.

##Wire it up!
* The type of Arduino board does not matter, nor do you need to insert it into a breadboard.
* Your 'sense' circuit used to detect speed may be different than the one shown below.  The code is defaulted to have the sensing circuit pull the Arduino sense pin down to ground, vs. up to 5V, to trigger a speed/motion detect.
![speedometer-circuit](http://cdaringe.net/img/speedometer4.png)

##Configure & Upload!
* Open speed.ino
* Adjust the #defines to match your speedometer hardware
* Upload
* Go!

##Notes
* A speedometer may be better suited for a servo motor, over a stepper, given the ability to map pulses to a fixed motor angle
* This implementation does not home the dial on startup.  Thus, if there is a power loss during activity, the user must reset the dial to its 0 position
