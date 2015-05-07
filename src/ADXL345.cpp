
#include <zeno_i2c_interface/ADXL345.h>
#include<zeno_i2c_interface/zeno_i2c_interface.h>

ADXL345::ADXL345(const ZenoI2CInterface &zeno_i2c_interface):
   zeno_i2c_interface_(zeno_i2c_interface)
  
{  
  xg =0;
  yg = 0;
  zg = 0;
  
}

bool ADXL345::init(char x_offset, char y_offset, char z_offset)
{
  //zeno_i2c_interface = ZenoI2CInterface();
  //if(!zeno_i2c_interface_.initilizeI2CInterface())
  //  return false;

  writeTo(ADXL345_POWER_CTL, 0x08); 
  writeTo(ADXL345_FIFO_CTL, 0x82);  
  writeTo(ADXL345_DATA_FORMAT, 0x0B);
 
  writeTo(ADXL345_OFSX, x_offset);
  writeTo(ADXL345_OFSY, y_offset);
  writeTo(ADXL345_OFSZ, z_offset);
  
 return true;
}


void ADXL345::shutdown()
{
 //zeno_i2c_interface_.closeI2CInterface();  
}

void ADXL345::setSoftwareOffset(double x, double y, double z)
{
  _xoffset = x;
  _yoffset = y;
  _zoffset = z; 
}

AccelRotation ADXL345::readPitchRoll()
{
  //http://developer.nokia.com/community/wiki/How_to_get_pitch_and_roll_from_accelerometer_data_on_Windows_Phone
  //http://www.hobbytronics.co.uk/accelerometer-info
  
  AccelG accel;
  accel = readAccelG();
  
  AccelRotation rot;
  
  rot.pitch = -(atan2(accel.x,sqrt(accel.y*accel.y+accel.z*accel.z)) * 180.0) / M_PI;
  rot.roll = (atan2(accel.y,(sqrt(accel.x*accel.x+accel.z*accel.z))) * 180.0) / M_PI;
  
  return rot;
}


void ADXL345::printCalibrationValues(int samples)
{
  double x,y,z;
  double xt,yt,zt;
  xt = 0;
  yt = 0;
  zt = 0;
  
  std::cout << "Calibration in: 3";
  usleep(1000);
  std::cout << " 2";
  usleep(1000);  
  std::cout << " 1" << std::endl;
  usleep(1000);  
  
  for(int i=0; i<samples; i++)
  {
    AccelG accel = readAccelG();
    xt += accel.x;
    yt += accel.y;
    zt += accel.z;
    usleep(100);
  }
  
  std::cout << "Accel Offset (mg): " << std::endl;
  std::cout << "X: " << xt/float(samples)*1000 << std::endl;
  std::cout << "Y: " << yt/float(samples)*1000 << std::endl;
  std::cout << "Z: " << zt/float(samples)*1000 << std::endl;
  usleep(2000);
}


// Writes val to address register on device
void ADXL345::writeTo(char address, char val) 
{
    zeno_i2c_interface_.writeDevice(ADXL345_DEVICE, address, val);
    usleep(10.0);
}

AccelG ADXL345::readAccelG()
{
  AccelRaw raw;
  raw = readAccel();
  
  //Scale = (16*2)/2^13

  double fXg, fYg, fZg;
  fXg =raw.x * 0.00390625 + _xoffset;
  fYg =raw.y * 0.00390625 + _yoffset;
  fZg =raw.z * 0.00390625 + _zoffset;  
  
  AccelG res;
  
  res.x = fXg * ALPHA + (xg * (1.0-ALPHA));
  xg = res.x;
  
  res.y = fYg * ALPHA + (yg * (1.0-ALPHA));
  yg = res.y;
  
  res.z = fZg * ALPHA + (zg * (1.0-ALPHA));
  zg = res.z;
  
  return res;  
  
}

AccelRaw ADXL345::readAccel() 
{
  std::vector<unsigned int> data;
  bool success;
  AccelRaw raw;

  readFrom(ADXL345_DATAX0, ADXL345_TO_READ, data); //read the acceleration data from the ADXL345

  // each axis reading comes in 16 bit resolution, i.e 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  unsigned int x = data[1];
  x = (x<<8) | data[0];
  unsigned int y = data[3];
  y = (y<<8) | data[2];
  unsigned int z = data[5];
  z = (z<<8) | data[4];

  signed int xx;
  signed int yy;
  signed int zz;
  if(x > pow(2, ADXL345_BIT_MODE))
    xx = x - (pow(2, ADXL345_DATA_SIZE) - 1);
  else
    xx = x;

   if(y > pow(2, ADXL345_BIT_MODE))
      yy = y - (pow(2, ADXL345_DATA_SIZE) - 1);
   else
      yy = y;

   if(z > pow(2, ADXL345_BIT_MODE))
      zz = z - (pow(2, ADXL345_DATA_SIZE) - 1);
   else
      zz = z;

  raw.x = xx;
  raw.y = yy;
  raw.z = zz;

  return raw;
}


void ADXL345::readFrom(char address, int num, std::vector<unsigned int> &raw_data)
{
   bool success;
   success = zeno_i2c_interface_.readDevice(ADXL345_DEVICE, address, raw_data, num);
   //return success;
}

void ADXL345::printAllRegister() 
{
}
