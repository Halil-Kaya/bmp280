#include "Bmp280T.h"
#include <Wire.h>
#include <Arduino.h>

//-----------------------------------------------<Bu kisimlar hesaplama yapilan degiskenler>----------------------
//durmadan bu degiskenleri olusturmak yerine burda bir kez olusturuyorum ayrica baglan() fonksiyonunda olusan baglanti bilgileri bu degiskenlerde tutluyor
//boylece bir deger hesaplanirken bu degiskenlerdeki adresleri kullanıyorum
//-------DİKKAT-----<Bunlari anlamaya calisma bu fonksiyonun calismasi icin gerekli olan degiskenler anlamana gerek yok>
// BMP280 I2C address is 0x76(108)
#define Addr 0x76
unsigned int b1[24];
unsigned int data[8];
//temp
unsigned int dig_T1;
int dig_T2;
int dig_T3;
//pressure
// pressure coefficients
  unsigned int dig_P1;
  int dig_P2;
  int dig_P3;
  int dig_P4;
  int dig_P5;
  int dig_P6;
  int dig_P7;
  int dig_P8;
  int dig_P9;
//-----------------------------------------------</Bu kisimlar hesaplama yapilan degiskenler>----------------------


void Bmp280T::baglan(){
  
  for (int i = 0; i < 24; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((136 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 1 byte of data
    if (Wire.available() == 1)
    {
      b1[i] = Wire.read();
    }
  }
  // Convert the data
  // temp coefficients
  dig_T1 = (b1[0] & 0xFF) + ((b1[1] & 0xFF) * 256);
  dig_T2 = b1[2] + (b1[3] * 256);
  dig_T3 = b1[4] + (b1[5] * 256);

  // pressure coefficients
  dig_P1 = (b1[6] & 0xFF) + ((b1[7] & 0xFF) * 256);
  dig_P2 = b1[8] + (b1[9] * 256);
  dig_P3 = b1[10] + (b1[11] * 256);
  dig_P4 = b1[12] + (b1[13] * 256);
  dig_P5 = b1[14] + (b1[15] * 256);
  dig_P6 = b1[16] + (b1[17] * 256);
  dig_P7 = b1[18] + (b1[19] * 256);
  dig_P8 = b1[20] + (b1[21] * 256);
  dig_P9 = b1[22] + (b1[23] * 256);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control measurement register
  Wire.write(0xF4);
  // Normal mode, temp and pressure over sampling rate = 1
  Wire.write(0x27);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select config register
  Wire.write(0xF5);
  // Stand_by time = 1000ms
  Wire.write(0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();

  this->ilkBasincDegeri=getBasinc();
  
}


double Bmp280T::getYukseklik(){
  return ((pow(this->ilkBasincDegeri/this->basinc,1/5.257)-1 )* (this->sicaklik+273.15 ) )/(0.0065);
}


double Bmp280T::getBasinc(){


  for (int i = 0; i < 8; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((247 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 1 byte of data
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }

// Convert pressure and temperature data to 19-bits
  long adc_p = (((long)(data[0] & 0xFF) * 65536) + ((long)(data[1] & 0xFF) * 256) + (long)(data[2] & 0xF0)) / 16;
  long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;

  // Temperature offset calculations
  double var1 = (((double)adc_t) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0) *
                 (((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  double t_fine = (long)(var1 + var2);
  double cTemp = (var1 + var2) / 5120.0;
  double fTemp = cTemp * 1.8 + 32;

  // Pressure offset calculations
  var1 = ((double)t_fine / 2.0) - 64000.0;
  var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
  var2 = var2 + var1 * ((double)dig_P5) * 2.0;
  var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
  var1 = (((double) dig_P3) * var1 * var1 / 524288.0 + ((double) dig_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
  double p = 1048576.0 - (double)adc_p;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double) dig_P9) * p * p / 2147483648.0;
  var2 = p * ((double) dig_P8) / 32768.0;
  double pressure = (p + (var1 + var2 + ((double)dig_P7)) / 16.0) / 100;
  

  this->basinc=pressure;
  return pressure;
}


double Bmp280T::getSicaklik(){

  for (int i = 0; i < 8; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((247 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);

    // Read 1 byte of data
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }
 // Convert pressure and temperature data to 19-bits
  long adc_p = (((long)(data[0] & 0xFF) * 65536) + ((long)(data[1] & 0xFF) * 256) + (long)(data[2] & 0xF0)) / 16;
  long adc_t = (((long)(data[3] & 0xFF) * 65536) + ((long)(data[4] & 0xFF) * 256) + (long)(data[5] & 0xF0)) / 16;

  // Temperature offset calculations
  double var1 = (((double)adc_t) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0) *
                 (((double)adc_t) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  double t_fine = (long)(var1 + var2);
  double cTemp = (var1 + var2) / 5120.0;
  double fTemp = cTemp * 1.8 + 32;

  
  this->sicaklik=cTemp;
  return cTemp;
  
}
