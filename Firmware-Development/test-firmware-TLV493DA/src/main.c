#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include "tlv493.h"

#define TLV_addr  ((uint8_t)0x5E)

// #define bme680  ((uint8_t)0x77)

#define I2C_NODE1 DT_NODELABEL(i2c1) 

// uint8_t sens_addr;

const struct device *i2c_dev1 = DEVICE_DT_GET(I2C_NODE1);
TLV493D_dev tlv;

bool i2c_init(){                  
        if(device_is_ready(i2c_dev1) == false){
                printk("I2C1 failed to initialize! \n\r");
                return(false);
         }
        else{
                printk("I2C1 initialized successfully!\n\r");
                return (true);
        }
    
}                      



void i2c1_scanner(void){

 
    printk("Scanning I2C1 bus...\n");

    
    uint8_t data1;

    for (uint8_t addr1= 0x03; addr1 <= 0x77; addr1++) {
        
        
        if (i2c_read(i2c_dev1, &data1, 1, addr1) == 0) {
            printk("Sensor detected at 0x%X Bus:I2C1\n", addr1);
        //     sens_addr = addr1;
        }
    }
    printk("I2C1 scan complete.\n\r");

}

int main(void)
{       
        if(i2c_init() == false){
                return 0;
        }
       
        i2c1_scanner();              /* i2c1 scanner for all three sensors*/
        

        /* Initializing TLV*/
        if(Tlv493d_Reset(&tlv, TLV_addr) == false){
                printk("TLV initialization failed!\n\r");
                return 0;
        }
        printk("TLV initialization successfull!\n\r");

        /*Setting Up TLV*/
        if(Tlv493d_Setup(&tlv ,i2c_dev1) == false){
                printk("TLV setup failed!\n\r");
                return 0 ;
        }
        printk("TLV setup successfull!\n\r");

        /*Date parsing*/
        while(1){
                if(Tlv493d_UpdateData(&tlv ,i2c_dev1)){
                        Tlv493d_GetValue_8bit(&tlv);

                }        

        k_msleep(2000);        
        }
        return 0;
}
