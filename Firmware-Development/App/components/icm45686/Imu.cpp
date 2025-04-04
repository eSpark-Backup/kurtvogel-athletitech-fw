#include "Imu.hpp"



const struct device *i2cDev = DEVICE_DT_GET(I2C_NODE0);
const struct device *i2cDev1 = DEVICE_DT_GET(I2C_NODE1);

static uint8_t addr0;
static uint8_t foundAddress0= 0x00;


uint8_t Imu::readRegister(uint8_t reg) {
    uint8_t ret;
    uint8_t data;
    ret = i2c_write_read(i2cDev, ICM45686_ADDR, &reg, 1, &data, 1);
    if (ret<0){
        printk("Failed to read WHO_AM_I register\n\r");
    }
    else{
        printk("Successfully data read from WHO_AM_I register\n\r");
        return data;
    }
    
}

void Imu::fetchData() {
    int16_t accel_x=0, accel_y=0, accel_z=0, gyro_x=0, gyro_y=0, gyro_z=0;
    uint8_t buffer[6];
    uint8_t ret1,ret2;

    ret1 = i2c_write_read(i2cDev, ICM45686_ADDR, (uint8_t*)ACCEL_REG, 1, buffer, 6);

       
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

        ret2 = i2c_write_read(i2cDev, ICM45686_ADDR, (uint8_t*)GYRO_REG, 1, buffer, 6);

       
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


void Imu::i2cScanner0(void){

    if (!device_is_ready(i2cDev)) {        /* i2c0 scanner for icm*/
        printk("I2C0 ready !\n");
        return;
    }

    printk("Scanning I2C0 bus...\n");

    
    uint8_t data0;

    for (addr0 = 0x03; addr0 <= 0x77; addr0++) {
        
        
        if (i2c_read(i2cDev, &data0, 1, addr0) == 0) {
            printk("Sensor detected at 0x%X Bus:I2C0\n", addr0);
            foundAddress0 = addr0;
        }
    }
    printk("I2C0 scan complete.\n\r");

}

void Imu::i2cScanner1(void){

    if (!device_is_ready(i2cDev1)) {               
        printk("I2C1 ready !\n");
        return;
    }

    printk("Scanning I2C1 bus...\n");

    
    uint8_t data1;

    for (uint8_t addr1= 0x03; addr1 <= 0x77; addr1++) {
        
        
        if (i2c_read(i2cDev1, &data1, 1, addr1) == 0) {
            printk("Sensor detected at 0x%X Bus:I2C1\n", addr1);
            
        }
    }
    printk("I2C1 scan complete.\n\r");

}


void Imu::init(){

    i2cScanner0();         /* i2c0 scanner for icm*/
    i2cScanner1();              /* i2c1 scanner for all three sensors*/
 
    
    if(foundAddress0 == ICM45686_ADDR){
            printk("ICM45686 found\n\r");
            printk("Initializing ICM45686\n\r");

            uint8_t who_am_i = readRegister(WHO_AM_I_REG);
            printk("WHO_AM_I: 0x%X\n", who_am_i);
     }
        else{
            printk("ICM not found....\n\r");
        }

}

void Imu::loop(){

        fetchData();
         
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

//         i2c_read(i2cDev,&product_id,1,WHO_AM_I);
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