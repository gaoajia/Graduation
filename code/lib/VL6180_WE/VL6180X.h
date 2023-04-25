#ifndef VL6180X_h_
#define VL6180X_h_

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <Wire.h>

#define VL6180x_FAILURE_RESET  -1

#define VL6180X_IDENTIFICATION_MODEL_ID              0x0000
#define VL6180X_IDENTIFICATION_MODEL_REV_MAJOR       0x0001
#define VL6180X_IDENTIFICATION_MODEL_REV_MINOR       0x0002
#define VL6180X_IDENTIFICATION_MODULE_REV_MAJOR      0x0003
#define VL6180X_IDENTIFICATION_MODULE_REV_MINOR      0x0004
#define VL6180X_IDENTIFICATION_DATE                  0x0006 //16bit value
#define VL6180X_IDENTIFICATION_TIME                  0x0008 //16bit value

#define VL6180X_SYSTEM_MODE_GPIO0                    0x0010
#define VL6180X_SYSTEM_MODE_GPIO1                    0x0011
#define VL6180X_SYSTEM_HISTORY_CTRL                  0x0012
#define VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO         0x0014
#define VL6180X_SYSTEM_INTERRUPT_CLEAR               0x0015
#define VL6180X_SYSTEM_FRESH_OUT_OF_RESET            0x0016
#define VL6180X_SYSTEM_GROUPED_PARAMETER_HOLD        0x0017

#define VL6180X_SYSRANGE_START                       0x0018
#define VL6180X_SYSRANGE_THRESH_HIGH                 0x0019
#define VL6180X_SYSRANGE_THRESH_LOW                  0x001A
#define VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD     0x001B
#define VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME        0x001C
#define VL6180X_SYSRANGE_CROSSTALK_COMPENSATION_RATE 0x001E
#define VL6180X_SYSRANGE_CROSSTALK_VALID_HEIGHT      0x0021
#define VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE  0x0022
#define VL6180X_SYSRANGE_PART_TO_PART_RANGE_OFFSET   0x0024
#define VL6180X_SYSRANGE_RANGE_IGNORE_VALID_HEIGHT   0x0025
#define VL6180X_SYSRANGE_RANGE_IGNORE_THRESHOLD      0x0026
#define VL6180X_SYSRANGE_MAX_AMBIENT_LEVEL_MULT      0x002C
#define VL6180X_SYSRANGE_RANGE_CHECK_ENABLES         0x002D
#define VL6180X_SYSRANGE_VHV_RECALIBRATE             0x002E
#define VL6180X_SYSRANGE_VHV_REPEAT_RATE             0x0031

#define VL6180X_SYSALS_START                         0x0038
#define VL6180X_SYSALS_THRESH_HIGH                   0x003A
#define VL6180X_SYSALS_THRESH_LOW                    0x003C
#define VL6180X_SYSALS_INTERMEASUREMENT_PERIOD       0x003E
#define VL6180X_SYSALS_ANALOGUE_GAIN                 0x003F
#define VL6180X_SYSALS_INTEGRATION_PERIOD            0x0040

#define VL6180X_RESULT_RANGE_STATUS                  0x004D
#define VL6180X_RESULT_ALS_STATUS                    0x004E
#define VL6180X_RESULT_INTERRUPT_STATUS_GPIO         0x004F
#define VL6180X_RESULT_ALS_VAL                       0x0050
#define VL6180X_RESULT_HISTORY_BUFFER                0x0052 
#define VL6180X_RESULT_RANGE_VAL                     0x0062
#define VL6180X_RESULT_RANGE_RAW                     0x0064
#define VL6180X_RESULT_RANGE_RETURN_RATE             0x0066
#define VL6180X_RESULT_RANGE_REFERENCE_RATE          0x0068
#define VL6180X_RESULT_RANGE_RETURN_SIGNAL_COUNT     0x006C
#define VL6180X_RESULT_RANGE_REFERENCE_SIGNAL_COUNT  0x0070
#define VL6180X_RESULT_RANGE_RETURN_AMB_COUNT        0x0074
#define VL6180X_RESULT_RANGE_REFERENCE_AMB_COUNT     0x0078
#define VL6180X_RESULT_RANGE_RETURN_CONV_TIME        0x007C
#define VL6180X_RESULT_RANGE_REFERENCE_CONV_TIME     0x0080

#define VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD      0x010A
#define VL6180X_FIRMWARE_BOOTUP                      0x0119
#define VL6180X_FIRMWARE_RESULT_SCALER               0x0120
#define VL6180X_I2C_SLAVE_DEVICE_ADDRESS             0x0212
#define VL6180X_INTERLEAVED_MODE_ENABLE              0x02A3


enum vl6180x_als_gain { //数据表将增益值显示为二进制列表

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
  ////按照 ST 数据表中的说明发送强制设置
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf (Section 1.3)
  uint8_t VL6180xInit(void);
  //使用 ST 数据表第 9 节中的默认设置。
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  void VL6180xDefautSettings(void);

  //获取范围距离（毫米）
  uint8_t getDistance();
  //获取 Lux 中的 ALS 级别
  float getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN);

  //Load structure provided by the user with identification info
  //Structure example:
  // struct VL6180xIdentification
  //  {
  //   uint8_t idModel;
  //   uint8_t idModelRevMajor;
  //   uint8_t idModelRevMinor;
  //   uint8_t idModuleRevMajor;
  //   uint8_t idModuleRevMinor;
  //   uint16_t idDate;
  //   uint16_t idTime;
  //   };
  void getIdentification(struct VL6180xIdentification *temp);

//更改设备的默认地址以允许多个
   //总线上的传感器。 最多可使用 127 个传感器。 新地址
   //保存在非易失性设备内存中。
  uint8_t changeAddress(uint8_t old_address, uint8_t new_address);
  //Wolle的附加功能：
  void VL6180xSetDistInt(uint8_t lowThres, uint8_t highThres);
  void VL6180xSetALSInt(vl6180x_als_gain VL6180X_ALS_GAIN, uint16_t lowThres, uint16_t highThres);
  uint8_t getDistanceContinously();
  uint8_t getLastDistanceFromHistory();
  float getAmbientLightContinously(vl6180x_als_gain VL6180X_ALS_GAIN);
  float getLastAmbientLightFromHistory(vl6180x_als_gain VL6180X_ALS_GAIN);
  void VL6180xClearInterrupt(void);

private:
//存储类初始化时给定的地址。
   //这个值可以通过changeAddress()函数改变
  int _i2caddress;

  uint8_t VL6180x_getRegister(uint16_t registerAddr);
  uint16_t VL6180x_getRegister16bit(uint16_t registerAddr);

  void VL6180x_setRegister(uint16_t registerAddr, uint8_t data);
  void VL6180x_setRegister16bit(uint16_t registerAddr, uint16_t data);

};

#endif

