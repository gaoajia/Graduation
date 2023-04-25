#include "VL6180X.h"

VL6180x::VL6180x(uint8_t address)
// 初始化库
{
  Wire.begin(); // Arduino Wire 库初始化器
  _i2caddress = address; //设置默认通讯地址
}


uint8_t VL6180x::VL6180xInit(void){
  uint8_t data; //用于临时数据存储

  data = VL6180x_getRegister(VL6180X_SYSTEM_FRESH_OUT_OF_RESET);

  if(data != 1) return VL6180x_FAILURE_RESET;

  //数据表要求； 仅从数据表中获取的推荐设置 
  //http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  VL6180x_setRegister(0x0207, 0x01);
  VL6180x_setRegister(0x0208, 0x01);
  VL6180x_setRegister(0x0096, 0x00);
  VL6180x_setRegister(0x0097, 0xfd);
  VL6180x_setRegister(0x00e3, 0x00);
  VL6180x_setRegister(0x00e4, 0x04);
  VL6180x_setRegister(0x00e5, 0x02);
  VL6180x_setRegister(0x00e6, 0x01);
  VL6180x_setRegister(0x00e7, 0x03);
  VL6180x_setRegister(0x00f5, 0x02);
  VL6180x_setRegister(0x00d9, 0x05);
  VL6180x_setRegister(0x00db, 0xce);
  VL6180x_setRegister(0x00dc, 0x03);
  VL6180x_setRegister(0x00dd, 0xf8);
  VL6180x_setRegister(0x009f, 0x00);
  VL6180x_setRegister(0x00a3, 0x3c);
  VL6180x_setRegister(0x00b7, 0x00);
  VL6180x_setRegister(0x00bb, 0x3c);
  VL6180x_setRegister(0x00b2, 0x09);
  VL6180x_setRegister(0x00ca, 0x09);  
  VL6180x_setRegister(0x0198, 0x01);
  VL6180x_setRegister(0x01b0, 0x17);
  VL6180x_setRegister(0x01ad, 0x00);
  VL6180x_setRegister(0x00ff, 0x05);
  VL6180x_setRegister(0x0100, 0x05);
  VL6180x_setRegister(0x0199, 0x05);
  VL6180x_setRegister(0x01a6, 0x1b);
  VL6180x_setRegister(0x01ac, 0x3e);
  VL6180x_setRegister(0x01a7, 0x1f);
  VL6180x_setRegister(0x0030, 0x00);

  return 0;
}

void VL6180x::VL6180xDefautSettings(void){
  //数据表中的推荐设置

  VL6180x_setRegister(VL6180X_SYSTEM_MODE_GPIO1, 0x10); // 采样完成后将 GPIO1 设置为高电平
  VL6180x_setRegister(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30); // 设置平均采样周期 // 数据表中推荐的 48 个样本 --> 4.3 ms
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46); //设置ALS增益 // 按照数据表中的建议
  VL6180x_setRegister(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF); // 设置自动校准周期（最大值 = 255）/（关闭 = 0）
  VL6180x_setRegister(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63); // 将 ALS 积分时间设置为 100ms
  VL6180x_setRegister(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01); // 执行单次温度校准
  //数据表中的可选设置
  //http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  VL6180x_setRegister(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // 将默认测距间隔测量周期设置为 100ms
  VL6180x_setRegister(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A); // 将默认的 ALS 测量间隔设置为 100ms
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x00); // 配置“新样品就绪阈值事件”中断 
  //来自社区的其他默认设置
  VL6180x_setRegister(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);
  VL6180x_setRegister(VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);
  VL6180x_setRegister16bit(VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B );
  VL6180x_setRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);

  VL6180x_setRegister(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD,0x30);
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN,0x40);
  VL6180x_setRegister(VL6180X_FIRMWARE_RESULT_SCALER,0x01);
}
void VL6180x::getIdentification(struct VL6180xIdentification *temp){

  temp->idModel =  VL6180x_getRegister(VL6180X_IDENTIFICATION_MODEL_ID);
  temp->idModelRevMajor = VL6180x_getRegister(VL6180X_IDENTIFICATION_MODEL_REV_MAJOR);
  temp->idModelRevMinor = VL6180x_getRegister(VL6180X_IDENTIFICATION_MODEL_REV_MINOR);
  temp->idModuleRevMajor = VL6180x_getRegister(VL6180X_IDENTIFICATION_MODULE_REV_MAJOR);
  temp->idModuleRevMinor = VL6180x_getRegister(VL6180X_IDENTIFICATION_MODULE_REV_MINOR);

  temp->idDate = VL6180x_getRegister16bit(VL6180X_IDENTIFICATION_DATE);
  temp->idTime = VL6180x_getRegister16bit(VL6180X_IDENTIFICATION_TIME);
}


uint8_t VL6180x::changeAddress(uint8_t old_address, uint8_t new_address){
  
 
  if( old_address == new_address) return old_address;
  if( new_address > 127) return old_address;
   
   VL6180x_setRegister(VL6180X_I2C_SLAVE_DEVICE_ADDRESS, new_address);
   
   return VL6180x_getRegister(VL6180X_I2C_SLAVE_DEVICE_ADDRESS); 
}
  


uint8_t VL6180x::getDistance()
{
  VL6180x_setRegister(VL6180X_SYSRANGE_START, 0x01); //启动单发模式（=0x01）//注意!!!!!!!!!!!!!! 从 0x01 改变
  delay(10);
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  return VL6180x_getRegister(VL6180X_RESULT_RANGE_VAL);
  //返回距离；
}


float VL6180x::getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN)
{
  //首先加载我们正在使用的增益，每次都这样做，以防有人对我们进行更改。
  //注意：高半字节应设置为 0x4，即对于 1.0 的 ALS 增益，写入 0x46
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | VL6180X_ALS_GAIN)); //设置ALS增益

  //开始ALS测量
  VL6180x_setRegister(VL6180X_SYSALS_START, 0x01);

    delay(100);

  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

  //从传感器中检索原始 ALS 值
  unsigned int alsRaw = VL6180x_getRegister16bit(VL6180X_RESULT_ALS_VAL);
  
 //获取积分周期进行计算，我们每次都这样做，以防有人更改它。
  unsigned int alsIntegrationPeriodRaw = VL6180x_getRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD);
  
  float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw ;

  //从 Appnotes 计算实际 LUX

  float alsGain = 0.0;
  
  switch (VL6180X_ALS_GAIN){
    case GAIN_20: alsGain = 20.0; break;
    case GAIN_10: alsGain = 10.32; break;
    case GAIN_5: alsGain = 5.21; break;
    case GAIN_2_5: alsGain = 2.60; break;
    case GAIN_1_67: alsGain = 1.72; break;
    case GAIN_1_25: alsGain = 1.28; break;
    case GAIN_1: alsGain = 1.01; break;
    case GAIN_40: alsGain = 40.0; break;
  }

//根据 AppNotes 中的公式计算 LUX
  
  float alsCalculated = (float)0.32 * ((float)alsRaw / alsGain) * alsIntegrationPeriod;

  return alsCalculated;
}

// --- 私有函数 --- //

uint8_t VL6180x::VL6180x_getRegister(uint16_t registerAddr)
{
  uint8_t data;

  Wire.beginTransmission( _i2caddress ); // 在类实例化时设置的地址
  Wire.write((registerAddr >> 8) & 0xFF); //寄存器地址的MSB
  Wire.write(registerAddr & 0xFF); //寄存器地址的LSB
  Wire.endTransmission(false); //发送地址和寄存器地址字节
  Wire.requestFrom( _i2caddress , 1);
  data = Wire.read(); //从选定的寄存器中读取数据

  return data;
}

uint16_t VL6180x::VL6180x_getRegister16bit(uint16_t registerAddr)
{
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  Wire.beginTransmission( _i2caddress ); // 在类实例化时设置的地址
  Wire.write((registerAddr >> 8) & 0xFF); //MSB 的寄存器地址
  Wire.write(registerAddr & 0xFF); //寄存器地址的 LSB
  Wire.endTransmission(false); //发送地址和寄存器地址字节

  Wire.requestFrom( _i2caddress, 2);
  data_high = Wire.read(); //从选定的寄存器中读取数据
  data_low = Wire.read(); //从选定的寄存器中读取数据
  data = (data_high << 8)|data_low;

  return data;
}

void VL6180x::VL6180x_setRegister(uint16_t registerAddr, uint8_t data)
{
  Wire.beginTransmission( _i2caddress ); //类实例化时设置的地址
  Wire.write((registerAddr >> 8) & 0xFF); //MSB 的寄存器地址
  Wire.write(registerAddr & 0xFF); //LSB 的寄存器地址
  Wire.write(data); // 要发送到设备的数据/设置。
  Wire.endTransmission(); //发送地址和寄存器地址字节
}

void VL6180x::VL6180x_setRegister16bit(uint16_t registerAddr, uint16_t data)
{
  Wire.beginTransmission( _i2caddress ); // //类实例化时设置的地址
  Wire.write((registerAddr >> 8) & 0xFF); //MSB  的寄存器地址
  Wire.write(registerAddr & 0xFF); //LSB  的寄存器地址
  uint8_t temp;
  temp = (data >> 8) & 0xff;
  Wire.write(temp); // 要发送到设备的数据/设置。
  temp = data & 0xff;
  Wire.write(temp); // 要发送到设备的数据/设置。
  Wire.endTransmission(); //发送地址和寄存器地址字节
}

// // 想要额外的功能

void VL6180x::VL6180xSetDistInt(uint8_t lowThres, uint8_t highThres){
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x03);
  VL6180x_setRegister(VL6180X_SYSRANGE_THRESH_LOW, lowThres); 
  VL6180x_setRegister(VL6180X_SYSRANGE_THRESH_HIGH, highThres);
}

void VL6180x::VL6180xSetALSInt(vl6180x_als_gain VL6180X_ALS_GAIN, uint16_t lowThres, uint16_t highThres){
  unsigned int alsIntegrationPeriodRaw = VL6180x_getRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD);
  
  float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw ;


  float alsGain = 0.0;
  
  switch (VL6180X_ALS_GAIN){
    case GAIN_20: alsGain = 20.0; break;
    case GAIN_10: alsGain = 10.32; break;
    case GAIN_5: alsGain = 5.21; break;
    case GAIN_2_5: alsGain = 2.60; break;
    case GAIN_1_67: alsGain = 1.72; break;
    case GAIN_1_25: alsGain = 1.28; break;
    case GAIN_1: alsGain = 1.01; break;
    case GAIN_40: alsGain = 40.0; break;
  }
	
  uint16_t alsLowThresRaw = (uint16_t)((float)lowThres * alsGain) / (alsIntegrationPeriod *0.32);
  uint16_t alsHighThresRaw = (uint16_t)((float)highThres * alsGain) / (alsIntegrationPeriod *0.32);
  Serial.println(alsLowThresRaw);
  Serial.println(alsHighThresRaw);
  
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x18);
  VL6180x_setRegister16bit(VL6180X_SYSALS_THRESH_LOW, alsLowThresRaw); 
  VL6180x_setRegister16bit(VL6180X_SYSALS_THRESH_HIGH, alsHighThresRaw);
}

uint8_t VL6180x::getDistanceContinously()
{
  VL6180x_setRegister(VL6180X_SYSRANGE_START, 0x01); delay(10);
  VL6180x_setRegister(VL6180X_SYSRANGE_START, 0x03); 
  delay(10);
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  VL6180x_setRegister(VL6180X_SYSTEM_HISTORY_CTRL, 0x01); // 为范围启用历史缓冲区
  return VL6180x_getRegister(VL6180X_RESULT_RANGE_VAL);
  //	return distance;
}

uint8_t VL6180x::getLastDistanceFromHistory()
{
  uint8_t lastDist;
  uint16_t histVal;
  histVal = VL6180x_getRegister16bit(VL6180X_RESULT_HISTORY_BUFFER);
  lastDist = histVal>>8; // 16 位历史缓冲区 0 的高位字节包含最后一个范围
  return lastDist;
}

float VL6180x::getAmbientLightContinously(vl6180x_als_gain VL6180X_ALS_GAIN)
{
//首先加载我们正在使用的增益，每次都这样做，以防有人对我们进行更改。
   //注意：高半字节应设置为 0x4，即对于 1.0 的 ALS 增益，写入 0x46
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | VL6180X_ALS_GAIN)); //设置ALS增益

  //开始ALS测量
  VL6180x_setRegister(VL6180X_SYSALS_START, 0x01); delay(100);
  VL6180x_setRegister(VL6180X_SYSALS_START, 0x03);

    delay(100); //give it time... 

  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  VL6180x_setRegister(VL6180X_SYSTEM_HISTORY_CTRL, 0x03); // 为 ALS 启用历史缓冲区

  //从传感器中检索原始 ALS 值
  unsigned int alsRaw = VL6180x_getRegister16bit(VL6180X_RESULT_ALS_VAL);
  
//获取积分周期进行计算，我们每次都这样做，以防有人更改它。
  unsigned int alsIntegrationPeriodRaw = VL6180x_getRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD);
  
  float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw ;

  //Calculate actual LUX from Appnotes

  float alsGain = 0.0;
  
  switch (VL6180X_ALS_GAIN){
    case GAIN_20: alsGain = 20.0; break;
    case GAIN_10: alsGain = 10.32; break;
    case GAIN_5: alsGain = 5.21; break;
    case GAIN_2_5: alsGain = 2.60; break;
    case GAIN_1_67: alsGain = 1.72; break;
    case GAIN_1_25: alsGain = 1.28; break;
    case GAIN_1: alsGain = 1.01; break;
    case GAIN_40: alsGain = 40.0; break;
  }
  //Calculate LUX from formula in AppNotes
  
  float alsCalculated = (float)0.32 * ((float)alsRaw / alsGain) * alsIntegrationPeriod;

  return alsCalculated;
}

float VL6180x::getLastAmbientLightFromHistory(vl6180x_als_gain VL6180X_ALS_GAIN)
{
//从历史缓冲区中获取原始ALS
  unsigned int alsRaw = VL6180x_getRegister16bit(VL6180X_RESULT_HISTORY_BUFFER);
  
//获取积分周期进行计算，我们每次都这样做，以防有人更改它。
  unsigned int alsIntegrationPeriodRaw = VL6180x_getRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD);
  
  float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw ;



  float alsGain = 0.0;
  
  switch (VL6180X_ALS_GAIN){
    case GAIN_20: alsGain = 20.0; break;
    case GAIN_10: alsGain = 10.32; break;
    case GAIN_5: alsGain = 5.21; break;
    case GAIN_2_5: alsGain = 2.60; break;
    case GAIN_1_67: alsGain = 1.72; break;
    case GAIN_1_25: alsGain = 1.28; break;
    case GAIN_1: alsGain = 1.01; break;
    case GAIN_40: alsGain = 40.0; break;
  }

  
  float alsCalculated = (float)0.32 * ((float)alsRaw / alsGain) * alsIntegrationPeriod;

  return alsCalculated;
}

void VL6180x::VL6180xClearInterrupt(void){
	VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
}