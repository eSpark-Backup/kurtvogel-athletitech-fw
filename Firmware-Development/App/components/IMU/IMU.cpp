#include "IMU.hpp"

ICM456xx icm;

LOG_MODULE_REGISTER(Imu, LOG_LEVEL_DBG);  // LOG_LEVEL_DBG shows everything

const struct device *i2cDev = DEVICE_DT_GET(I2C_NODE_ICM);
const struct device *imuGpioDev= DEVICE_DT_GET(IMU_GPIO_NODE);
static struct gpio_callback imuCallbackData;

bool imuCallbackFlag= false;
static uint8_t addr;
static uint8_t foundAddress= 0x00;


void imuCallback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	imuCallbackFlag=true;
    LOG_INF("IMU interrupt triggered on pin %d\n", IMU_INT_PIN);
}


void IMU::i2cScanner(void){

    if (!device_is_ready(i2cDev)) {        /* i2c0 scanner for icm*/
        LOG_INF("[i2cScanner] I2C0 ready !");
        return;
    }

    LOG_WRN("[i2cScanner] Scanning I2C0 bus...");

    
    uint8_t data0;

    for (addr = 0x03; addr <= 0x77; addr++) {
        
        
        if (i2c_read(i2cDev, &data0, 1, addr) == 0) {
            LOG_INF("[i2cScanner] Sensor detected at 0x%X Bus:I2C0", addr);
            foundAddress = addr;
        }
    }
    LOG_WRN("[i2cScanner] I2C0 scan complete");

}




void IMU::init(){

    i2cScanner();         /* i2c scanner for icm*/

    if (!device_is_ready(imuGpioDev)) {
        LOG_INF("[main] GPIO device not ready");
        return  ;
    }


  	int ret = icm.begin(i2cDev);
	if (ret != 0 ) {
			LOG_INF("[main] IMU initialization failed");
			while(1);
	}
	else{ 	LOG_INF("[main] IMU initialization successfull");	}


	icm.enableFifoInterrupt(imuGpioDev , i2cDev , IMU_INT_PIN , imuCallback  , &imuCallbackData, 10);
    // Accel ODR = 100 Hz and Full Scale Range = 16G
    icm.startAccel(i2cDev ,100,16);
	// Gyro ODR = 100 Hz and Full Scale Range = 2000 dps
    icm.startGyro(i2cDev ,100,2000);
    
}

void IMU::loop(){

        inv_imu_sensor_data_t imuData;
        icm.getDataFromRegisters(i2cDev, imuData);

        LOG_INF("AccelX: %d " , imuData.accel_data[0]);
        LOG_INF("AccelY: %d " , imuData.accel_data[1]);
        LOG_INF("AccelZ: %d" , imuData.accel_data[2]);
        LOG_INF("GyroX: %d " , imuData.gyro_data[0]);
        LOG_INF("GyroY: %d " , imuData.gyro_data[1]);
        LOG_INF("GyroZ: %d " , imuData.gyro_data[2]);
        LOG_INF("Temperature: %d " , imuData.temp_data);

        //FIFO
		 if(imuCallbackFlag == true){
			imuCallbackFlag=false;
			for(int i = 0; i < 10 ; i ++)
			{
				inv_imu_fifo_data_t imuFifoData;				
				icm.getDataFromFifo(i2cDev, imuFifoData);				
				LOG_INF("AccelX: %d " , imuFifoData.byte_16.accel_data[0]);
				LOG_INF("AccelY: %d " , imuFifoData.byte_16.accel_data[1]);
				LOG_INF("AccelZ: %d" , imuFifoData.byte_16.accel_data[2]);
				LOG_INF("GyroX: %d " , imuFifoData.byte_16.gyro_data[0]);
				LOG_INF("GyroY: %d " , imuFifoData.byte_16.gyro_data[1]);
				LOG_INF("GyroZ: %d " , imuFifoData.byte_16.gyro_data[2]);
				LOG_INF("Temperature: %d " , imuFifoData.byte_16.temp_data);
			}

		}  
		k_msleep(10);
         
}



// /*ICM REGISTER ADDRESSES*/
// #define		    ACCEL_DATA_X1_UI  0x00
// #define			PWR_MGMT0         0x10
// #define			ACCEL_CONFIG      0x18
// #define			GYRO_CONFIG 	  0x19
// #define			WHO_AM_I   		  0x72


// #define CHIP_ID 0xD0
// #define BME680_ADDR 0x76

// // Register addresses
// #define ICM45686_ADDR 0x69  // Adjust if needed
// #define WHO_AM_I_REG  0x72  // WHO_AM_I register
// #define ACCEL_REG     0x1B  // Acceleration data register
// #define GYRO_REG      0x1C  // Gyroscope data register
