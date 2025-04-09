#pragma once


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

#include "tlv493.h"



#define TLV_addr  ((uint8_t)0x5E)

// #define bme680  ((uint8_t)0x77)

#define I2C_NODE1 DT_NODELABEL(i2c1) 



class Magnetometer{
    public:
        void init();
        void loop();

    private:
        bool i2cInitialization();
        void i2cScanner();

};