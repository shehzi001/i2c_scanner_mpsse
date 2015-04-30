/*
 * zeno_imu.cpp
 *
 * Created on: March 29, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/ADXL345.h"
ADXL345 accel;

void setup()
{   
  accel.init();
  accel.setSoftwareOffset(-0.03795, -0.0521981, 0.0365782); //(-x_calib,-y_calib,1-z_calib)
  //accel.printCalibrationValues(1000);
}

void loop()
{
  //ACCEL
  
  AccelRotation accelRot;
   
  accelRot = accel.readPitchRoll();
 
  //std::cout << "Accelerometer(Roll,Pitch): [" << accelRot.roll << ","<< accelRot.pitch << "]" <<std::endl;
/*  
  AccelRaw raw;
  raw = accel.readAccel();
  std::cout << "accel_raw[x,y,z]: [" << raw.x  << "," << raw.y << ","<< raw.z << "]" << std::endl;
  */ 
  
  AccelG accelG;
  accelG = accel.readAccelG();
  std::cout << "[" << accelG.x << "," << accelG.y << ","<< accelG.z << "," << accelRot.roll << "," << accelRot.pitch << "]" << std::endl; 
 
 //END ACCEL
}

void imu_accelerometer() {
    //std::cout << "IMU accelerometer interface started" << std::endl;
    setup();
    int i=0;
    
   while(i<1000) {
     loop();
     i++;
     usleep(10);
    }
    //std::cout << "IMU accelerometer interface exiting" << std::endl;
}

int main(int argc, char **argv) {
    imu_accelerometer();
    return 0;
}
