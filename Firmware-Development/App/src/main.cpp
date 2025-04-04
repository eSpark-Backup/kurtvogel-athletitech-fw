#include <zephyr/kernel.h>
#include "magnetometer.hpp"
#include "BLEManager.hpp"
#include "Imu.hpp"
#include "Gnss.hpp"

Magnetometer magneto;
BLEManager ble;
Imu imuu;


int main(void)
{
        
        ble.init();
        magneto.init();
        imuu.init();

        while(1){
                ble.loop();
                magneto.loop();
                imuu.loop();

                k_msleep(1000);                        
        }

        return 0;
}
