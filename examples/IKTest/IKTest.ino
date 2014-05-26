/* meArm_Adafruit IK test - York Hackspace May 2014
 * Test applying Nick Moriarty's Inverse Kinematics solver
 * to Phenoptix' meArm robot arm, to walk a specified path.
 *
 * Pins:
 * Arduino    PWMServo
 *    GND         GND
 *     5V    VCC & V+
 *     A4         SDA
 *     A5         SCL
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

meArm arm;

void setup() {
  Serial.begin(9600);
  arm.begin();
}

void loop() {
  //Clap - so it's obvious we're at this part of the routine
  arm.openGripper();
  arm.closeGripper();
  arm.openGripper();
  arm.closeGripper();
  arm.openGripper();
  delay(500);
  //Go up and left to grab something
  arm.gotoPoint(-80,100,140); 
  arm.closeGripper();
  //Go down, forward and right to drop it
  arm.gotoPoint(70,200,10);
  arm.openGripper();
  //Back to start position
  arm.gotoPoint(0,100,50);
  delay(2000);
}
