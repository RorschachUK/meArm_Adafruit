/* meArm Wii Classic game pad - York Hackspace May 2014
 * Using inverse kinematics with Wii Classic game controller
 * First stick moves gripper forwards, backwards, left and right
 * Second stick moves gripper up and down (and left and right again).
 * Shoulder buttons or A & B buttons open and close the gripper.
 * The home button returns to starting point.
 * 
 * Uses meArm_Adafruit library from https://github.com/RorschachUK/meArm_Adafruit
 * Uses ClassicController library from https://github.com/wayneandlayne/Video-Game-Shield
 *
 * Connect the Wii Classic gamepad via a breakout such as
 * http://www.phenoptix.com/collections/breakout-board/products/wiichuck-a-wii-nunchuck-break-out-board-by-seeed-studio
 *
 * Pins:
 * Arduino    PWMServo  WiiChuck
 *    GND         GND        -
 *     5V    VCC & V+        +
 *     A4         SDA        d
 *     A5         SCL        c
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
#include <ClassicController.h>

meArm arm;
ClassicController cc;

void setup() {
  arm.begin();
  cc.begin(WII_PLAYER_1);
  cc.joy_left_set_scaled_min_max(0, 99, 0, 99);
  cc.joy_right_set_scaled_min_max(0, 99, 0, 99);
}

void loop() {
  cc.update();

  float dx = 0;
  float dy = 0;
  float dz = 0;
  float dg = 0;
  if (cc.joy_left_left() || cc.joy_right_left())
    dx = -5.0;
  else if (cc.joy_left_right() || cc.joy_right_right())
    dx = 5.0;
  if (cc.joy_left_up())
    dy = 5.0;
  else if (cc.joy_left_down())
    dy = -5.0;
  if (cc.joy_right_up())
    dz = 5.0;
  else if (cc.joy_right_down())
    dz = -5.0;
  if (cc.button_b() || cc.shoulder_left() > 16)
    arm.openGripper();
  else if (cc.button_a() || cc.shoulder_right() > 16)
    arm.closeGripper();
  if (cc.button_home()) {
    arm.gotoPoint(0, 100, 50);
  } 
  if (!(dx == 0 && dy == 0 && dz == 0))
    arm.goDirectlyTo(arm.getX() + dx, arm.getY() + dy, arm.getZ() + dz);
  
  delay(50);
}
