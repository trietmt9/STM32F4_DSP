/**
 * @file    mpu9250.c
 * @brief   MPU9250 driver implementation
 * @author  Stephen Truong
 * @date    2025-03-21
 */
#include "mpu9250.h"

/**
 * @brief Enable MPU9250
 * @param None
 * @retval None
 */
inline void SPI_MPU_EN()
{
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port, MPU_CS_Pin, GPIO_PIN_RESET);
}

/**
 * @brief Disable MPU9250
 * @param None
 * @retval None
 */
inline void SPI_MPU_DIS()
{
    HAL_GPIO_WritePin(MPU_CS_GPIO_Port, MPU_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief Read MPU9250 register
 * @param hspi: SPI handler
 * @param reg: register address
 * @param data: data buffer
 * @param size: size of data buffer
 * @retval MPU9250_OK
 */
inline mpu_state MPU_ReadReg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *pData, uint16_t size)
{   
    SPI_MPU_EN();
    uint8_t read_reg = reg | 0x80u;
    HAL_SPI_Transmit(hspi, &read_reg, 1, MPU9250_SPI_TIMEOUT);
    HAL_SPI_Receive(hspi, pData, size, MPU9250_SPI_TIMEOUT);
    SPI_MPU_DIS();
    return MPU9250_OK;
}

/**
 * @brief Read MPU9250 register in DMA mode
 * @param hspi: SPI handler
 * @param reg: register address
 * @param data: data buffer
 * @param size: size of data buffer
 * @retval MPU9250_OK
 */
inline mpu_state MPU_ReadRegDMA(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *pData, uint16_t size)
{   
    //reset DMA transfer complete flag
    uint8_t read_reg = reg | 0x80u; 
    // start DMA transfer
    SPI_MPU_EN();
    HAL_SPI_Transmit_DMA(hspi, &read_reg, 1);
    HAL_SPI_Receive_DMA(hspi, pData, size);
    return MPU9250_OK;
}

/**
 * @brief Write MPU9250 register
 * @param hspi: SPI handler
 * @param reg: register address
 * @param data: data buffer
 * @param size: size of data buffer
 * @retval MPU9250_OK
 */
inline mpu_state MPU_WriteReg(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, uint16_t size)
{
    SPI_MPU_EN();
    HAL_SPI_Transmit(hspi, &reg, 1, MPU9250_SPI_TIMEOUT);
    HAL_SPI_Transmit(hspi, data, size, MPU9250_SPI_TIMEOUT);
    SPI_MPU_DIS();
    return MPU9250_OK;
}

/**
 * @brief Write MPU9250 register
 * @param hspi: SPI handler
 * @param reg: register address
 * @param data: data buffer
 * @param size: size of data buffer
 * @retval MPU9250_OKDMA transfer and
 */
inline mpu_state MPU_WriteRegDMA(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t *data, uint16_t size)
{
    // start DMA transfer
    SPI_MPU_EN();
    HAL_SPI_Transmit_DMA(hspi, &reg, 1);
    HAL_SPI_Transmit_DMA(hspi, data, size);
    SPI_MPU_DIS();
    return MPU9250_OK;
}

/**
 * @brief Get MPU9250 WHO_AM_I register
 * @param hspi: SPI handler
 * @param data: data buffer
 * @retval MPU9250_OK
 */

mpu_state MPU_WhoAmI(SPI_HandleTypeDef *hspi, uint8_t *data)
{
    MPU_ReadReg(hspi, WHO_AM_I, data, 1);
    return MPU9250_OK;
}

/**
 * @brief Get MPU9250 WHO_AM_I register DMA mode
 * @param hspi: SPI handler
 * @param data: data buffer
 * @retval MPU9250_OK
 */

 mpu_state MPU_WhoAmIDMA(SPI_HandleTypeDef *hspi, uint8_t *data)
 {
     MPU_ReadRegDMA(hspi, WHO_AM_I, data, 1);
     return MPU9250_OK;
 }

/**
 * @brief Initialize MPU9250
 * @param hspi: SPI handler
 * @param gyro_dlpft: gyro low pass filter rate
 * @param accel_fullscale: accelerometer full scale range
 * @param gyro_fullscale: gyroscope full scale range
 * @param accel_dlpf: accelerometer low pass filter rate
 * @retval MPU9250_OK
 */

mpu_state MPU_Init(SPI_HandleTypeDef *hspi, gyro_dlpf_rate gyro_dlpft, accel_fs_t accel_fullscale, gyro_fs_t gyro_fullscale,accel_dlpf_rate accel_dlpf)
{
    uint8_t data;
    uint8_t whoami;
    // Check if device is connected 
    MPU_WhoAmI(hspi, &whoami);
    if (whoami != MPU9250_SPI_ID)
    {
        return MPU9250_ERROR;
    }
    else 
    {
        // Reset MPU9250
        data = 0x80U;
        if (MPU_WriteReg(hspi, PWR_MGMT_1, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
        HAL_Delay(100);

        // Wake, use PLL with gyro reference
        data = 0x01U;
        if (MPU_WriteReg(hspi, PWR_MGMT_1, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
        HAL_Delay(10);

        // Wakeup
        data = 0x00U;
        if (MPU_WriteReg(hspi, PWR_MGMT_2, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
        HAL_Delay(10);

        // Sample rate: 1 kHz
        data = 0;
        if (MPU_WriteReg(hspi, SMPLRT_DIV, &data, 1) != MPU9250_OK) return MPU9250_ERROR;

        // Gyro DLPF
        data = gyro_dlpft;
        if (MPU_WriteReg(hspi, CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;

        // Gyro full scale
        data = gyro_fullscale << 3;
        if (MPU_WriteReg(hspi, GYRO_CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;

        // Accel full scale
        data = accel_fullscale << 3;
        if (MPU_WriteReg(hspi, ACCEL_CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;

        // Accel DLPF
        data = accel_dlpf;
        if (MPU_WriteReg(hspi, ACCEL_CONFIG2, &data, 1) != MPU9250_OK) return MPU9250_ERROR;

        return MPU9250_OK;
    }
}

/**
 * @brief Initialize MPU9250
 * @param hspi: SPI handler
 * @param gyro_dlpft: gyro low pass filter rate
 * @param accel_fullscale: accelerometer full scale range
 * @param gyro_fullscale: gyroscope full scale range
 * @param accel_dlpf: accelerometer low pass filter rate
 * @retval MPU9250_OK
 */

 mpu_state MPU_InitDMA(SPI_HandleTypeDef *hspi, gyro_dlpf_rate gyro_dlpft, accel_fs_t accel_fullscale, gyro_fs_t gyro_fullscale,accel_dlpf_rate accel_dlpf)
 {
     uint8_t data;
     // Reset MPU9250
     data = 0x80U;
     if (MPU_WriteRegDMA(hspi, PWR_MGMT_1, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
     HAL_Delay(100);
 
     // Wake, use PLL with gyro reference
     data = 0x01U;
     if (MPU_WriteRegDMA(hspi, PWR_MGMT_1, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
     HAL_Delay(10);
 
     // Wakeup
     data = 0x00U;
     if (MPU_WriteRegDMA(hspi, PWR_MGMT_2, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
     HAL_Delay(10);
 
     // Sample rate: 1 kHz
     data = 0;
     if (MPU_WriteRegDMA(hspi, SMPLRT_DIV, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
 
     // Gyro DLPF
     data = gyro_dlpft;
     if (MPU_WriteRegDMA(hspi, CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
 
     // Gyro full scale
     data = gyro_fullscale << 3;
     if (MPU_WriteRegDMA(hspi, GYRO_CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
 
     // Accel full scale
     data = accel_fullscale << 3;
     if (MPU_WriteRegDMA(hspi, ACCEL_CONFIG, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
 
     // Accel DLPF
     data = accel_dlpf;
     if (MPU_WriteRegDMA(hspi, ACCEL_CONFIG2, &data, 1) != MPU9250_OK) return MPU9250_ERROR;
 
     return MPU9250_OK;
 }

/**
 * @brief Calibrate MPU9250
 * @param hspi: SPI handler
 * @param pMPUData: IMU data structure
 * @param samples: number of samples
 * @param accell_scale: accelerometer scale
 * @param gyro_scale: gyroscope scale
 * @retval MPU9250_OK
 */
mpu_state MPU_Callibrate(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, uint32_t samples, accel_fs_t accell_scale, gyro_fs_t gyro_scale)
{
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;

    // Zero guard
    if (samples == 0)
    {
        samples = 1;
    }

    // Save number of samples 
    for(uint16_t i = 0; i < samples; i++)
    {
        MPU_ReadAll(hspi, pMPUData, accell_scale, gyro_scale);
        x += pMPUData->Gyro_data.Gx_raw;
        y += pMPUData->Gyro_data.Gy_raw;
        z += pMPUData->Gyro_data.Gz_raw;
        HAL_Delay(3);
    }

    // Adverage gyro offset
    pMPUData->Gyro_callib.Gx = (float)x / samples;
    pMPUData->Gyro_callib.Gy = (float)y / samples;
    pMPUData->Gyro_callib.Gz = (float)z / samples;

    return MPU9250_OK;
}

/**
 * @brief Calibrate MPU9250 DMA mode
 * @param hspi: SPI handler
 * @param pMPUData: IMU data structure
 * @param samples: number of samples
 * @param accell_scale: accelerometer scale
 * @param gyro_scale: gyroscope scale
 * @retval MPU9250_OK
 */
mpu_state MPU_CallibrateDMA(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, uint32_t samples)
{
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;

    // Zero guard
    if (samples == 0)
    {
        samples = 1;
    }

    // Save number of samples 
    for(uint16_t i = 0; i < samples; i++)
    {
        MPU_ReadAllDMA(hspi, pMPUData);
        x += pMPUData->Gyro_data.Gx_raw;
        y += pMPUData->Gyro_data.Gy_raw;
        z += pMPUData->Gyro_data.Gz_raw;
        HAL_Delay(3);
    }

    // Adverage gyro offset
    pMPUData->Gyro_callib.Gx = (float)x / samples;
    pMPUData->Gyro_callib.Gy = (float)y / samples;
    pMPUData->Gyro_callib.Gz = (float)z / samples;

    return MPU9250_OK;
}

/** 
 * @brief Read MPU9250 data
 * @param hspi: SPI handler
 * @param pIMU: IMU data structure
 * @param accel_scale: accelerometer scale
 * @param gyro_scale: gyroscope scale
 * @retval MPU9250_OK
 */
mpu_state MPU_ReadAll(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData, accel_fs_t accel_scale, gyro_fs_t gyro_scale)
{   
    if (MPU_ReadReg(hspi, ACCEL_XOUT_H, pMPUData->data, 14) == MPU9250_OK)
    {
        float a_scale, g_scale;

        //* Scale factor for accelerometer
        switch (accel_scale) {
            case accel_fs_2g: a_scale = 2.0f / 32768.0f; break;
            case accel_fs_4g: a_scale = 4.0f / 32768.0f; break;
            case accel_fs_8g: a_scale = 8.0f / 32768.0f; break;
            case accel_fs_16g: a_scale = 16.0f / 32768.0f; break;
        }

        //TODO: Read Accelerometer data
        pMPUData->Accel_data.Ax_raw = (((int16_t)pMPUData->data[0]) << 8) | pMPUData->data[1];
        pMPUData->Accel_data.Ay_raw = (((int16_t)pMPUData->data[2]) << 8) | pMPUData->data[3];
        pMPUData->Accel_data.Az_raw = (((int16_t)pMPUData->data[4]) << 8) | pMPUData->data[5];

        //TODO: Convert to real data
        pMPUData->Accel_data.Ax = (int16_t)pMPUData->Accel_data.Ax_raw * a_scale;
        pMPUData->Accel_data.Ay = (int16_t)pMPUData->Accel_data.Ay_raw * a_scale;
        pMPUData->Accel_data.Az = (int16_t)pMPUData->Accel_data.Az_raw * a_scale;

        //* Scale factor for gyroscope
        switch (gyro_scale) {
            case gyro_fs_250dps:  g_scale = 250.0f / 32768.0f; break;
            case gyro_fs_500dps:  g_scale = 500.0f / 32768.0f; break;
            case gyro_fs_1000dps: g_scale = 1000.0f / 32768.0f; break;
            case gyro_fs_2000dps: g_scale = 2000.0f / 32768.0f; break;
            default: g_scale = 250.0f / 32768.0f; break;
        }

        //TODO: Read Gyroscope data
        pMPUData->Gyro_data.Gx_raw = (((int16_t)pMPUData->data[8] ) << 8) | pMPUData->data[9];
        pMPUData->Gyro_data.Gy_raw = (((int16_t)pMPUData->data[10]) << 8) | pMPUData->data[11];
        pMPUData->Gyro_data.Gz_raw = (((int16_t)pMPUData->data[12]) << 8) | pMPUData->data[13];

        //TODO: Calibrate Gyroscope data
        pMPUData->Gyro_data.Gx = (int16_t)pMPUData->Gyro_data.Gx_raw - pMPUData->Gyro_callib.Gx;
        pMPUData->Gyro_data.Gy = (int16_t)pMPUData->Gyro_data.Gy_raw - pMPUData->Gyro_callib.Gx;
        pMPUData->Gyro_data.Gz = (int16_t)pMPUData->Gyro_data.Gz_raw - pMPUData->Gyro_callib.Gx;

        // TODO: Convert to real data
        pMPUData->Gyro_data.Gx *= g_scale;
        pMPUData->Gyro_data.Gy *= g_scale;
        pMPUData->Gyro_data.Gz *= g_scale;

        pMPUData->Pitch = atan2f(-pMPUData->Accel_data.Ax, sqrtf((pMPUData->Accel_data.Ay * pMPUData->Accel_data.Ay) + (pMPUData->Accel_data.Az * pMPUData->Accel_data.Az))) * RAD_TO_DEG;
        pMPUData->Roll = atan2f(pMPUData->Accel_data.Ay, pMPUData->Accel_data.Az) * RAD_TO_DEG;
    }  
    return MPU9250_OK;
}

/** 
 * @brief Read MPU9250 data DMA mode
 * @param hspi: SPI handler
 * @param reg: register address
 * @param data: data buffer
 * @param size: size of data buffer
 * @retval MPU9250_OK
 */
mpu_state MPU_ReadAllDMA(SPI_HandleTypeDef *hspi, mpu9250_t *pMPUData)
{   
    if (MPU_ReadRegDMA(hspi, ACCEL_XOUT_H, pMPUData->data, 14) == MPU9250_OK)
    {
        return MPU9250_OK;
    } 
    else
    {
        return MPU9250_ERROR;
    }
}

mpu_state MPU_ReadAllDMA_Cplt(mpu9250_t *pMPUData, accel_fs_t accel_scale, gyro_fs_t gyro_scale)
{
    SPI_MPU_DIS();
    float a_scale, g_scale;

        //* Scale factor for accelerometer
        switch (accel_scale) {
            case accel_fs_2g: a_scale = 2.0f / 32768.0f; break;
            case accel_fs_4g: a_scale = 4.0f / 32768.0f; break;
            case accel_fs_8g: a_scale = 8.0f / 32768.0f; break;
            case accel_fs_16g: a_scale = 16.0f / 32768.0f; break;
        }

        //TODO: Read Accelerometer data
        pMPUData->Accel_data.Ax_raw = (((int16_t)pMPUData->data[0]) << 8) | pMPUData->data[1];
        pMPUData->Accel_data.Ay_raw = (((int16_t)pMPUData->data[2]) << 8) | pMPUData->data[3];
        pMPUData->Accel_data.Az_raw = (((int16_t)pMPUData->data[4]) << 8) | pMPUData->data[5];

        //TODO: Convert to real data
        pMPUData->Accel_data.Ax = (int16_t)pMPUData->Accel_data.Ax_raw * a_scale;
        pMPUData->Accel_data.Ay = (int16_t)pMPUData->Accel_data.Ay_raw * a_scale;
        pMPUData->Accel_data.Az = (int16_t)pMPUData->Accel_data.Az_raw * a_scale;

        //* Scale factor for gyroscope
        switch (gyro_scale) {
            case gyro_fs_250dps:  g_scale = 250.0f / 32768.0f; break;
            case gyro_fs_500dps:  g_scale = 500.0f / 32768.0f; break;
            case gyro_fs_1000dps: g_scale = 1000.0f / 32768.0f; break;
            case gyro_fs_2000dps: g_scale = 2000.0f / 32768.0f; break;
            default: g_scale = 250.0f / 32768.0f; break;
        }

        //TODO: Read Gyroscope data
        pMPUData->Gyro_data.Gx_raw = (((int16_t)pMPUData->data[8] ) << 8) | pMPUData->data[9];
        pMPUData->Gyro_data.Gy_raw = (((int16_t)pMPUData->data[10]) << 8) | pMPUData->data[11];
        pMPUData->Gyro_data.Gz_raw = (((int16_t)pMPUData->data[12]) << 8) | pMPUData->data[13];

        //TODO: Calibrate Gyroscope data
        pMPUData->Gyro_data.Gx = (int16_t)pMPUData->Gyro_data.Gx_raw - pMPUData->Gyro_callib.Gx;
        pMPUData->Gyro_data.Gy = (int16_t)pMPUData->Gyro_data.Gy_raw - pMPUData->Gyro_callib.Gx;
        pMPUData->Gyro_data.Gz = (int16_t)pMPUData->Gyro_data.Gz_raw - pMPUData->Gyro_callib.Gx;

        // TODO: Convert to real data
        pMPUData->Gyro_data.Gx *= g_scale;
        pMPUData->Gyro_data.Gy *= g_scale;
        pMPUData->Gyro_data.Gz *= g_scale;

        pMPUData->Pitch = atan2f(-pMPUData->Accel_data.Ax, sqrtf((pMPUData->Accel_data.Ay * pMPUData->Accel_data.Ay) + (pMPUData->Accel_data.Az * pMPUData->Accel_data.Az))) * RAD_TO_DEG;
        pMPUData->Roll = atan2f(pMPUData->Accel_data.Ay, pMPUData->Accel_data.Az) * RAD_TO_DEG;

        return MPU9250_OK;
}