// TienAnh.Info
// 22/02/2014//

// Ham viet gia tri xuong giao tiep I2C
void Write2Mpu6050(unsigned int8 RegAddr,unsigned int8 data)
{
   i2c_start();    // Start condition
   i2c_write(MPU6050_ADDRESS_WRITE);// Device address write
   i2c_write(RegAddr);// Regiter address
   i2c_write(data);// data send
   i2c_stop();     // Stop condition
}

// Ham doc 1 byte tu 1 thanh ghi cua MPU6050
unsigned int8 ReadMpu6050(unsigned int8 RegAddr)
{
   unsigned int8 value;
   i2c_start();
   i2c_write(MPU6050_ADDRESS_WRITE);
   i2c_write(RegAddr);
   i2c_start();
   i2c_write(MPU6050_ADDRESS_READ);// Device address read   
   value = i2c_read(0);   
   i2c_stop();   
   return value;
 }

// Mpu6050 xuat ra gia tri 16bit
signed int16 GetdataMpu6050(unsigned int8 RegAddr)
{
 unsigned int8 LSB;
 unsigned int8 MSB;
 MSB=ReadMpu6050(RegAddr);
 LSB=ReadMpu6050(RegAddr+1); 
 return (make16(MSB,LSB));
 }
 
// Ham khoi tao gia tri ban dau cho Mpu6050
void InitMpu6050()
{
   Write2Mpu6050(MPU6050_RA_PWR_MGMT_1, 0x01);
   Write2Mpu6050(MPU6050_RA_GYRO_CONFIG, 0x00); //full scale range mode 0 +-250do/s   
   Write2Mpu6050(MPU6050_RA_ACCEL_CONFIG, 0x00); //full scale range mode 0 +-2g
   //delay_ms(30);
   Write2Mpu6050(MPU6050_RA_PWR_MGMT_1, 0x08);// internal 8MHz, disabled SLEEP mode, disable CYCLE mode   
   //Sets sample rate to 8000/(1+7) = 1000Hz
   Write2Mpu6050(MPU6050_RA_SMPLRT_DIV, 0x07);
   //Disable FSync, 256Hz DLPF
   Write2Mpu6050(MPU6050_RA_CONFIG, 0x06); //DLPF disable 0x06
   //Disable sensor output to FIFO buffer
   Write2Mpu6050(MPU6050_RA_FIFO_EN, 0x00);
}

// Self test MPU6050
void SelftestMpu6050()
{
   Write2Mpu6050(MPU6050_RA_PWR_MGMT_1, 0x00); // internal 8MHz, disabled SLEEP mode, disable CYCLE mode
   Write2Mpu6050(MPU6050_RA_SMPLRT_DIV, 0x07); //sample rate: 8khz
   Write2Mpu6050(MPU6050_RA_CONFIG, 0x06); //DLPF disable
   Write2Mpu6050(MPU6050_RA_GYRO_CONFIG, 0xe0); //Enable Selftest Gyroscope
   Write2Mpu6050(MPU6050_RA_ACCEL_CONFIG, 0xf0); //Enable Selftest Acceleromenter
   delay_ms(5);
   unsigned int8 xg,yg,zg;
   char temp[16];
   xg=ReadMpu6050(MPU6050_RA_SELF_TEST_X);
   yg=ReadMpu6050(MPU6050_RA_SELF_TEST_Y);
   zg=ReadMpu6050(MPU6050_RA_SELF_TEST_Z);
   delay_ms(5);
   sprintf(temp,"%ux%uy%uz\r\n",xg,yg,zg);
   fprintf(com1,temp);// Truyen du lieu len PC   
}

// Chuong tinh tinh goc quay trung binh cam bien mpu650
float32 get_angle()
{
   float32 angle=0;
   int i;
   signed int16 y_accel,z_accel;
   for(i=0;i<5;i++)
   {
      y_accel = GetdataMpu6050(MPU6050_RA_ACCEL_YOUT_H)-MPU6050_OFFSET_ACCEL_Y;
      z_accel = GetdataMpu6050(MPU6050_RA_ACCEL_ZOUT_H)-MPU6050_OFFSET_ACCEL_Z;
     angle+=atan2(y_accel,z_accel)* 57.296;
   }
   return(angle/5.0);
}
// Chuong trinh test MPU6050
void TestMpu6050()
{
      signed int16 xg,yg,zg;
      xg=GetdataMpu6050(MPU6050_RA_ACCEL_XOUT_H);
      yg=GetdataMpu6050(MPU6050_RA_ACCEL_YOUT_H);
      zg=GetdataMpu6050(MPU6050_RA_ACCEL_ZOUT_H);
//!      sprintf(value,"%ldx%ldy%ldz\r\n",xg,yg,zg);
//!      fprintf(com1,value);// Truyen du lieu len PC  
}


