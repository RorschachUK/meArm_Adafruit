meArm_Adafruit
==============

Inverse Kinematics motion control library for Phenoptix meArm on an Arduino with an Adafruit 16-channel PWM Servo driver board.

The meArm has four mini servos - one for the gripper, and one each to rotate the base, shoulder joint and elbow joint.  But it's not terribly convenient to be specifying things in terms of servo angles when you're much more interested in where you would like to place the gripper, in normal Cartesian (x, y, z) coordinates.

This library solves the angles required to send to the servos in order to meet a given position, allowing for much simpler coding.

Coordinates are measured in mm from the base rotation centre.  Initial 'home' position is at (0, 100, 50), i.e. 100mm forward of the base and 50mm off the ground.

For a version of this library which drives the servos directly without the Adafruit board, try: https://github.com/yorkhackspace/meArm

Usage
-----

```
#include "meArm_Adafruit.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

meArm arm;

void setup() {
  arm.begin();
  arm.openGripper();
}

void loop() {
  //Go up and left to grab something
  arm.gotoPoint(-80,100,140); 
  arm.closeGripper();
  //Go down, forward and right to drop it
  arm.gotoPoint(70,200,10);
  arm.openGripper();
  //Back to start position
  arm.gotoPoint(0,100,50);
}
```

Three usage examples are included:
* IKTest follows a pre-programmed path defined in Cartesian coordinates
* JoystickIK uses two analogue thumb sticks to guide the gripper in Cartesian space
* meArm_Wii_Classic uses a Wii Classic gamepad connected over I2C to guide the gripper

Installation
------------
* Clone this repository to your Arduino libraries folder
* You will also need Adafruit's PWM Servo driver library from here: https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

Class methods of meArm object
-----------------------------
* void begin(int pinBlock = 0, int address = 0x40) - Optionally specify which block of four pins on the Adafruit board to use (base, shoulder, elbow and gripper), and which I2C address to use.  Block -1 mirrors to all four blocks.  Begin must be called in setup() before any other calls to the meArm instance are made.
* void openGripper() - opens the gripper, letting go of anything it was holding
* void closeGripper() - closes the gripper, perhaps grabbing and holding something as it does so
* void gotoPoint(float x, float y, float z) - move in a straight line from the current point to the requested position
* void goDirectlyTo(float x, float y, float z) - set the servo angles to immediately go to the requested point without caring what path the arm swings through to get there - faster but less predictable than gotoPoint
* bool isReachable() - returns true if the point can theoretically be reached by the arm
* float getX() - current x coordinate
* float getY() - current y coordinate
* float getZ() - current z coordinate
