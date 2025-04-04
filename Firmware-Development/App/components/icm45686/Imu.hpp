#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>


#define I2C_NODE0 DT_NODELABEL(i2c0) // Use i2c1 if needed
#define I2C_NODE1 DT_NODELABEL(i2c1) 

// #define CHIP_ID 0xD0
// #define BME680_ADDR 0x76

// Register addresses
#define ICM45686_ADDR 0x69  // Adjust if needed
#define WHO_AM_I_REG  0x72  // WHO_AM_I register
#define ACCEL_REG     0x1B  // Acceleration data register
#define GYRO_REG      0x1C  // Gyroscope data register


class Imu{
    public:
        void init();
        void loop();


    private:
        void i2cScanner1(void);
        void i2cScanner0(void);
        void fetchData();
        uint8_t readRegister(uint8_t reg);

};