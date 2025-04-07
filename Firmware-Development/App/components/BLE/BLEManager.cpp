
#include "BLEManager.hpp"

LOG_MODULE_REGISTER(BleManager, LOG_LEVEL_DBG);  // LOG_LEVEL_DBG shows everything

static struct bt_conn *currentConnection = NULL;
static bool imuNotifyEnabled = false;
static bool magnetoNotifyEnabled = false;
static bool gnssNotifyEnabled = false;

ImuData imu;
MagnetoData magnet;
GnssData gnss;

// Read callback functions for the characteristics
static ssize_t readImu(struct bt_conn *conn, const struct bt_gatt_attr *attr,
    void *buf, uint16_t len, uint16_t offset){
        return bt_gatt_attr_read(conn, attr, buf, len, offset, &imu, sizeof(imu));
}

static ssize_t readMagneto(struct bt_conn *conn, const struct bt_gatt_attr *attr,
        void *buf, uint16_t len, uint16_t offset){

        return bt_gatt_attr_read(conn, attr, buf, len, offset, &magnet, sizeof(magnet));
}

static ssize_t readGnss(struct bt_conn *conn, const struct bt_gatt_attr *attr,
     void *buf, uint16_t len, uint16_t offset)
{
return bt_gatt_attr_read(conn, attr, buf, len, offset, &gnss, sizeof(gnss));
}

// Notify configuration callbacks
static void imuCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value){

        imuNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
        printk("IMU Notifications %s\n", imuNotifyEnabled ? "enabled" : "disabled");
}

static void magnetoCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value){

        magnetoNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
        printk("magnetNotifications %s\n", magnetoNotifyEnabled ? "enabled" : "disabled");
}

static void gnssCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value){

    gnssNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
    printk("GNSS Notifications %s\n", gnssNotifyEnabled ? "enabled" : "disabled");
}

// Callback for connection events
static void connected(struct bt_conn *conn, uint8_t err){
    
        if (err) {
            LOG_ERR("[connected] Connection failed (err %d)", err);
            return;
        }
        LOG_INF("[connected] Connection Success");
        currentConnection = bt_conn_ref(conn); // Store connection reference
}

static void disconnected(struct bt_conn *conn, uint8_t reason){

        LOG_ERR("[disconnected] Disconnected (reason %d)", reason);
        if (currentConnection) {
            bt_conn_unref(currentConnection);
            currentConnection = NULL;
        }
        imuNotifyEnabled = false;
        magnetoNotifyEnabled = false;
        gnssNotifyEnabled = false;
}

// Connection callbacks
static struct bt_conn_cb conn_callbacks = {
        .connected = connected,
        .disconnected = disconnected,
};

    // Define the BLE Service
BT_GATT_SERVICE_DEFINE(dataTransferService,
        BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xc43a8e28, 0x59a1, 0x4c5b, 0x9ec4, 0xff8fc845b179))),

        // IMU Characteristic - Read + Notify
        BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x7c2b0f38, 0x4993, 0x4c6a, 0x9458, 0xff8fc845b179)),
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                readImu, NULL, &imu),
        BT_GATT_CCC(imuCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

        // Magnetometer Characteristic - Read + Notify
        BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x1a4c5e87, 0x60f1, 0x4b14, 0x8a9e, 0xe71c8f0d5e30)),
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                readMagneto, NULL, &magnet),
        BT_GATT_CCC(magnetoCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

        // GNSS Characteristic - Read + Notify
        BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xf8a1d2e0, 0x86d1, 0x41e6, 0xb489, 0xcae4c5fd5b11)),
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                readGnss, NULL, &gnss),
        BT_GATT_CCC(gnssCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

// Notify function
void BLEManager::notifySensorData(void){

    if (!currentConnection) {
        LOG_WRN("[notifySensorData] No active connection" );
        return;
    }

    // Update timeStamps
    uint32_t current_time = k_uptime_get_32();

    LOG_INF("[notifySensorData] current_time : %d" , current_time);
    imu.timeStamp = current_time;
    magnet.timeStamp = current_time;
    gnss.timeStamp = current_time;

    // Send notifications for each characteristic if enabled
    if (imuNotifyEnabled) {
        int err = bt_gatt_notify(currentConnection, &dataTransferService.attrs[2], &imu, sizeof(imu));
        if (err) {
            LOG_ERR("[notifySensorData] IMU Notification failed (err %d)", err);
        } else {
            LOG_INF("[notifySensorData] IMU Notification sent (err %d)", err);
        }
    }
    
    if (magnetoNotifyEnabled) {
        int err = bt_gatt_notify(currentConnection, &dataTransferService.attrs[5], &magnet, sizeof(magnet));
        if (err) {
            LOG_ERR("[notifySensorData] magnetNotification failed (err %d)", err);
        } else {
            LOG_INF("[notifySensorData] magnetNotification sent (err %d)", err);
        }
    }

    if (gnssNotifyEnabled) {
        int err = bt_gatt_notify(currentConnection, &dataTransferService.attrs[8], &gnss, sizeof(gnss));
        if (err) {
            LOG_ERR("[notifySensorData] GNSS Notification failed (err %d)", err);
        } else {
            LOG_INF("[notifySensorData] GNSS Notification sent (err %d)", err);
        }
    }
}


 void BLEManager::bleReady(int err){


    if (err) {
        LOG_ERR("[bleReady] Bluetooth init failed (err %d)", err);
        return;
    }
    LOG_INF("[bleReady] Bluetooth initialized");
    // Register connection callbacks
    bt_conn_cb_register(&conn_callbacks);

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, NULL, 0, NULL, 0);
    if (err) {
        LOG_ERR("[bleReady] Advertising failed to start (err %d)", err);
    }
    LOG_INF("[bleReady] Advertising Started");

}



void BLEManager::init(){
    
 


    int err = bt_enable(bleReady);
    if (err) {
        LOG_ERR("[init] BLE init failed (err %d)",err);
        return;
    }
    LOG_INF("[init] BLE peripheral started (err %d)",err);

}

void BLEManager::loop(){

    
    notifySensorData();
}