#pragma once


#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/logging/log.h>


typedef struct  {

    uint32_t timeStamp ;
    int16_t accel_x = 1 ;
    int16_t accel_y = 2 ;
    int16_t accel_z  = 3 ;
    int16_t gyro_x = 4 ;
    int16_t gyro_y = 5 ;
    int16_t gyro_z  = 6 ;
    int16_t odometer = 7 ;

}ImuData;

// Structure to hold Magnetometer data
typedef struct  {

    uint32_t timeStamp ;
    int16_t mag_x = 1 ;
    int16_t mag_y = 2 ;
    int16_t mag_z = 3 ; 
    int16_t euler_x = 4 ;
    int16_t euler_y = 5 ;
    int16_t euler_z = 6 ;
    int16_t temperature = 7 ;
    
}MagnetoData;

// Structure to hold GNSS data
typedef struct  {

    uint32_t timeStamp;
    int32_t timeUTC = 1 ;
    int16_t altitude = 2 ;
    int16_t latitude = 3 ;
    int16_t longitude = 4 ;
    int16_t acceleration = 5 ;
    int16_t velocity = 6 ;
    int16_t svs = 7 ; 

}GnssData;


class BLEManager{

    public:
        void init();
        void loop();

    private:
        void notifySensorData(void);

        static void bleReady(int err);

        static ssize_t readImu(struct bt_conn *conn, const struct bt_gatt_attr *attr,
            void *buf, uint16_t len, uint16_t offset);
};  