/* meArm library York Hack Space May 2014
 * A simple control library for Phenoptix' meArm
 * Usage:
 *   meArm arm;
 *   arm.begin(1, 10, 9, 6);
 *   arm.openGripper();
 *   arm.gotoPoint(-80, 100, 140);
 *   arm.closeGripper();
 *   arm.gotoPoint(70, 200, 10);
 *   arm.openGripper();
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>
#include "ik.h"
#include "meArm_Adafruit.h"

bool setup_servo (ServoInfo& svo, const int n_min, const int n_max,
                  const float a_min, const float a_max)
{
    float n_range = n_max - n_min;
    float a_range = a_max - a_min;

    // Must have a non-zero angle range
    if(a_range == 0) return false;

    // Calculate gain and zero
    svo.gain = n_range / a_range;
    svo.zero = n_min - svo.gain * a_min;

    // Set limits
    svo.n_min = n_min;
    svo.n_max = n_max;

    return true;
}

int angle2pwm (const ServoInfo& svo, const float angle)
{
  int pwm = 150 + int(0.5f + 450.0 * (svo.zero + svo.gain * angle) / 180.0);
  return pwm;
}

//Full constructor with calibration data
meArm::meArm(int sweepMinBase, int sweepMaxBase, float angleMinBase, float angleMaxBase,
          int sweepMinShoulder, int sweepMaxShoulder, float angleMinShoulder, float angleMaxShoulder,
          int sweepMinElbow, int sweepMaxElbow, float angleMinElbow, float angleMaxElbow,
          int sweepMinGripper, int sweepMaxGripper, float angleMinGripper, float angleMaxGripper) {
  //calroutine();
  setup_servo(_svoBase, sweepMinBase, sweepMaxBase, angleMinBase, angleMaxBase);
  setup_servo(_svoShoulder, sweepMinShoulder, sweepMaxShoulder, angleMinShoulder, angleMaxShoulder);
  setup_servo(_svoElbow, sweepMinElbow, sweepMaxElbow, angleMinElbow, angleMaxElbow);
  setup_servo(_svoGripper, sweepMinGripper, sweepMaxGripper, angleMinGripper, angleMaxGripper);
}

void meArm::begin(int pinBlock, int address) {
  _pinBlock = pinBlock;
  _pwm = Adafruit_PWMServoDriver(address);
  _pwm.begin();
  _pwm.setPWMFreq(60);

  goDirectlyTo(0, 100, 50);
  openGripper();
}

//Set servos to reach a certain point directly without caring how we get there 
void meArm::goDirectlyTo(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {
    if (_pinBlock < 0) {
      for (int i = 0; i < 4; i++) {
        _pwm.setPWM(i * 4, 0, angle2pwm(_svoBase,radBase));
        _pwm.setPWM(i * 4 + 1, 0, angle2pwm(_svoShoulder,radShoulder));
        _pwm.setPWM(i * 4 + 2, 0, angle2pwm(_svoElbow,radElbow));
      }
    } else {
      _pwm.setPWM(_pinBlock * 4, 0, angle2pwm(_svoBase,radBase));
      _pwm.setPWM(_pinBlock * 4 + 1, 0, angle2pwm(_svoShoulder,radShoulder));
      _pwm.setPWM(_pinBlock * 4 + 2, 0, angle2pwm(_svoElbow,radElbow));
    }
    _x = x; _y = y; _z = z;
  }    
}

//Travel smoothly from current point to another point
void meArm::gotoPoint(float x, float y, float z) {
  //Starting points - current pos
  float x0 = _x; 
  float y0 = _y; 
  float z0 = _z;
  float dist = sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y)+(z0-z)*(z0-z));
  int step = 10;
  for (int i = 0; i<dist; i+= step) {
    goDirectlyTo(x0 + (x-x0)*i/dist, y0 + (y-y0) * i/dist, z0 + (z-z0) * i/dist);
    delay(50);
  }
  goDirectlyTo(x, y, z);
  delay(50);
}

//Check to see if possible
bool meArm::isReachable(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  return (solve(x, y, z, radBase, radShoulder, radElbow));
}

//Grab something
void meArm::openGripper() {
    if (_pinBlock < 0) {
      for (int i = 0; i < 4; i++) {
        _pwm.setPWM(i * 4 + 3, 0, angle2pwm(_svoGripper,PI/4.0));
      }
    } else {
      _pwm.setPWM(_pinBlock * 4 + 3, 0, angle2pwm(_svoGripper,PI/4.0));
    }
  delay(300);
}

//Let go of something
void meArm::closeGripper() {
    if (_pinBlock < 0) {
      for (int i = 0; i < 4; i++) {
        _pwm.setPWM(i * 4 + 3, 0, angle2pwm(_svoGripper,-PI/4.0));
      }
    } else {
      _pwm.setPWM(_pinBlock * 4 + 3, 0, angle2pwm(_svoGripper,-PI/4.0));
    }
  delay(300);
}

//Current x, y and z
float meArm::getX() {
  return _x;
}
float meArm::getY() {
  return _y;
}
float meArm::getZ() {
  return _z;
}

