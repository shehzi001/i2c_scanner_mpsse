/*
 * zeno_imu.cpp
 *
 * Created on: March 29, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/ADXL345.h"
#include <zeno_i2c_interface/zeno_i2c_interface.h>

class Accelerometer
{
    public:
        Accelerometer(ZenoI2CInterface &zeno_i2c_interface):
          accel(zeno_i2c_interface)
        {
        }

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
	 
	  AccelRaw raw;
	  raw = accel.readAccel();
	  
	  AccelG accelG;
	  accelG = accel.readAccelG();
	  std::cout << "[" << accelG.x << ", " << accelG.y << ", "<< accelG.z << ", " 
                           << accelRot.roll << ", " << accelRot.pitch << "]" << std::endl; 
	 //END ACCEL
	}

	void imu_accelerometer() {
	    std::cout << "IMU accelerometer interface started" << std::endl;
	    setup();
	    int i=0;
          std::cout << "[" << "accelG_x" << "," << "accelG_y" << ","<< "accelG_z" << ","
                           << "accelRot_roll" << "," << "accelRot_pitch" << "]" << std::endl;	    
	   while(i<10) {
	     loop();
	     i++;
	     usleep(100);
	    }
	    std::cout << "IMU accelerometer interface exiting" << std::endl;
	}
  private:
     ADXL345 accel;
};

int main(int argc, char **argv)
{
   ZenoI2CInterface zeno_i2c_interface;
    if(zeno_i2c_interface.initilizeI2CInterface()) {
      Accelerometer accelerometer(zeno_i2c_interface);
      accelerometer.imu_accelerometer();
      zeno_i2c_interface.closeI2CInterface();
    }

    return 0;
}
