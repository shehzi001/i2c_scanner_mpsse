
#ifndef ADXL345_H_
#define ADXL345_H_

#include<math.h>
#include<iostream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<zeno_i2c_interface/zeno_i2c_interface.h>

#define _USE_MATH_DEFINES

#define ADXL345_DEVICE 0x53
#define ADXL345_TO_READ 6

#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATAX0 0x32
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_FIFO_CTL 0x38

#define ADXL345_OFSX 0x1E
#define ADXL345_OFSY 0x1F
#define ADXL345_OFSZ 0x20

#define ALPHA 0.5

#define ADXL345_BIT_MODE 15  //bits
#define ADXL345_DATA_SIZE 16 //bits

struct AccelRaw
{
  int x;
  int y;
  int z;
};

struct AccelG
{
  double x;
  double y;
  double z;
};

struct AccelRotation
{
  double pitch;
  double roll;
};

class ADXL345
{
  public:
    ADXL345(ZenoI2CInterface &zeno_i2c_interface);
    bool init(char x_offset=0, char y_offset=0, char z_offset=0);
    void shutdown();
    void writeTo(char address, char val);
    AccelRaw readAccel();
    AccelG readAccelG();
    void readFrom(char address, int num, std::vector<unsigned int> &raw_data);
    void printAllRegister();
    void printCalibrationValues(int samples);
    AccelRotation readPitchRoll();
    void setSoftwareOffset(double x=0, double y=0, double z=0);
    
    
  private: 
     double xg;
     double yg;
     double zg;
     
     double _xoffset;
     double _yoffset;
     double _zoffset;
     ZenoI2CInterface &zeno_i2c_interface_;   
    
};

#endif
