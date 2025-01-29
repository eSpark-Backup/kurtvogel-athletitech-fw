#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define I2C_NODE DT_NODELABEL(i2c0) // Use i2c1 if needed
// #define CHIP_ID 0xD0
// #define BME680_ADDR 0x76

// Register addresses
#define ICM45686_ADDR 0x69  // Adjust if needed
#define WHO_AM_I_REG  0x72  // WHO_AM_I register
#define ACCEL_REG     0x1B  // Acceleration data register
#define GYRO_REG      0x1C  // Gyroscope data register


const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

uint8_t read_register(uint8_t reg) {
    uint8_t ret;
    uint8_t data;
    ret=i2c_write_read(i2c_dev, ICM45686_ADDR, &reg, 1, &data, 1);
    if (ret<0){
        printk("Failed to read WHO_AM_I register\n\r");
    }
    else{
        printk("Successfully data read from WHO_AM_I register\n\r");
        return data;
    }
    
}

void fetch_data() {
    int16_t accel_x=0, accel_y=0, accel_z=0, gyro_x=0, gyro_y=0, gyro_z=0;
    uint8_t buffer[6];
    uint8_t ret1,ret2;

    ret1 = i2c_write_read(i2c_dev, ICM45686_ADDR, (uint8_t[]){ACCEL_REG}, 1, buffer, 6);
       
        printk("Buffer: ");
        for (int i = 0; i < 6; i++) {
            printk("%02X", buffer[i]);  // Print each byte as hex
        }
        printk("\n");

        if(ret1<0){
            printk("failed to read data form ACCEL_REG \n\r");
        }
        else{
            printk("Successfull data fetch from ACCEL_REG\n\r");
            accel_x = (buffer[0] << 8) | buffer[1];
            accel_y = (buffer[2] << 8) | buffer[3];
            accel_z = (buffer[4] << 8) | buffer[5];
        }

    ret2 = i2c_write_read(i2c_dev, ICM45686_ADDR, (uint8_t[]){GYRO_REG}, 1, buffer, 6);
       
        printk("Buffer2: ");
        for (int i = 0; i < 6; i++) {
            printk("%02X", buffer[i]);  // Print each byte as hex
        }
        printk("\n");

        if(ret2<0){
            printk("failed to read data form ACCEL_REG \n\r");
        }
        else{    
            printk("Successfull data fetch from GYRO_REG\n\r");
            gyro_x = (buffer[0] << 8) | buffer[1];
            gyro_y = (buffer[2] << 8) | buffer[3];
            gyro_z = (buffer[4] << 8) | buffer[5];
        }

    printk("Accel: X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);
    printk("Gyro: X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
}

void main(void) {
    
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found!\n");
        return;
    }

    printk("Scanning I2C bus...\n");

    uint8_t found_address=0x00;
    uint8_t data;

    for (uint8_t addr = 0x03; addr <= 0x77; addr++) {
        
        
        if (i2c_read(i2c_dev, &data, 1, addr) == 0) {
            printk("Sensor detected at 0x%X\n", addr);
            found_address = addr;
        }
    }


    printk("I2C scan complete.\n\r");
        if(found_address == ICM45686_ADDR){
            printk("Sensor found : ICM45686\n\r");
            printk("Initializing ICM45686\n\r");

            uint8_t who_am_i = read_register(WHO_AM_I_REG);
            printk("WHO_AM_I: 0x%X\n", who_am_i);

            while (1) {
                fetch_data();
                k_sleep(K_MSEC(500));
                 }
        }
        else{
            printk("ICM not found.... other sensor detected\n\r");
        }
}

   
// static uint8_t verify_connection_(){
//     uint8_t product_id = 0x00;
// 	uint8_t error_count = 0;

// 	while(product_id != 0x41){

// 		ICM45686_HAL::read(ICM45686_HAL::REGISTER::WHO_AM_I, &product_id, 1);
// 		error_count ++;

// 		if(error_count >= 100){

// 			while(1){

// 			}
// 		}
// 	}

// 	return 0;
// }


// enum class REGISTER: uint8_t{

// 			ACCEL_DATA_X1_UI = 0x00,
// 			PWR_MGMT0        = 0x10,
// 			ACCEL_CONFIG     = 0x18,
// 			GYRO_CONFIG 	 = 0x19,
// 			WHO_AM_I   		 = 0x72
// 		};

// 		ICM45686_HAL(I2C_HandleTypeDef* i2c_pin);

// 		void write(ICM45686_HAL::REGISTER, uint8_t* command, uint8_t len);
// 		void read(ICM45686_HAL::REGISTER, uint8_t* receive_buffer, uint8_t len);


// static uint8_t verify_connection(){
//         uint8_t product_id = 0x00;
//         uint8_t error_count = 0;


//     while(product_id != 0x41){

//         i2c_read(i2c_dev,&product_id,1,WHO_AM_I);
// 		error_count ++;
//         if(error_count >= 100){

// 			while(1){

// 			}
// 		}
//     }
//     return 0;
// }


// /*ICM REGISTER ADDRESSES*/
// #define		    ACCEL_DATA_X1_UI  0x00
// #define			PWR_MGMT0         0x10
// #define			ACCEL_CONFIG      0x18
// #define			GYRO_CONFIG 	  0x19
// #define			WHO_AM_I   		  0x72
