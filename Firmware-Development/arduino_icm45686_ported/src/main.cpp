
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "ICM45686.h"
// #define IMU_INT_PIN 2
// #define IMU_GPIO_NODE DT_NODELABEL(gpio0)


#define I2C_NODE1 DT_NODELABEL(i2c0) 

// const struct device *imu_gpio_dev= DEVICE_DT_GET(IMU_GPIO_NODE);
// static struct gpio_callback imu_cb_data;

LOG_MODULE_REGISTER(Imu, LOG_LEVEL_DBG);  // LOG_LEVEL_DBG shows everything
const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE1);

ICM456xx IMU;



// void imu_int_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
// {
//     printk("IMU interrupt triggered on pin %d\n", IMU_INT_PIN);
// }

int main(){

    // if (!device_is_ready(imu_gpio_dev)) {
    //     printk("GPIO device not ready\n");
    //     return;
    // }

	// gpio_pin_configure(imu_gpio_dev, IMU_INT_PIN, GPIO_INPUT | GPIO_PULL_UP);
    // gpio_pin_interrupt_configure(imu_gpio_dev, IMU_INT_PIN, GPIO_INT_EDGE_TO_ACTIVE);
    // gpio_init_callback(&imu_cb_data, imu_int_handler, BIT(IMU_INT_PIN));
    // gpio_add_callback(imu_gpio_dev, &imu_cb_data);


	
   int ret = IMU.begin(i2c_dev);
	if (ret != 0 ) {
			LOG_INF("[main] IMU initialization failed");
			while(1);
	}
	else{ 	LOG_INF("[main] IMU initialization successfull");	}

    // Accel ODR = 100 Hz and Full Scale Range = 16G
   IMU.startAccel(i2c_dev ,100,16);

	// Gyro ODR = 100 Hz and Full Scale Range = 2000 dps
   IMU.startGyro(i2c_dev ,100,2000);
   k_msleep(100);


	while(1){

   		inv_imu_sensor_data_t imu_data;

		// IMU.enableFifoInterrupt();
		// Read registers
	   IMU.getDataFromRegisters(i2c_dev, imu_data);

		//    // Format data for Serial Plotter
		LOG_INF("AccelX: %d " , imu_data.accel_data[0]);
		LOG_INF("AccelY: %d " , imu_data.accel_data[1]);
		LOG_INF("AccelZ: %d" , imu_data.accel_data[2]);
		LOG_INF("GyroX: %d " , imu_data.gyro_data[0]);
		LOG_INF("GyroY: %d " , imu_data.gyro_data[1]);
		LOG_INF("GyroZ: %d " , imu_data.gyro_data[2]);
		LOG_INF("Temperature: %d " , imu_data.temp_data);

		k_msleep(10);
		
	}


	return 0;


}