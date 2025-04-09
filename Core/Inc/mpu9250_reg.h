/**
 * @file mpu9250_reg.h
 * @brief MPU9250 register definitions
 * @author Stephen Truong
 * @date 2025-03-21 
 */

#ifndef _MPU9250_REG_H
#define _MPU9250_REG_H

/* Gyroscope self-test registers: 0x00 - 0x02(0:2)*/
#define SELF_TEST_X_GYRO         0x00U
#define SELF_TEST_Y_GYRO         0x01U
#define SELF_TEST_Z_GYRO         0x02U

/* Accelerometer self-test registers: 0x0D - 0x0F (13:15)*/
#define SELF_TEST_X_ACCEL        0x0DU
#define SELF_TEST_Y_ACCEL        0x0EU
#define SELF_TEST_Z_ACCEL        0x0FU

/* Gyro offset Registers: 0x13 - 0x15(19:24) */
#define XG_OFFSET_H              0x13U
#define XG_OFFSET_L              0x14U
#define YG_OFFSET_H              0x15U
#define YG_OFFSET_L              0x16U
#define ZG_OFFSET_H              0x17U
#define ZG_OFFSET_L              0x18U

/* Sample rate divider: 0x19(25)*/
#define SMPLRT_DIV               0x19U

/* Configuration: 0x1A(26) */
#define CONFIG                   0x1AU

/* Gyroscope configuration: 0x1B(27) */
#define GYRO_CONFIG              0x1BU

/* Accelerometer configuration: 0x1C(28) */
#define ACCEL_CONFIG             0x1CU

/* Accelerometer configuration 2: 0x1D(29) */
#define ACCEL_CONFIG2            0x1DU

/* Low power accelerometer configuration: 0x1E(30) */
#define LP_ACCEL_ODR             0x1EU

/* Wake-on motion threshold: 0x1F(31) */
#define WOM_THR                  0x1FU

/* FIFO enable: 0x23(35)*/
#define FIFO_EN                  0x23U

/* I2C master control: 0x24(36) */
#define I2C_MST_CTRL             0x24U

/* I2C slave 0 control: 0x25(37) */
#define I2C_SLV0_ADDR            0x25U

/* I2C slave 0 register: 0x26(38) */
#define I2C_SLV0_REG             0x26U

/* I2C slave 0 control: 0x27(39) */
#define I2C_SLV0_CTRL            0x27U

/* I2C slave 1 address: 0x28(40) */
#define I2C_SLV1_ADDR            0x28U

/* I2C slave 1 register: 0x29(41) */
#define I2C_SLV1_REG             0x29U

/* I2C slave 1 control: 0x2A(42) */
#define I2C_SLV1_CTRL            0x2AU

/* I2C slave 2 address: 0x2B(43) */
#define I2C_SLV2_ADDR            0x2BU

/* I2C slave 2 register: 0x2C(44) */
#define I2C_SLV2_REG             0x2CU

/* I2C slave 2 control: 0x2D(45) */ 
#define I2C_SLV2_CTRL            0x2DU

/* I2C slave 3 address: 0x2E(46) */
#define I2C_SLV3_ADDR            0x2EU

/* I2C slave 3 register: 0x2F(47) */
#define I2C_SLV3_REG             0x2FU

/* I2C slave 3 control: 0x30(48) */
#define I2C_SLV3_CTRL            0x30U

/* I2C slave 4 address: 0x31(49) */
#define I2C_SLV4_ADDR            0x31U

/* I2C slave 4 register: 0x32(50) */
#define I2C_SLV4_REG             0x32U

/* I2C slave 4 data out:0x33(51) */
#define I2C_SLV4_DO              0x33U

/* I2C master delay control: 0x34(52) */
#define I2C_SLV4_CTRL           0x34U

/* I2C slave 4 data in: 0x35(53) */
#define I2C_SLV4_DI             0x35U

/* I2C master status: 0x36(54) */
#define I2C_MST_STATUS          0x36U

/* Interrupt pin configuration: 0x37(55) */
#define INT_PIN_CFG             0x37U

/* Interrupt enable: 0x38(56) */
#define INT_ENABLE              0x38U

/* Interrupt status: 0x3A(58) */
#define INT_STATUS              0x3AU

/* Accelerometer measurements: 0x3B - 0x40(59:64) */
#define ACCEL_XOUT_H            0x3BU
#define ACCEL_XOUT_L            0x3CU
#define ACCEL_YOUT_H            0x3DU
#define ACCEL_YOUT_L            0x3EU
#define ACCEL_ZOUT_H            0x3FU
#define ACCEL_ZOUT_L            0x40U

/* Temperature measurement: 0x41 - 0x42(65:66) */
#define TEMP_OUT_H              0x41U
#define TEMP_OUT_L              0x42U

/* Gyroscope measurements: 0x43 - 0x48(67:72) */
#define GYRO_XOUT_H             0x43U
#define GYRO_XOUT_L             0x44U
#define GYRO_YOUT_H             0x45U
#define GYRO_YOUT_L             0x46U
#define GYRO_ZOUT_H             0x47U
#define GYRO_ZOUT_L             0x48U

/* External sensor data: 0x49 - 0x60(73:96) */
#define EXT_SENS_DATA_00        0x49U
#define EXT_SENS_DATA_01        0x4AU
#define EXT_SENS_DATA_02        0x4BU
#define EXT_SENS_DATA_03        0x4CU
#define EXT_SENS_DATA_04        0x4DU
#define EXT_SENS_DATA_05        0x4EU
#define EXT_SENS_DATA_06        0x4FU
#define EXT_SENS_DATA_07        0x50U
#define EXT_SENS_DATA_08        0x51U
#define EXT_SENS_DATA_09        0x52U   
#define EXT_SENS_DATA_10        0x53U
#define EXT_SENS_DATA_11        0x54U
#define EXT_SENS_DATA_12        0x55U
#define EXT_SENS_DATA_13        0x56U
#define EXT_SENS_DATA_14        0x57U
#define EXT_SENS_DATA_15        0x58U
#define EXT_SENS_DATA_16        0x59U
#define EXT_SENS_DATA_17        0x5AU
#define EXT_SENS_DATA_18        0x5BU
#define EXT_SENS_DATA_19        0x5CU
#define EXT_SENS_DATA_20        0x5DU
#define EXT_SENS_DATA_21        0x5EU
#define EXT_SENS_DATA_22        0x5FU
#define EXT_SENS_DATA_23        0x60U

/* I2C Slave 0 Data out: 0x63(99) */
#define I2C_SLV0_DO             0x63U

/* I2C Slave 1 Data out: 0x64(100) */
#define I2C_SLV1_DO             0x64U

/* I2C Slave 2 Data out: 0x65(101) */
#define I2C_SLV2_DO             0x65U

/* I2C Slave 3 Data out: 0x66(102) */   
#define I2C_SLV3_DO             0x66U

/* I2C Master delay control: 0x67(103) */
#define I2C_MST_DELAY_CTRL      0x67U

/* Signal path reset: 0x68(104) */
#define SIGNAL_PATH_RESET       0x68U

/* Accelerometer interrupt control: 0x69(105) */
#define MOT_DETECT_CTRL         0x69U

/* User control: 0x6A(106) */
#define USER_CTRL               0x6AU

/* Power management 1: 0x6B(107) */
#define PWR_MGMT_1              0x6BU

/* Power management 2: 0x6C(108) */
#define PWR_MGMT_2              0x6CU

/* FIFO count registers: 0x72 - 0x73(114:115) */
#define FIFO_COUNTH             0x72U
#define FIFO_COUNTL             0x73U   

/* FIFO read write: 0x74(116) */
#define FIFO_R_W                0x74U

/* Who am I: 0x75(117) */
#define WHO_AM_I                0x75U

/* Accelerometer offset: 0x77, 0x78, 0x7A, 0x7B, 0x7D, 0x7E (119, 120, 122, 123, 125, 126) */
#define XA_OFFSET_H             0x77U
#define XA_OFFSET_L             0x78U

#define YA_OFFSET_H             0x7AU
#define YA_OFFSET_L             0x7BU

#define ZA_OFFSET_H             0x7DU
#define ZA_OFFSET_L             0x7EU

/* Magnetometer registers: 0x0A - 0x0F(10:15) */
#define WIA                     0x00U
#define INFO                    0x01U
#define ST1                     0x02U
#define HXL                     0x03U
#define HXH                     0x04U
#define HYL                     0x05U
#define HYH                     0x06U
#define HZL                     0x07U
#define HZH                     0x08U
#define ST2                     0x09U
#define CNTL                    0x0AU
#define CNTL2                   0x0BU
#define ASTC                    0x0CU
#define TS1                     0x0DU
#define TS2                     0x0EU
#define I2CDIS                  0x0FU

/* Magnetometer sensitivity adjustment values: 0x10 - 0x12(16:18) */
#define ASAX                    0x10U
#define ASAY                    0x11U
#define ASAZ                    0x12U


#endif