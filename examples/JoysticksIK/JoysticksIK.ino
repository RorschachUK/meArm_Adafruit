/* meArm_Adafruit IK joysticks - York Hackspace May 2014
 * Using inverse kinematics with joysticks
 * Uses two analogue joystcks (two pots each)
 * First stick moves gripper forwards, backwards, left and right
 * Second stick moves gripper up, down, and closes and opens.
 * 
 * I used Sparkfun thumbstick breakout boards, oriented 'upside down'.
 * 
 * Pins:
 * Arduino    Stick1    Stick2   PWMServo  
 *    GND       GND       GND        GND
 *     5V       VCC       VCC   VCC & V+
 *     A0       HOR
 *     A1       VER
 *     A2                 HOR
 *     A3                 VER
 *     A4                            SDA
 *     A5                            SCL
 * 
 * The servos attach to the first block of four servo connections on
 * the Adafruit board, brown wire at the bottom, yellow wire at the top.
 * Adafruit    Servo
 *       0      Base
 *       1  Shoulder (right)
 *       2     Elbow (left)
 *       3   Gripper
 *
 * You can attach to a different block of four by changing the 'begin' call
 * to specify a block 0-3, e.g. to use the second block call arm.begin(1);
 * - to mirror movements to all 4 blocks, call arm.begin(-1);
 */
#include "meArm_Adafruit.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

int xdirPin = 0;
int ydirPin = 1;
int zdirPin = 3;
int gdirPin = 2;

meArm arm;

void setup() {
  arm.begin();
}

void loop() {
  float dx = map(analogRead(xdirPin), 0, 1023, -5.0, 5.0);
  float dy = map(analogRead(ydirPin), 0, 1023, 5.0, -5.0);
  float dz = map(analogRead(zdirPin), 0, 1023, 5.0, -5.0);
  float dg = map(analogRead(gdirPin), 0, 1023, 5.0, -5.0);
  if (abs(dx) < 1.5) dx = 0;
  if (abs(dy) < 1.5) dy = 0;
  if (abs(dz) < 1.5) dz = 0;
  
  if (!(dx == 0 && dy == 0 && dz == 0))
    arm.goDirectlyTo(arm.getX() + dx, arm.getY() + dy, arm.getZ() + dz);
  
  if (dg < -3.0)
    arm.closeGripper();
  else if (dg > 3.0)
    arm.openGripper();  
  delay(50);
}
