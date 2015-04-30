/*
 * zeno_imu.cpp
 *
 * Created on: March 29, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/ADXL345.h"
#include <boost/shared_ptr.hpp>
#include<zeno_i2c_interface/zeno_i2c_interface.h>
//ADXL345 accel;


class IMUIntegration
{
  public:
      IMUIntegration(const ZenoI2CInterface &zeno_i2c_interface):
        accel_(zeno_i2c_interface)
      {
      }

     bool init()
      {
        accel_.init();
        accel_.setSoftwareOffset(-0.03795, -0.0521981, 0.0365782); //(-x_calib,-y_calib,1-z_calib)
        //accel.printCalibrationValues(1000);
      }

     void executeCycle()
      {
        //std::cout << "IMU accelerometer interface started" << std::endl;
        init();
        int i=0; 
        while(i<1000) {
               loop();
               i++;
               usleep(10);
        }
         //std::cout << "IMU accelerometer interface exiting" << std::endl;

      }
 
     void loop()
      {
        //ACCEL
  
        AccelRotation accelRot;
   
        accelRot = accel_.readPitchRoll();
 
        //std::cout << "Accelerometer(Roll,Pitch): [" << accelRot.roll << ","<< accelRot.pitch << "]" <<std::endl;
  
        AccelG accelG;
        accelG = accel_.readAccelG();
        std::cout << "[" << accelG.x << "," << accelG.y << ","<< accelG.z << "," << accelRot.roll << "," << accelRot.pitch << "]" << std::endl; 
 
       //END ACCEL
}

  private:
        /**
         * Copy Ctor.
         */
        IMUIntegration(const IMUIntegration &other);

        /**
         * Assignment operator
         */
        IMUIntegration &operator=(const IMUIntegration &other);

        ADXL345 accel_;
      
      //boost::shared_ptr<ADXL345> accel_;
};

int main(int argc, char **argv) 
{
    ZenoI2CInterface zeno_i2c_interface;
    if(zeno_i2c_interface.initilizeI2CInterface()) {
      IMUIntegration imu_intgration(zeno_i2c_interface);
      imu_intgration.executeCycle();
      zeno_i2c_interface.closeI2CInterface();
    }
    return 0;
}
