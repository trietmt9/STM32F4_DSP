#include "bmp280.h"
#include "stm32f4xx_hal_def.h"

/**
 * @brief Read BMP280 ID 
 * @param hi2c: pointer to I2C handler 
 * @param id: pointer to variable stored BMP280's id
 * @return BMP280_OK
 */

inline bmp280_status_t BMP280_ID(I2C_HandleTypeDef *phi2c, uint8_t* id)
{
    HAL_I2C_Mem_Read(phi2c, BMP280_I2C_ADDRESS, BMP280_CHIP_ID, 1, id, sizeof(id), HAL_MAX_DELAY);
    return BMP280_OK;
}

/** 
 * @brief Startup BMP280 
 * @param hi2c: pointer to I2C handler 
 * @param spi_operation: select SPI 3-wire or 4-wire mode
 * @param mode: select BMP280 working mode
 * @param oversampling_p: oversampling factor for pressure reading 
 * @param oversampling_t: oversampling factor for temperature reading
 * @param iir_filter: select IIR filter coefficient 
 * @param standby_time: select standby time
 * @return BMP280_OK
 */

bmp280_status_t BMP280_Init(I2C_HandleTypeDef *phi2c, bmp280_SPI_operation_t spi_operation, bmp280_mode_t mode, bmp280_oversample_t oversampling_p, bmp280_oversample_t oversampling_t, bmp280_IIR_filter_t iir_filter, bmp280_standby_time_t standby_time)
{
    uint8_t data, id;
    BMP280_ID(phi2c, &id);

    if(id != BMP280_CHIP_ID)
    {
        return BMP280_ERROR;
    }
    
    /* Reset BMP280 */
    data = 0xB6u;
    HAL_I2C_Mem_Write(phi2c, BMP280_I2C_ADDRESS, BMP280_RESET_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);
    
    /* Setup control measurement */
    data &= 0x00u;
    data |= (mode << 0)|(oversampling_p << 2)|(oversampling_t << 5);
    HAL_I2C_Mem_Write(phi2c, BMP280_I2C_ADDRESS, BMP280_CTRL_MEAS_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);

    /* Configure BMP280 */
    data &= 0x00u;
    data |= (spi_operation << 0)|(iir_filter << 2)|(standby_time << 5);
    HAL_I2C_Mem_Write(phi2c, BMP280_I2C_ADDRESS, BMP280_CONFIG_REG, 1, &data, sizeof(data), HAL_MAX_DELAY);
    return BMP280_OK;
}

/**
 * @brief Conpensation Temperature data 
 * @param pBMP280: pointer to bmp280 struct
 * @param adc_Temperature: analog temperature reading from sensor 
 * @param t_fine: Temperature after filter
 * @return Temp 
 */
int32_t conpensateTemp(bmp280_t *pBMP280, int32_t adc_Temperature, int32_t* t_fine)
{
    int32_t var1, var2 , Temp;
    var1 = ((((adc_Temperature>>3) - (pBMP280->dig_T1<<1))) *(pBMP280->dig_T2))>>11;
    var2 = (((((adc_Temperature>>4) - (pBMP280->dig_T1)) * ((adc_Temperature>>4) - pBMP280->dig_T1)))>>12)*(pBMP280->dig_T3) >> 14;
    *t_fine = var1 + var2;
    Temp = (*t_fine*5+128)>>8;
    return Temp;
}

/**
 * @brief Conpensation Temperature data 
 * @param pBMP280: pointer to bmp280 struct
 * @param adc_Pressure: analog Pressure reading from sensor 
 * @param t_fine: Temperature after filter
 * @return Press 
 */
int32_t conpensatePressure(bmp280_t* pBMP280, int32_t adc_Pressure, int64_t t_fine)
{
    int64_t var1, var2, p;
    var1 = t_fine - 128000;
    var2 = var1 * var1 * pBMP280->dig_P6;
    var2 = var2 +((var1*pBMP280->dig_P5) << 17);
    var2 = var2 +((pBMP280->dig_P4) << 35);
    var1 = ((var1*var1 * pBMP280->dig_P3)>>8) + ((var1 * pBMP280->dig_P2)<<12);
    var1 = ((((1ll << 47)+var1))*pBMP280->dig_P1)>>33;
    if(var1 == 0)
    {
        return 0;
    }
    p = 1048576 - adc_Pressure;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = ((pBMP280->dig_P9)*(p>>13)*(p>>13)) >> 25;
    var2 = ((pBMP280->dig_P8)*p) >> 19;
    p = ((p + var1 +var2)>>8) + ((pBMP280->dig_P7)<<4);
    return (uint32_t)p;
}

/** 
 * @brief Read BMP280 pressure data
 * @param hi2c: pointer to I2C handler 
 * @param pBMP280: pointer to BMP280 struct
 * @return BMP280_OK  
 */
bmp280_status_t BMP280_ReadRaw(I2C_HandleTypeDef *phi2c, bmp280_t *pBMP280)
{
    uint8_t rawdata;
    uint16_t data[13]; 
    HAL_I2C_Mem_Read(phi2c, BMP280_I2C_ADDRESS, BMP280_PRESSURE_LSB_REG, 1, &rawdata, sizeof(rawdata), HAL_MAX_DELAY);
    
    // Reading 20bits format 
    pBMP280->rawPress = (data[0]<<12)|(data[1]<<4)|(data[2]>>4);
    pBMP280->rawTemp  = (data[3]<<12)|(data[4]<<4)|(data[5]>>4);

    return BMP280_OK;
}