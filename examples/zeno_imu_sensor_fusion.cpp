/*
 * zeno_imu_sensor_fusion.cpp
 *
 * Created on: March 30, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/ADXL345.h"
#include "zeno_i2c_interface/L3G4200D.h"
#include<zeno_i2c_interface/zeno_i2c_interface.h>


class IMUIntegration
{
  public:
      IMUIntegration(const ZenoI2CInterface &zeno_i2c_interface):
        accel_(zeno_i2c_interface), gyro_(zeno_i2c_interface)
      {
      }

      bool init()
      {
        accel_.init();
        accel_.setSoftwareOffset(-0.03795, -0.0521981, 0.0365782); //(-x_calib,-y_calib,1-z_calib)
        //accel.printCalibrationValues(1000);

        xi = yi = zi = 0;
        roll.resize(3);
        pitch.resize(3);
        yaw.resize(3);
        gyro_.init(-1.3, 0.40, 0.669669);//-1.22858, -0.383618, -0.575575);//(-0.18891-0.04382, -0.09454-0.05530, -0.31720-0.04951); //Don’t  use my ca
        //gyro.printCalibrationValues(1000);

        angleF_roll = 0.0;
        angleF_pitch = 0.0;
      }

      void executeCycle()
      {
        //std::cout << "IMU accelerometer interface started" << std::endl;
        init();
        int i=0;
        pT = clock();
        while(i<1000) {
              loop();
              i++;
              sleepcp(10);
        }
         //std::cout << "IMU accelerometer interface exiting" << std::endl;
      }
 
      void loop()
      {
        //ACCEL
        AccelRotation accelRot;
   
        accelRot = accel_.readPitchRoll();
        /*
        AccelG accelG;
        accelG = accel_.readAccelG();
        std::cout << "[" << accelG.x << "," << accelG.y << ","<< accelG.z << "," 
                    << accelRot.roll << "," << accelRot.pitch << "]" << std::endl;
        */
        //END ACCEL

        //GYRO
        clock_t cT = clock();

        GyroDPS gDPS;
        gDPS = gyro_.readGyroDPS();
         
        unsigned long dT = cT - pT;
        pT = cT;

        angleF_roll = 0.95*(angleF_roll + gDPS.x*0.03) +0.05*accelRot.roll;
        angleF_pitch = 0.95*(angleF_pitch - gDPS.y*0.03) +0.05*accelRot.pitch;
        
        /*
        double roll_c = gDPS.x*((double)dT/CLOCKS_PER_SEC);
        double pitch_c =  gDPS.y*((double)dT/CLOCKS_PER_SEC);
        double yaw_c = gDPS.z*((double)dT/CLOCKS_PER_SEC);


        
        // simple integrator
        xi = xi + gDPS.x*0.03;//((double)dT/CLOCKS_PER_SEC);
        yi = yi + gDPS.y*0.03;//((double)dT/CLOCKS_PER_SEC); 
        zi = zi + gDPS.z*0.03;//((double)dT/CLOCKS_PER_SEC);
        
        
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
        
        std::cout << "[" << gDPS.x << "," << gDPS.y << ","<< gDPS.z << "," 
                      << xi << "," << yi << "," << zi << "]" << std::endl; 
        */
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
        L3G4200D gyro_;
        double xi,yi,zi;
        clock_t pT;
        std::vector<double> roll;
        std::vector<double> pitch;
        std::vector<double> yaw;

        double angleF_roll;
        double angleF_pitch;
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
