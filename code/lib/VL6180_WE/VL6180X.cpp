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

            /* 
            0x0207 和 0x0208：分别用于激活和重新启动传感器。
            0x0096 和 0x0097：用于设置距离测量模式下的采样率。
            0x00e3 到 0x00e7：用于设置环境光强度测量模式下的积分时间和增益。
            0x00f5：用于设置距离测量模式下的偏差校准。
            0x00d9、0x00db、0x00dc、0x00dd 和 0x009f：用于设置其他距离测量参数。
            0x00a3：用于设置GPIO功能。
            0x00b2 和 0x00bb：用于设置距离测量结果滤波器。
            0x00ca：用于设置距离测量结果偏差校准。
            0x0198：用于设置时钟控制。
            0x01b0 和 0x01ad：用于设置全局偏差校准。
            0x00ff、0x0100 和 0x0199：用于设置距离测量的 ROI。
            0x01a6、0x01ac 和 0x01a7：用于设置距离测量模式下的触发器和周围光线干扰检测功能。
            0x0030：用于关闭额外的GPIO输出。
            */

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
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46); //设置ALS增益 // 按照数据表中的建议将环境光强度增益设置为72（即0x46）
  VL6180x_setRegister(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF); // 设置自动校准周期（最大值 = 255）/（关闭 = 0）以提高稳定性。
  VL6180x_setRegister(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63); // 将环境光强度测量 （ALS） 积分时间设置为 100ms
  VL6180x_setRegister(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01); // 执行单次温度校准
  //数据表中的可选设置
  //http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  VL6180x_setRegister(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // 将默认测距间隔测量周期设置为 100ms
  VL6180x_setRegister(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A); // 将默认的 ALS 测量间隔设置为 100ms
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x00); // 配置“新样品就绪阈值事件”中断 
  //来自社区的其他默认设置
  VL6180x_setRegister(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);//用于设置距离测量模式下最大收敛时间。建议值为50（0x32），以平衡精度和响应速度。
  VL6180x_setRegister(VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);//用于启用/禁用距离测量结果的范围检查和值设置的校验位，以提高准确性和稳定性。此处将“结果输出超出有效范围”检查和“结果输出负值”检查都启用。
  VL6180x_setRegister16bit(VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B );//用于设置距离测量开头的收敛估计。建议值为123（0x7B），以提高测量精度。
  VL6180x_setRegister16bit(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);//用于设置环境光强度测量积分时间。建议值为100ms（0x64）。

  VL6180x_setRegister(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD,0x30);//用于设置距离测量模式下的平均采样周期。建议值为48个样本（4.3ms）。
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN,0x40);//用于设置环境光强度增益。建议值为64（0x40）。
  VL6180x_setRegister(VL6180X_FIRMWARE_RESULT_SCALER,0x01);//用于设置固件结果缩放系数。建议值为1，以确保正确的距离测量。
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

//修改IIC地址
uint8_t VL6180x::changeAddress(uint8_t old_address, uint8_t new_address){
  if( old_address == new_address) return old_address;
  if( new_address > 127) return old_address;
  VL6180x_setRegister(VL6180X_I2C_SLAVE_DEVICE_ADDRESS, new_address);
  return VL6180x_getRegister(VL6180X_I2C_SLAVE_DEVICE_ADDRESS); 
}
  

//VL6180x 传感器获取距离
//需要注意的是，在将系统距离测量寄存器设置为单发模式后，传感器会在每次测量结束后自动返回到空闲模式。
//因此，如果需要连续测量多个距离值，则需要在每次测量之间添加适当的延迟，并再次调用此函数以启动下一次测量。
uint8_t VL6180x::getDistance()
{
  VL6180x_setRegister(VL6180X_SYSRANGE_START, 0x01); //启动单发模式（=0x01）//注意!!!!!!!!!!!!!! 从 0x01 改变
  delay(10);
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  return VL6180x_getRegister(VL6180X_RESULT_RANGE_VAL);
  //返回距离；
}

//VL6180x 传感器获取环境光强度
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

// ---  VL6180x 传感器读取寄存器值的私有函数 --- //
//从传感器设备中读取一个字节的数据，并返回该数据作为函数的输出。
uint8_t VL6180x::VL6180x_getRegister(uint16_t registerAddr)
{
  uint8_t data;

  Wire.beginTransmission( _i2caddress ); // 启动 I2C 通信并设置设备地址为类实例化时设置的地址。
  Wire.write((registerAddr >> 8) & 0xFF); //将寄存器地址分为高低两部分（MSB 和 LSB）写入 I2C 缓冲区中。
  Wire.write(registerAddr & 0xFF); //LSB
  Wire.endTransmission(false); //结束 I2C 传输但不释放总线，以便后续读取数据。
  Wire.requestFrom( _i2caddress , 1);//从设备地址为类实例化时设置的地址的设备中读取一个字节的数据。
  data = Wire.read(); //从 I2C 缓冲区中读取刚刚请求的数据，并将其存储在变量 data 中。

  return data;
}
//从传感器设备中读取两个字节的数据，并将其合并成一个 16 位的数据，返回该数据作为函数的输出。
uint16_t VL6180x::VL6180x_getRegister16bit(uint16_t registerAddr)
{
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  Wire.beginTransmission( _i2caddress ); 
  Wire.write((registerAddr >> 8) & 0xFF); 
  Wire.write(registerAddr & 0xFF); 
  Wire.endTransmission(false); 

  Wire.requestFrom( _i2caddress, 2);
  data_high = Wire.read(); //从 I2C 缓冲区中读取刚刚请求的第一个数据，并将其存储在变量 data_high 中。
  data_low = Wire.read(); //从 I2C 缓冲区中读取刚刚请求的第二个数据，并将其存储在变量 data_low 中。
  data = (data_high << 8)|data_low;//合并成一个 16 位的数据，存储在变量 data 中。

  return data;
}
//将一个字节的数据写入传感器设备的寄存器中。
void VL6180x::VL6180x_setRegister(uint16_t registerAddr, uint8_t data)
{
  Wire.beginTransmission( _i2caddress ); 
  Wire.write((registerAddr >> 8) & 0xFF); 
  Wire.write(registerAddr & 0xFF); 
  Wire.write(data); 
  Wire.endTransmission(); 
}
//将一个 16 位的数据分为高低两个字节，并将它们依次写入传感器设备的寄存器中。
void VL6180x::VL6180x_setRegister16bit(uint16_t registerAddr, uint16_t data)
{
  Wire.beginTransmission( _i2caddress ); 
  Wire.write((registerAddr >> 8) & 0xFF); 
  Wire.write(registerAddr & 0xFF);
  uint8_t temp;
  temp = (data >> 8) & 0xff;
  Wire.write(temp); 
  temp = data & 0xff;
  Wire.write(temp); 
  Wire.endTransmission(); 
}

// // 想要额外的功能

//设置距离阈值中断
void VL6180x::VL6180xSetDistInt(uint8_t lowThres, uint8_t highThres){
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x03);//启用 GPIO 中断功能。
  VL6180x_setRegister(VL6180X_SYSRANGE_THRESH_LOW, lowThres); //设置距离低阈值
  VL6180x_setRegister(VL6180X_SYSRANGE_THRESH_HIGH, highThres);//设置距离高阈值
}

/* VL6180x 传感器设置环境光强度阈值中断的函数实现，该函数接受三个参数，VL6180X_ALS_GAIN 表示 ALS 增益，lowThres 表示环境光强度低阈值，highThres 表示环境光强度高阈值。
该函数执行以下步骤：
1.调用 VL6180x_getRegister16bit() 函数，读取寄存器 VL6180X_SYSALS_INTEGRATION_PERIOD 的值，并计算出 ALS 积分周期。
2.根据输入的 VL6180X_ALS_GAIN 参数，计算出对应的 ALS 增益值。
3.将计算得到的 ALS 积分周期和 ALS 增益值一起用于计算环境光强度阈值的真实值。
4.调用 VL6180x_setRegister16bit() 函数，将真实值写入寄存器 VL6180X_THRESH_LOW 和 VL6180X_THRESH_HIGH 中，以设置环境光强度低阈值和高阈值。
5.调用 VL6180x_setRegister() 函数，将寄存器 VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO 的值设置为 0x18，以启用环境光强度中断功能。
 */
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


/* VL6180x 传感器连续获取距离值的函数实现，该函数执行以下步骤：
1.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSRANGE_START 写入值 0x01，以启动一次新的测量。
2.延迟 10 毫秒，等待测量结果。
3.调用 VL6180x_setRegister() 函数，再次向寄存器 VL6180X_SYSRANGE_START 写入值 0x03，以使传感器开始连续测量模式。在连续测量模式下，距离值将被不断更新。
4.延迟 10 毫秒，等待传感器进入连续测量模式。
5.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSTEM_INTERRUPT_CLEAR 写入值 0x07，以清除所有中断标志位。
6.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSTEM_HISTORY_CTRL 写入值 0x01，以为范围测量启用历史缓冲区。
7.调用 VL6180x_getRegister() 函数，读取寄存器 VL6180X_RESULT_RANGE_VAL 的值，并返回该值作为函数的输出。这个值就是当前测量得到的距离值。 */
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

/*  VL6180x 传感器从历史缓冲区中读取最后一个距离值的函数实现。该函数执行以下步骤：
1.声明一个 lastDist 变量，用于存储从历史缓冲区中读取到的距离值。
2.调用 VL6180x_getRegister16bit() 函数，读取 16 位历史缓冲区寄存器 VL6180X_RESULT_HISTORY_BUFFER 的值，并将该值存储在 histVal 变量中。
3.将 histVal 变量的高位字节右移 8 位，并将结果赋值给 lastDist 变量。
4.返回 lastDist 变量作为函数输出值。这个值就是从历史缓冲区中读取到的最后一个距离值。 */
uint8_t VL6180x::getLastDistanceFromHistory()
{
  uint8_t lastDist;
  uint16_t histVal;
  histVal = VL6180x_getRegister16bit(VL6180X_RESULT_HISTORY_BUFFER);
  lastDist = histVal>>8; // 16 位历史缓冲区 0 的高位字节包含最后一个范围
  return lastDist;
}

/*  VL6180x 传感器连续获取环境光强度值的函数实现。该函数接受一个参数 VL6180X_ALS_GAIN，表示 ALS 增益设置。该函数执行以下步骤：
1.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSALS_ANALOGUE_GAIN 写入 (0x40 | VL6180X_ALS_GAIN) 的值，以设置当前使用的 ALS 增益。
2.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSALS_START 写入值 0x01，以启动一次新的环境光强度测量。
3.延迟 100 毫秒，等待测量结果。
4.调用 VL6180x_setRegister() 函数，再次向寄存器 VL6180X_SYSALS_START 写入值 0x03，以使传感器开始连续测量模式。在连续测量模式下，环境光强度值将被不断更新。
5.延迟 100 毫秒，等待传感器进入连续测量模式。
6.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSTEM_INTERRUPT_CLEAR 写入值 0x07，以清除所有中断标志位。
7.调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSTEM_HISTORY_CTRL 写入值 0x03，以为 ALS 启用历史缓冲区。
8.调用 VL6180x_getRegister16bit() 函数，读取寄存器 VL6180X_RESULT_ALS_VAL 的值，并将结果存储在 alsRaw 变量中。这个值是原始的 ALS 值。
9.调用 VL6180x_getRegister16bit() 函数，读取寄存器 VL6180X_SYSALS_INTEGRATION_PERIOD 的值，并计算出 ALS 积分周期。
10.根据输入的 VL6180X_ALS_GAIN 参数，计算出对应的 ALS 增益值。
11.将计算得到的 ALS 积分周期、ALS 增益值和原始的 ALS 值一起用于计算环境光强度值。
12.返回计算得到的环境光强度值。 */
float VL6180x::getAmbientLightContinously(vl6180x_als_gain VL6180X_ALS_GAIN)
{
  //首先加载我们正在使用的增益，每次都这样做，以防有人对我们进行更改。
  //注意：高半字节应设置为 0x4，即对于 1.0 的 ALS 增益，写入 0x46
  VL6180x_setRegister(VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | VL6180X_ALS_GAIN)); //设置ALS增益
  //开始ALS测量
  VL6180x_setRegister(VL6180X_SYSALS_START, 0x01); delay(100);
  VL6180x_setRegister(VL6180X_SYSALS_START, 0x03);
    delay(100); 
  VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  VL6180x_setRegister(VL6180X_SYSTEM_HISTORY_CTRL, 0x03); // 为 ALS 启用历史缓冲区
  //从传感器中检索原始 ALS 值
  unsigned int alsRaw = VL6180x_getRegister16bit(VL6180X_RESULT_ALS_VAL);
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

/*  VL6180x 传感器从历史缓冲区中读取最后一个环境光强度值的函数实现。该函数接受一个参数 VL6180X_ALS_GAIN，表示 ALS 增益设置。该函数执行以下步骤：
1.调用 VL6180x_getRegister16bit() 函数，读取寄存器 VL6180X_RESULT_HISTORY_BUFFER 的值，并将结果存储在 alsRaw 变量中。这个值是从历史缓冲区中读取到的原始 ALS 值。
2.调用 VL6180x_getRegister16bit() 函数，读取寄存器 VL6180X_SYSALS_INTEGRATION_PERIOD 的值，并计算出 ALS 积分周期。
3.根据输入的 VL6180X_ALS_GAIN 参数，计算出对应的 ALS 增益值。
4.将计算得到的 ALS 积分周期、ALS 增益值和从历史缓冲区中读取到的原始 ALS 值一起用于计算环境光强度值。
5.返回计算得到的环境光强度值。 */
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

//VL6180x 传感器清除中断标志位的函数实现。该函数调用 VL6180x_setRegister() 函数，向寄存器 VL6180X_SYSTEM_INTERRUPT_CLEAR 写入值 0x07，以清除所有中断标志位。
void VL6180x::VL6180xClearInterrupt(void){
	VL6180x_setRegister(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
}