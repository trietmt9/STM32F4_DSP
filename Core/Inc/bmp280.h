/**
 * @file bmp280.h
 * @author  trietmt9
 * @date 18-4-2025 
 * @brief BMP280 driver header file
 * @note This file is part of the BMP280 driver for STM32.
 * @note This driver is designed to work with STM32 microcontrollers. 
 */

#ifndef __BMP280_H__
#define __BMP280_H__

/**
 * @brief: Include necessary libraries
*/
#include "main.h"
#include "math.h"
#include "string.h"
#include "stdint.h"

/**
 * @brief: Define BMP280 I2C address
 */
#define BMP280_I2C_ADDRESS          0x76U << 1
#define BMP280_CHIP_ID              0x58U

/**
 * @brief: Define BMP280 registers
 */
#define BMP280_ID_REG                      0xD0U
#define BMP280_RESET_REG                   0xE0U
#define BMP280_STATUS_REG                  0xF3U
#define BMP280_CTRL_MEAS_REG               0xF4U
#define BMP280_CONFIG_REG                  0xF5U
#define BMP280_PRESSURE_MSB_REG            0xF7U
#define BMP280_PRESSURE_LSB_REG            0xF8U
#define BMP280_PRESSURE_XLSB_REG           0xF9U
#define BMP280_TEMPERATURE_MSB_REG         0xFAU
#define BMP280_TEMPERATURE_LSB_REG         0xFBU
#define BMP280_TEMPERATURE_XLSB_REG        0xFCU
#define BMP280_CALIBRATION00_START_REG     0x88U
#define BMP280_CALIBRATION25_END_REG       0xA1U
#define BMP280_SOFT_RESET_VALUE            0xB6U
/** 
 * @brief BMP280 status enum
 */
typedef enum
{
    BMP280_OK = 0,
    BMP280_ERROR = 1,
} bmp280_status_t;

/**
 * @brief BMP280 calibration data structure
 */
 typedef enum 
 {
    SKIPED = 0,
    OVER_SAMPLINNG_x1,
    OVER_SAMPLINNG_x2,
    OVER_SAMPLINNG_x4,
    OVER_SAMPLINNG_x8,
    OVER_SAMPLINNG_x16,
 }bmp280_oversample_t;

/**
 * @brief BMP280 operation type enum
 */
typedef enum
{
    BMP280_SPI_4WIRE = 0,
    BMP280_SPI_3WIRE = 1
}bmp280_SPI_operation_t;

/**
 * @brief BMP280 mode enum
 */
typedef enum
{
    BMP280_SLEEP_MODE = 0,
    BMP280_FORCED_MODE = 1,
    BMP280_NORMAL_MODE = 3,
}bmp280_mode_t;

/**
 * @brief BMP280 IIR filter constants
 */
typedef enum
{
    BMP280_IIR_FILTER_OFF = 0,
    BMP280_IIR_FILTER_2 = 1,
    BMP280_IIR_FILTER_4 = 2,
    BMP280_IIR_FILTER_8 = 3,
    BMP280_IIR_FILTER_16 = 4,
}bmp280_IIR_filter_t;

/**
 * @brief BMP280 stanby time constants
 */
typedef enum
{
    BMP280_STANDBY_TIME_0_5  = 0,
    BMP280_STANDBY_TIME_62_5 = 1,
    BMP280_STANDBY_TIME_125  = 2,
    BMP280_STANDBY_TIME_250  = 3,
    BMP280_STANDBY_TIME_500  = 4,
    BMP280_STANDBY_TIME_1000 = 5,
    BMP280_STANDBY_TIME_2000 = 6,
    BMP280_STANDBY_TIME_4000 = 7,
}bmp280_standby_time_t;

/**
 * @brief BMP280 struct 
 */
typedef struct 
{
    uint8_t data; 
    // state variables 
    float pressure; 
    float temperature;
    float altitude;
    
    // Raw variables
    int32_t rawPress;
    int32_t rawTemp;
    // calibrating data 
    float calib;
    // trim data storing
    int32_t  dig_T1, dig_T2, dig_T3;
    int64_t  dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

}bmp280_t; 
/**
 * @brief BMP280 driver functions
 */
bmp280_status_t BMP280_ID(I2C_HandleTypeDef* phi2c, uint8_t *id);
bmp280_status_t BMP280_Init(I2C_HandleTypeDef *phi2c, bmp280_SPI_operation_t spi_operation, bmp280_mode_t mode, bmp280_oversample_t oversampling_p, bmp280_oversample_t oversampling_t, bmp280_IIR_filter_t iir_filter, bmp280_standby_time_t standby_time);
bmp280_status_t BMP280_Read(I2C_HandleTypeDef *phi2c, bmp280_t *pBMP280);
bmp280_status_t BMP280_ReadDMA(I2C_HandleTypeDef *phi2c, bmp280_t *pBMP280);
bmp280_status_t BMP280_ReadCalibrationData(I2C_HandleTypeDef *phi2c, bmp280_t *pBMP280);
#endif