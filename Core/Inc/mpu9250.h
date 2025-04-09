/** 
 * @file mpu9250.h
 * @brief MPU9250 driver header file
 * @author Stephen Truong
 * @date 2025-03-21 
*/
#ifndef __MPU9250_H
#define __MPu9250_H

#include "mpu9250_reg.h"
#include "stdint.h"
#include "main.h"
#include "math.h"
#include "stdbool.h"
#define RAD_TO_DEG          (180.0f / M_PI)
#define MPU9250_SPI_TIMEOUT 1000
#define MPU9250_SPI_ID      0x70U
typedef enum 
{
    MPU9250_OK = 0,
    MPU9250_ERROR = 1,
    MPU9250_TIMEOUT = 2
}mpu_state;

typedef enum 
{
    gyro_fs_250dps  = 0,
    gyro_fs_500dps  = 1,
    gyro_fs_1000dps = 2,
    gyro_fs_2000dps = 3
}gyro_fs_t;

typedef enum 
{
    accel_fs_2g  = 0,
    accel_fs_4g  = 1,
    accel_fs_8g  = 2,
    accel_fs_16g = 3
}accel_fs_t;

typedef enum
{
    dlpf_rate_0hz   = 0,
    dlpf_rate_184hz = 1,
    dlpf_rate_92hz  = 2,
    dlpf_rate_41hz  = 3,
    dlpf_rate_20hz  = 4,
    dlpf_rate_10hz  = 5,
    dlpf_rate_5hz   = 6,
    dlpf_rate_3600hz = 7
}gyro_dlpf_rate;

typedef enum
{
    accel_dlpf_218hz = 0,
    accel_dlpf_99hz  = 2,
    accel_dlpf_44_8hz = 3,
    accel_dlpf_21_2hz = 4,
    accel_dlpf_10_2hz = 5,
    accel_dlpf_5_05hz = 6,
    accel_dlpf_420hz = 7
}accel_dlpf_rate;

typedef struct 
{
    /* data */
    int16_t Ax_raw; 
    int16_t Ay_raw;
    int16_t Az_raw;
    float Ax;
    float Ay;
    float Az;
}Accel_t;

typedef struct 
{
    int16_t Gx_raw;
    int16_t Gy_raw;
    int16_t Gz_raw;
    float Gx;
    float Gy;
    float Gz;
}Gyro_t;
typedef struct 
{
    uint8_t data[14];
    Accel_t Accel_data;
    Gyro_t Gyro_data;
    Gyro_t Gyro_callib;
    float Roll;
    float Pitch;
    float Yaw;
}mpu9250_t;

void SPI_MPU_EN();
void SPI_MPU_DIS();

mpu_state MPU_WriteRegDMA(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, uint16_t size);
mpu_state MPU_WriteReg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, uint16_t size);
mpu_state MPU_ReadRegDMA(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *pData, uint16_t size);
mpu_state MPU_ReadReg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *pData, uint16_t size);

mpu_state MPU_Init(SPI_HandleTypeDef *hspi, gyro_dlpf_rate gyro_dlpft, accel_fs_t accel_fullscale, gyro_fs_t gyro_fullscale,accel_dlpf_rate accel_dlpf);
mpu_state MPU_InitDMA(SPI_HandleTypeDef *hspi, gyro_dlpf_rate gyro_dlpft, accel_fs_t accel_fullscale, gyro_fs_t gyro_fullscale,accel_dlpf_rate accel_dlpf);
mpu_state MPU_WhoAmI(SPI_HandleTypeDef *hspi, uint8_t *whoami);
mpu_state MPU_WhoAmIDMA(SPI_HandleTypeDef *hspi, uint8_t *data);
mpu_state MPU_Callibrate(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, uint32_t samples, accel_fs_t accell_scale, gyro_fs_t gyro_scale);
mpu_state MPU_CallibrateDMA(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, uint32_t samples);
mpu_state MPU_ReadAll(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, accel_fs_t accel_scale, gyro_fs_t gyro_scale);
mpu_state MPU_ReadAllDMA(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData);
mpu_state MPU_ReadAllDMA_Cplt(mpu9250_t *pMPUData, accel_fs_t accel_scale, gyro_fs_t gyro_scale);
#endif