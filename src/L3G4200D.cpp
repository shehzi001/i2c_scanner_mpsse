
#include <zeno_i2c_interface/L3G4200D.h>
#include <time.h>
L3G4200D::L3G4200D(ZenoI2CInterface &zeno_i2c_interface):
    zeno_i2c_interface_(zeno_i2c_interface)
{
  xg = yg = zg = 0;
}

bool L3G4200D::init(double xoffset, double yoffset, double zoffset)
{
  //if(!zeno_i2c_interface_.initilizeI2CInterface())
  //  return false;
  _xoffset = xoffset;
  _yoffset = yoffset;
  _zoffset = zoffset;
 
  writeTo(L3G4200D_CTRL_REG1, 0x0F);
  writeTo(L3G4200D_CTRL_REG3, 0x08); //Dont override values
  writeTo(L3G4200D_CTRL_REG4, 0x80); 
  writeTo(L3G4200D_CTRL_REG5, 0x40);
  writeTo(L3G4200D_FIFO_ACT, 0x40);
 /*
  writeTo(L3G4200D_CTRL_REG1, 0x0F);
  writeTo(L3G4200D_CTRL_REG4, 0x80); //Dont override values
  writeTo(L3G4200D_CTRL_REG5, 0x80);
*/
  return true;
}

void L3G4200D::shutdown()
{
 //zeno_i2c_interface_.closeI2CInterface();
}


void L3G4200D::printCalibrationValues(int samples)
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
    GyroDPS dps = readGyroDPS();
    xt += dps.x;
    yt += dps.y;
    zt += dps.z;
    //usleep(100);
    clock_t time_end;
    time_end = clock() + (10) * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
    }

  }
  
  std::cout << "Gyroscope Offset (dps): " << std::endl;
  std::cout << "X: " << xt/float(samples) << std::endl;
  std::cout << "Y: " << yt/float(samples) << std::endl;
  std::cout << "Z: " << zt/float(samples) << std::endl;
  usleep(2000);

}

GyroDPS L3G4200D::readGyroDPS()
{
  //deafult 250dps = 8.75 mdps/digit
  
  GyroRaw raw;
  raw = readGyro();
  
  double fXg, fYg, fZg;
  
  fXg = raw.x * 0.00875 + _xoffset;
  fYg = raw.y * 0.00875 + _yoffset;
  fZg = raw.z * 0.00875 + _zoffset;

  GyroDPS dps;
  
  dps.x = fXg * ALPHA_G + (xg * (1.0-ALPHA_G));
  xg = dps.x;
  
  dps.y = fYg * ALPHA_G + (yg * (1.0-ALPHA_G));
  yg = dps.y;
  
  dps.z = fZg * ALPHA_G + (zg * (1.0-ALPHA_G));
  zg = dps.z;
  //std::cout << "[" << raw.x << "," << raw.y << ","<< raw.z << "," << dps.x << "," << dps.y << ","<< dps.z << "]" << std::endl;
  return dps; 
}

GyroRaw L3G4200D::readGyro() 
{
  std::vector<unsigned int> data;
  
  readFrom(L3G4200D_OUT_X_L | 0x80, L3G4200D_BYTES_READ, data); //0x80 enable auto increment (who would have known?)

  // each axis reading comes in 16 bit resolution, ie 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  GyroRaw raw;
  unsigned int x = data[1];
  int16_t  x_t = (x<<8) | (data[0]&0xFC); 
  
  unsigned int y = data[3];
  int16_t y_t = (y<<8) | (data[2]&0xFC);

  unsigned int z = data[5];
  int16_t z_t = (z<<8) | (data[4]&0XFC);
  
  raw.x = x_t;
  raw.y = y_t;
  raw.z = z_t;
   
  //std::cout << "[" << raw.x << "," << raw.y << ","<< raw.z << "]" << std::endl;
  return raw;
}

// Writes val to address register on device
void L3G4200D::writeTo(char address, char val) 
{
  zeno_i2c_interface_.writeDevice(L3G4200D_DEVICE, address, val);
  usleep(10.0);
}

// Reads num bytes starting from address register on device in to _buff array
void L3G4200D::readFrom(char address, int num, std::vector<unsigned int> &raw_data)
{
   bool success;
   success = zeno_i2c_interface_.readDevice(L3G4200D_DEVICE, address, raw_data, num);
}

void L3G4200D::printAllRegister() 
{
}
