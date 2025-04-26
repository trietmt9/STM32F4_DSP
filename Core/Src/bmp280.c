#include "bmp280.h"
#include "stm32f4xx_hal_def.h"

inline bmp280_status_t BMP280_ID(I2C_HandleTypeDef *hi2c, uint8_t* id)
{
    HAL_I2C_Mem_Read(hi2c, BMP280_I2C_ADDRESS, BMP280_CHIP_ID, 1, id, sizeof(id), HAL_MAX_DELAY);
    return BMP280_OK;
}

bmp280_status_t BMP280_Init(I2C_HandleTypeDef *hi2c, bmp280_SPI_operation_t spi_operation, bmp280_mode_t mode, bmp280_oversample_t oversampling_p, bmp280_oversample_t oversampling_t, bmp280_IIR_filter_t iir_filter, bmp280_standby_time_t standby_time)
{
    uint8_t data, id;
    BMP280_ID(hi2c, &id);

    if(id != BMP280_CHIP_ID)
    {
        return BMP280_ERROR;
    }
    
    /* Reset BMP280 */
    data = 0xB6u;
    HAL_I2C_Mem_Write(hi2c, BMP280_I2C_ADDRESS, BMP280_RESET_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);
    
    /* Setup control measurement */
    data &= 0x00u;
    data |= (mode << 0)|(oversampling_p << 2)|(oversampling_t << 5);
    HAL_I2C_Mem_Write(hi2c, BMP280_I2C_ADDRESS, BMP280_CTRL_MEAS_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);

    /* Configure BMP280 */
    data &= 0x00u;
    data |= (spi_operation << 0)|(iir_filter << 2)|(standby_time << 5);
    HAL_I2C_Mem_Write(hi2c, BMP280_I2C_ADDRESS, BMP280_CONFIG_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);
    return BMP280_OK;
}
