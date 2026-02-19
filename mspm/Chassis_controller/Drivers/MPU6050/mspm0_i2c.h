#ifndef _MSPM0_I2C_H_
#define _MSPM0_I2C_H_

#include "ti_msp_dl_config.h"

#define GPIO_I2C_MPU6050_IOMUX_SDA GPIO_COMM_I2C_IOMUX_SDA
#define GPIO_I2C_MPU6050_IOMUX_SCL GPIO_COMM_I2C_IOMUX_SCL
#define GPIO_I2C_MPU6050_IOMUX_SCL_FUNC GPIO_COMM_I2C_IOMUX_SCL_FUNC
#define GPIO_I2C_MPU6050_IOMUX_SDA_FUNC GPIO_COMM_I2C_IOMUX_SDA_FUNC
#define GPIO_I2C_MPU6050_SCL_PORT GPIO_COMM_I2C_SCL_PORT
#define GPIO_I2C_MPU6050_SCL_PIN GPIO_COMM_I2C_SCL_PIN
#define GPIO_I2C_MPU6050_SDA_PORT GPIO_COMM_I2C_SDA_PORT
#define GPIO_I2C_MPU6050_SDA_PIN GPIO_COMM_I2C_SDA_PIN
#define I2C_MPU6050_INST COMM_I2C_INST

#define SYSCFG_DL_I2C_MPU6050_init SYSCFG_DL_COMM_I2C_init

void mpu6050_i2c_sda_unlock(void);

int mspm0_i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data);

int mspm0_i2c_read(unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data);

#endif  /* #ifndef _MSPM0_I2C_H_ */
