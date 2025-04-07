#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "Magnetometer.hpp"
#include "BLEManager.hpp"
#include "Imu.hpp"
#include "Gnss.hpp"

#define MAIN_TASK_STACK_SIZE 4096
#define MAIN_TASK_PRIORITY 1

#define GNSS_TASK_STACK_SIZE 1024
#define GNSS_TASK_PRIORITY 2

LOG_MODULE_REGISTER(Main, LOG_LEVEL_DBG);  // LOG_LEVEL_DBG shows everything


K_THREAD_STACK_DEFINE(mainStackkSize, MAIN_TASK_STACK_SIZE);
struct k_thread mainTaskThreadState;

K_THREAD_STACK_DEFINE(gnssStackSize, GNSS_TASK_STACK_SIZE);
struct k_thread gnssTaskThreadState;

Magnetometer magneto;
BLEManager ble;
Imu imuu;
Gnss gps;



void mainTask(void *arg1, void *arg2, void *arg3) {
        while (1) {                        
                ble.loop();
                magneto.loop();
                // imuu.loop();

                k_msleep(1000);                        
        }
}

void gnssTask(void *arg1, void *arg2, void *arg3){

        gps.loop();
}


int main(void)
{
        ble.init();
        magneto.init();
        imuu.init();
        gps.init();


        k_thread_create(&mainTaskThreadState, mainStackkSize,
                MAIN_TASK_STACK_SIZE, mainTask,
                NULL, NULL, NULL,
                MAIN_TASK_PRIORITY, 0, K_NO_WAIT);

        
        k_thread_create(&gnssTaskThreadState, gnssStackSize,
                        GNSS_TASK_STACK_SIZE, gnssTask,
                        NULL, NULL, NULL,
                        GNSS_TASK_PRIORITY, 0, K_NO_WAIT);

}
