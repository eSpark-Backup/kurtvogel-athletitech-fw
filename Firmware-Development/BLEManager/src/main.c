#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

static struct bt_conn *current_conn = NULL;
static bool imuNotifyEnabled = false;
static bool magnetoNotifyEnabled = false;
static bool gnssNotifyEnabled = false;

// Structure to hold IMU data
struct imuData {
    int16_t timestamp;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};

// Structure to hold Magnetometer data
struct magnetoData {
    int16_t timestamp;
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    int16_t euler_x;
    int16_t euler_y;
    int16_t euler_z;
    int16_t temperature;
};

// Structure to hold GNSS data
struct gnssData {
    int16_t timestamp;
    int16_t altitude;
    int16_t latitude;
    int16_t longitude;
    int16_t velocity;
    int16_t acceleration;
    int16_t odometer;
    int16_t svs;
    int32_t time_utc;
};

// Create instances of the structures
static struct imuData imu = {
    .accel_x = 1, .accel_y = 2, .accel_z = 3,
    .gyro_x = 4, .gyro_y = 5, .gyro_z = 6,
    .timestamp = 999
};

static struct magnetoData magneto = {
    .mag_x = 1, .mag_y = 2, .mag_z = 3,
    .euler_x = 4, .euler_y = 5, .euler_z = 6,
    .temperature = 7, .timestamp = 999
};

static struct gnssData gnss = {
    .altitude = 1, .latitude = 2, .longitude = 3,
    .velocity = 4, .acceleration = 5, .odometer = 6,
    .svs = 7, 
    .time_utc = 8,
    .timestamp = 999
};

// Read callback functions for the characteristics
static ssize_t readImu(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                       void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &imu, sizeof(imu));
}

static ssize_t readMagneto(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                           void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &magneto, sizeof(magneto));
}

static ssize_t readGnss(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                        void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &gnss, sizeof(gnss));
}

// Notify configuration callbacks
static void imuCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value)
{
    imuNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
    printk("IMU Notifications %s\n", imuNotifyEnabled ? "enabled" : "disabled");
}

static void magnetoCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value)
{
    magnetoNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
    printk("Magneto Notifications %s\n", magnetoNotifyEnabled ? "enabled" : "disabled");
}

static void gnssCccCfgChanged(const struct bt_gatt_attr *attr, uint16_t value)
{
    gnssNotifyEnabled = (value == BT_GATT_CCC_NOTIFY);
    printk("GNSS Notifications %s\n", gnssNotifyEnabled ? "enabled" : "disabled");
}

// Callback for connection events
static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err %d)\n", err);
        return;
    }
    printk("Connected\n");
    current_conn = bt_conn_ref(conn); // Store connection reference
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason %d)\n", reason);
    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
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
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789ABCDEF0))),

    // IMU Characteristic - Read + Notify
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xABCDEF01, 0x1234, 0x5678, 0x1234, 0x56789ABCDEF0)),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           readImu, NULL, &imu),
    BT_GATT_CCC(imuCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    // Magnetometer Characteristic - Read + Notify
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xABCDEF02, 0x1234, 0x5678, 0x1234, 0x56789ABCDEF0)),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           readMagneto, NULL, &magneto),
    BT_GATT_CCC(magnetoCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    // GNSS Characteristic - Read + Notify
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0xABCDEF03, 0x1234, 0x5678, 0x1234, 0x56789ABCDEF0)),
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           readGnss, NULL, &gnss),
    BT_GATT_CCC(gnssCccCfgChanged, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

// Notify function
void notifySensorData(void)
{
    if (!current_conn) {
        printk("No active connection\n");
        return;
    }

    // Update timestamps
    uint32_t current_time = k_uptime_get_32();
    imu.timestamp = current_time;
    magneto.timestamp = current_time;
    gnss.timestamp = current_time;

    // Send notifications for each characteristic if enabled
    if (imuNotifyEnabled) {
        int err = bt_gatt_notify(current_conn, &dataTransferService.attrs[2], &imu, sizeof(imu));
        if (err) {
            printk("IMU Notification failed (err %d)\n", err);
        } else {
            printk("IMU notification sent\n");
        }
    }
    
    if (magnetoNotifyEnabled) {
        int err = bt_gatt_notify(current_conn, &dataTransferService.attrs[5], &magneto, sizeof(magneto));
        if (err) {
            printk("Magneto Notification failed (err %d)\n", err);
        } else {
            printk("Magneto notification sent\n");
        }
    }

    if (gnssNotifyEnabled) {
        int err = bt_gatt_notify(current_conn, &dataTransferService.attrs[8], &gnss, sizeof(gnss));
        if (err) {
            printk("GNSS Notification failed (err %d)\n", err);
        } else {
            printk("GNSS notification sent\n");
        }
    }
}

// Initialize Bluetooth
static void bleReady(int err)
{
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    // Register connection callbacks
    bt_conn_cb_register(&conn_callbacks);

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, NULL, 0, NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
    }
}

// Main function
void main(void)
{
    int err = bt_enable(bleReady);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("BLE Peripheral Started\n");

    while (1) {
        notifySensorData();
        k_msleep(1000); // 1 second between notifications
    }
}