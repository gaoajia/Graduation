#ifndef VL6180X_h_
#define VL6180X_h_

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <Wire.h>

#define VL6180x_FAILURE_RESET  -1 //复位

#define VL6180X_IDENTIFICATION_MODEL_ID              0x0000 //传感器型号标识
#define VL6180X_IDENTIFICATION_MODEL_REV_MAJOR       0x0001 //传感器型号主版本号
#define VL6180X_IDENTIFICATION_MODEL_REV_MINOR       0x0002 //传感器型号次版本号
#define VL6180X_IDENTIFICATION_MODULE_REV_MAJOR      0x0003 //传感器模块主版本号
#define VL6180X_IDENTIFICATION_MODULE_REV_MINOR      0x0004 //传感器模块次版本号
#define VL6180X_IDENTIFICATION_DATE                  0x0006 //传感器制造日期
#define VL6180X_IDENTIFICATION_TIME                  0x0008 //传感器制造时间

#define VL6180X_SYSTEM_MODE_GPIO0                    0x0010 //GPIO0
#define VL6180X_SYSTEM_MODE_GPIO1                    0x0011 //GPIO1
#define VL6180X_SYSTEM_HISTORY_CTRL                  0x0012 //历史数据记录控制寄存器
#define VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO         0x0014 //中断配置GPIO
#define VL6180X_SYSTEM_INTERRUPT_CLEAR               0x0015 //中断清除
#define VL6180X_SYSTEM_FRESH_OUT_OF_RESET            0x0016 //复位状态标志
#define VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD        0x0017 //参数锁定
//SysRange类别，用于配置和控制距离测量相关的参数和操作。
#define VL6180X_SYSRANGE_START                       0x0018 //距离测量启动寄存器
#define VL6180X_SYSRANGE_THRESH_HIGH                 0x0019 //上限阈值
#define VL6180X_SYSRANGE_THRESH_LOW                  0x001A //下限阈值
#define VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD     0x001B //距离测量之间的时间
#define VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME        0x001C //距离测量收敛时间
#define VL6180X_SYSRANGE_CROSSTALK_COMPENSATION_RATE 0x001E //交叉干扰补偿率
#define VL6180X_SYSRANGE_CROSSTALK_VALID_HEIGHT      0x0021 //交叉干扰有效高度
#define VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE  0x0022 //早期距离测量估计值
#define VL6180X_SYSRANGE_PART_TO_PART_RANGE_OFFSET   0x0024 //芯片间距离偏差补偿
#define VL6180X_SYSRANGE_RANGE_IGNORE_VALID_HEIGHT   0x0025 //距离测量不计入有效高度的控制寄存器
#define VL6180X_SYSRANGE_RANGE_IGNORE_THRESHOLD      0x0026 //距离测量不计入阈值的控制寄存器
#define VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT      0x002C //最大环境光强度乘数
#define VL6180X_SYSRANGE_RANGE_CHECK_ENABLES         0x002D //距离测量检查使能
#define VL6180X_SYSRANGE_VHV_RECALIBRATE             0x002E //垂直扫描高压再校准
#define VL6180X_SYSRANGE_VHV_REPEAT_RATE             0x0031 //垂直扫描高压重复速率
//SysAls类别，用于配置和控制环境光强度测量相关的参数和操作。
#define VL6180X_SYSALS_START                         0x0038  //环境光强度测量启动
#define VL6180X_SYSALS_THRESH_HIGH                   0x003A  //环境光强度上限阈值
#define VL6180X_SYSALS_THRESH_LOW                    0x003C  //下限阈值
#define VL6180X_SYSALS_INTERMEASUREMENT_PERIOD       0x003E  //环境光强度测量之间的时间
#define VL6180X_SYSALS_ANALOGUE_GAIN                 0x003F  //环境光强度模拟增益
#define VL6180X_SYSALS_INTEGRATION_PERIOD            0x0040  //环境光强度积分周期
//这些寄存器存储了距离测量和环境光强度测量的实际结果，可以被读取并进一步处理或显示。
#define VL6180X_RESULT_RANGE_STATUS                  0x004D  //距离测量状态
#define VL6180X_RESULT_ALS_STATUS                    0x004E  //环境光强度测量状态
#define VL6180X_RESULT_INTERRUPT_STATUS_GPIO         0x004F  //GPIO中断状态
#define VL6180X_RESULT_ALS_VAL                       0x0050  //环境光强度测量值
#define VL6180X_RESULT_HISTORY_BUFFER                0x0052  // 历史数据缓存
#define VL6180X_RESULT_RANGE_VAL                     0x0062  //距离测量值
#define VL6180X_RESULT_RANGE_RAW                     0x0064  //距离测量原始值
#define VL6180X_RESULT_RANGE_RETURN_RATE             0x0066  //距离返回速率
#define VL6180X_RESULT_RANGE_REFERENCE_RATE          0x0068  //距离参考速率
#define VL6180X_RESULT_RANGE_RETURN_SIGNAL_COUNT     0x006C  //距离返回信号计数
#define VL6180X_RESULT_RANGE_REFERENCE_SIGNAL_COUNT  0x0070  //距离参考信号计数
#define VL6180X_RESULT_RANGE_RETURN_AMB_COUNT        0x0074  //距离返回环境光强度计数
#define VL6180X_RESULT_RANGE_REFERENCE_AMB_COUNT     0x0078  //距离参考环境光强度计数
#define VL6180X_RESULT_RANGE_RETURN_CONV_TIME        0x007C  //距离返回测量收敛时间
#define VL6180X_RESULT_RANGE_REFERENCE_CONV_TIME     0x0080  //距离参考测量收敛时间

#define VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD      0x010A  //读取平均采样周期
#define VL6180X_FIRMWARE_BOOTUP                      0x0119  //固件启动状态
#define VL6180X_FIRMWARE_RESULT_SCALER               0x0120  //固件结果缩放系数
#define VL6180X_I2C_SLAVE_DEVICE_ADDRESS             0x0212  //I2C从设备地址
#define VL6180X_INTERLEAVED_MODE_ENABLE              0x02A3  //交错模式使能


enum vl6180x_als_gain { 

GAIN_20 = 0, // Actual ALS Gain of 20
GAIN_10,     // Actual ALS Gain of 10.32
GAIN_5,      // Actual ALS Gain of 5.21
GAIN_2_5,    // Actual ALS Gain of 2.60
GAIN_1_67,   // Actual ALS Gain of 1.72
GAIN_1_25,   // Actual ALS Gain of 1.28
GAIN_1 ,     // Actual ALS Gain of 1.01
GAIN_40,     // Actual ALS Gain of 40

};

struct VL6180xIdentification
{
  uint8_t idModel;
  uint8_t idModelRevMajor;
  uint8_t idModelRevMinor;
  uint8_t idModuleRevMajor;
  uint8_t idModuleRevMinor;
  uint16_t idDate;
  uint16_t idTime;
};


class VL6180x
{
public:	
  //使用默认地址初始化库
  VL6180x(uint8_t address);
  uint8_t VL6180xInit(void);
  void VL6180xDefautSettings(void);

  //获取当前测量到的范围距离值
  uint8_t getDistance();
  //获取当前测量到的环境光强度值
  float getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN);
  //从传感器中读取标识信息，并将其存储在结构体变量中。
  void getIdentification(struct VL6180xIdentification *temp);
  //更改传感器的I2C地址
  uint8_t changeAddress(uint8_t old_address, uint8_t new_address);
  //设置范围距离中断阈值。
  void VL6180xSetDistInt(uint8_t lowThres, uint8_t highThres);
  //设置环境光强度中断阈值。
  void VL6180xSetALSInt(vl6180x_als_gain VL6180X_ALS_GAIN, uint16_t lowThres, uint16_t highThres);
  //连续获取范围距离值。
  uint8_t getDistanceContinously();
  //获取最近一次测量到的范围距离值。
  uint8_t getLastDistanceFromHistory();
  //连续获取环境光强度值。
  float getAmbientLightContinously(vl6180x_als_gain VL6180X_ALS_GAIN);
  //获取最近一次测量到的环境光强度值。
  float getLastAmbientLightFromHistory(vl6180x_als_gain VL6180X_ALS_GAIN);
  //清除传感器中断标志位。
  void VL6180xClearInterrupt(void);

private:
  //初始化时给定的地址。
  int _i2caddress;
  //从指定寄存器地址中读取一个字节的值。
  uint8_t VL6180x_getRegister(uint16_t registerAddr);
  //从指定寄存器地址中读取两个字节的值并将其存储为16位整数。
  uint16_t VL6180x_getRegister16bit(uint16_t registerAddr);
  //向指定寄存器地址中写入一个字节的值。
  void VL6180x_setRegister(uint16_t registerAddr, uint8_t data);
  //将给定的16位整数值写入到指定寄存器地址中。
  void VL6180x_setRegister16bit(uint16_t registerAddr, uint16_t data);
};

#endif

