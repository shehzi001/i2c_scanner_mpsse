/*
 * zeno_imu_gyroscope.cpp
 *
 * Created on: March 29, 2015
 * Author: Shehzad
 */
#include <iostream>
#include <time.h>
#include "zeno_i2c_interface/L3G4200D.h"
#include <vector>
#include <zeno_i2c_interface/zeno_i2c_interface.h>

class Gyroscope
{
    public:
	Gyroscope(ZenoI2CInterface &zeno_i2c_interface):
		  gyro(zeno_i2c_interface)
	{
	}

	void setup_gyro()
	{
	 gyro.init(-1.3, 0.40, 0.669669);
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
	    //std::cout << "time diff: " << ((double)dT/CLOCKS_PER_SEC) <<std::endl;
	    /*GyroRaw raw;
	    raw = gyro.readGyro();   
	    std::cout << "[" << raw.x << "," << raw.y << ","<< raw.z << "]" << std::endl;
	    */
	    GyroDPS gDPS;
	    gDPS = gyro.readGyroDPS();
	     
	    unsigned long dT = cT - pT;
	    pT = cT;
	    
	    double roll_c = gDPS.x*((double)dT/CLOCKS_PER_SEC);
	    double pitch_c =  gDPS.y*((double)dT/CLOCKS_PER_SEC);
	    double yaw_c = gDPS.z*((double)dT/CLOCKS_PER_SEC);
	    
	    // simple integrator
	    xi = xi + gDPS.x*0.03;//((double)dT/CLOCKS_PER_SEC);
	    yi = yi + gDPS.y*0.03;//((double)dT/CLOCKS_PER_SEC); 
	    zi = zi + gDPS.z*0.03;//((double)dT/CLOCKS_PER_SEC);
	    
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
	    std::cout << "[" << gDPS.x << "," << gDPS.y << ","<< gDPS.z << "," 
                             << xi << "," << yi << "," << zi << "]" << std::endl; 
	   
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

	void imu_gyro() 
        {
	    std::cout << "IMU gyro interface started" << std::endl;
	    setup_gyro();
	    int i=0; 
	    pT = clock();
	  // gyro.printCalibrationValues(1000);
	   
	    std::cout << "[" << "gDPS_x" << "," << "gDPS_y" << ","<< "gDPS_z" << "," 
                             << "roll" << "," << "pitch" << "," << "yaw" << "]" << std::endl; 
	    while(i<10) {
	     loop_gyro();
	     i++;
	     sleepcp(10);
	    }
	   
	    std::cout << "IMU gyro interface exiting" << std::endl;
	}

    private: 

	L3G4200D gyro;
	bool readGyro;
	double xi,yi,zi;
	clock_t pT;
	std::vector<double> roll;
	std::vector<double> pitch;
	std::vector<double> yaw;
       
};

int main(int argc, char **argv)
{
   ZenoI2CInterface zeno_i2c_interface;
    if(zeno_i2c_interface.initilizeI2CInterface()) {
      Gyroscope gyroscope(zeno_i2c_interface);
      gyroscope.imu_gyro();
      zeno_i2c_interface.closeI2CInterface();
    }

    return 0;
}
