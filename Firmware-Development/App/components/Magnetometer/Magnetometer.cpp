#include "Magnetometer.hpp"


LOG_MODULE_REGISTER(Magnetometerr, LOG_LEVEL_DBG);  // LOG_LEVEL_DBG shows everything


const struct device *i2c_dev1 = DEVICE_DT_GET(I2C_NODE1);
TLV493D_dev tlv;

bool Magnetometer::i2cInitialization(){                  
        if(device_is_ready(i2c_dev1) == false){
                LOG_ERR("[i2cInitialization] I2C1 failed to initialize!");
                return(false);
         }
        else{
                LOG_INF("[i2cInitialization] I2C1 initialized successfully!");
                return (true);
        }
    
}                      



void Magnetometer::i2cScanner(void){

 
    LOG_WRN("[i2cScanner] Scanning I2C1 bus...");

    uint8_t addr1;
    uint8_t data1;

    for (addr1= 0x03; addr1 <= 0x77; addr1++) {
        
        
        if (i2c_read(i2c_dev1, &data1, 1, addr1) == 0) {
                LOG_INF("[i2cScanner] Sensor detected at 0x%X Bus:I2C1", addr1);
        //     sens_addr = addr1;
        }
    }

    LOG_WRN("[i2cScanner] I2C1 scan complete.", addr1);

}



void Magnetometer::init(){

        if(i2cInitialization() == false){
                return ;
        }

        i2cScanner();              /* i2c1 scanner for all three sensors*/


        /* Initializing TLV*/
        if(Tlv493d_Reset(&tlv, TLV_addr) == false){
                LOG_ERR("[init] TLV initialization failed!");
                return ;
        }
        LOG_INF("[init] TLV initialization Success");

        /*Setting Up TLV*/
        if(Tlv493d_Setup(&tlv ,i2c_dev1) == false){
                LOG_ERR("[init] TLV setup failed!");
                return ;
        }
        LOG_INF("[init] TLV setup successfull! ");

}


void Magnetometer::loop(){


        if(Tlv493d_UpdateData(&tlv ,i2c_dev1)){
                Tlv493d_GetValue_8bit(&tlv);

        } 


}
