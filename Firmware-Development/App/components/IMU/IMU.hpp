#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

#include "ICM45686.hpp"

#define IMU_INT_PIN 2

#define I2C_NODE_ICM DT_NODELABEL(i2c0) 
#define IMU_GPIO_NODE DT_NODELABEL(gpio0)



class IMU{
    public:
        void init();
        void loop();


    private:
        void i2cScanner(void);

};