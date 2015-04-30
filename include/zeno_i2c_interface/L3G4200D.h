#ifndef L3G4200D_h
#define L3G4200D_h

#include<math.h>
#include<iostream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<zeno_i2c_interface/zeno_i2c_interface.h>

#define _USE_MATH_DEFINES

#define L3G4200D_DEVICE 0x69
#define L3G4200D_BYTES_READ 6

#define L3G4200D_CTRL_REG1 0x20
#define L3G4200D_CTRL_REG2 0x21
#define L3G4200D_CTRL_REG3 0x22
#define L3G4200D_CTRL_REG4 0x23
#define L3G4200D_CTRL_REG5 0x24
#define L3G4200D_FIFO_ACT 0x2E

#define L3G4200D_OUT_X_L 0x28
#define L3G4200D_OUT_X_H 0x29
#define L3G4200D_OUT_Y_L 0x2A
#define L3G4200D_OUT_Y_H 0x2B
#define L3G4200D_OUT_Z_L 0x2C
#define L3G4200D_OUT_Z_H 0x2D

#define ALPHA_G 0.3

#define L3G4200D_BIT_MODE 15  //bits
#define L3G4200D_DATA_SIZE 16 //bits
struct GyroRaw
{
  int x;
  int y;
  int z;
};

struct GyroDPS
{
  double x;
  double y;
  double z;
};

class L3G4200D
{
  public:
    L3G4200D();
    bool init(double xoffset=0, double yoffset=0, double zoffset=0);
    void shutdown();
    void writeTo(char address, char val);
    void readFrom(char address, int num, std::vector<unsigned int> &raw_data);
    GyroRaw readGyro();
    GyroDPS readGyroDPS();
    void printAllRegister();
    void printCalibrationValues(int samples);
    
  private:

     double xg;
     double yg;
     double zg;
     
     double _xoffset;
     double _yoffset;
     double _zoffset;
     ZenoI2CInterface zeno_i2c_interface_;
};

#endif
