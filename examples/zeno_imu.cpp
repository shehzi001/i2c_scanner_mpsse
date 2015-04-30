/*
 * zeno_imu.cpp
 *
 * Created on: March 29, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/ADXL345.h"
#include "zeno_i2c_interface/L3G4200D.h"
#include <vector>
ADXL345 accel;
L3G4200D gyro;
bool readGyro;
double xi,yi,zi;
clock_t pT;
std::vector<double> roll;
std::vector<double> pitch;
std::vector<double> yaw;

void setup()
{   
  accel.init(-1, 0, 8);
  accel.setSoftwareOffset(-0.023, 0, 0.03577027);//-0.0198297, -0.0921918, -0.468992);//-0.0196, -0.0912071, -0.465631);
 // accel.printCalibrationValues(100);
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
    }
    //std::cout << "IMU accelerometer interface exiting" << std::endl;
}


void setup_gyro()
{
 gyro.init(-1.23977124535, 0.364847, 0.60590615);//-1.22858, -0.383618, -0.575575);//(-0.18891-0.04382, -0.09454-0.05530, -0.31720-0.04951); //Don’t  use my ca
 xi = yi = zi = 0;
 readGyro = false;
 roll.resize(3);
 pitch.resize(3);
 yaw.resize(3); 
}

void loop_gyro()
{
    readGyro = false;
    
    clock_t cT = clock(); 

    unsigned long dT = cT - pT;

    //std::cout << "time diff: " << ((double)dT/CLOCKS_PER_SEC) <<std::endl;
    /*GyroRaw raw;
    raw = gyro.readGyro();   
    std::cout << "[" << raw.x << "," << raw.y << ","<< raw.z << "]" << std::endl;
    */
    GyroDPS gDPS;
    gDPS = gyro.readGyroDPS();
     
    pT = cT;
    
    double roll_c = gDPS.x*((double)dT/CLOCKS_PER_SEC);
    double pitch_c =  gDPS.y*((double)dT/CLOCKS_PER_SEC);
    double yaw_c = gDPS.z*((double)dT/CLOCKS_PER_SEC);
    
    // simple integrator
    xi = xi + gDPS.x*((double)dT/CLOCKS_PER_SEC);
    yi = yi + gDPS.y*((double)dT/CLOCKS_PER_SEC); 
    zi = zi + gDPS.z*((double)dT/CLOCKS_PER_SEC);
    
    /*
    //runge-kutta integration 
    xi = xi + (1/6)*(roll[2]+(2*roll[1])+(2*roll[1])+roll_c);
    yi = yi + (1/6)*(pitch[2]+(2*pitch[1])+(2*pitch[1])+pitch_c); 
    zi = zi + (1/6)*(yaw[2]+(2*yaw[1])+(2*yaw[1])+yaw_c);
    roll[2] = roll[1];
    roll[1] = roll[0];
    roll[0] = roll_c;

    pitch[2] = pitch[1];
    pitch[1] = pitch[0];
    pitch[0] = pitch_c;

    yaw[2] = yaw[1];
    yaw[1] = yaw[0];
    yaw[0] = yaw_c;
    */
    std::cout << "[" << gDPS.x << "," << gDPS.y << ","<< gDPS.z << "," << xi << "," << yi << "," << zi << "]" << std::endl; 
   
    //END GYRO
}

void sleepcp(int milliseconds) // cross-platform sleep function
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }
}
void imu_gyro() {
    //std::cout << "IMU gyro interface started" << std::endl;
    setup_gyro();
    int i=0; 
    pT = clock();
    //gyro.printCalibrationValues(1000);
   
    while(i<1000) {
     loop_gyro();
     i++;
     sleepcp(10);
    }
   
    //std::cout << "IMU gyro interface exiting" << std::endl;
}

int main(int argc, char **argv) {
    imu_accelerometer();
    //imu_gyro();
    return 0;
}
