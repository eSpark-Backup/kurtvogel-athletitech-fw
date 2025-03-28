#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

static uint8_t char1_value[20] = "Data1";  // Characteristic 1
static uint8_t char2_value[20] = "Data2";  // Characteristic 2
static uint8_t char3_value[20] = "Data3";  // Characteristic 3

// Read callback function (optional, but good for debugging)
static ssize_t read_char1(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                          void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, char1_value, sizeof(char1_value));
}

static ssize_t read_char2(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                          void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, char2_value, sizeof(char2_value));
}

static ssize_t read_char3(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                          void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset, char3_value, sizeof(char3_value));
}

// Define the BLE Service
BT_GATT_SERVICE_DEFINE(my_service,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)),

    // Characteristic 1 - Readable
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(0xABCDEF01, 0x1234, 0x5678, 0x1234, 0x56789abcdef0),
                           BT_GATT_CHRC_READ,
                           BT_GATT_PERM_READ,
                           read_char1, NULL, char1_value),

    // Characteristic 2 - Readable
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(0xABCDEF02, 0x1234, 0x5678, 0x1234, 0x56789abcdef0),
                           BT_GATT_CHRC_READ,
                           BT_GATT_PERM_READ,
                           read_char2, NULL, char2_value),

    // Characteristic 3 - Readable
    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(0xABCDEF03, 0x1234, 0x5678, 0x1234, 0x56789abcdef0),
                           BT_GATT_CHRC_READ,
                           BT_GATT_PERM_READ,
                           read_char3, NULL, char3_value)
);

static void bt_ready(int err)
{
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");
    bt_le_adv_start(BT_LE_ADV_CONN_NAME, NULL, 0, NULL, 0);
}

void update_characteristic_values(void)
{
    strcpy(char1_value, "imu");
    strcpy(char2_value, "magnetometer");
    strcpy(char3_value, "GNSS");

    printk("Characteristic values updated\n");
}

void main(void)
{
    int err;

    err = bt_enable(bt_ready);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }
 
    printk("BLE Peripheral Started\n");

    while(1){
        update_characteristic_values();
        k_msleep(5000);
    }

}

