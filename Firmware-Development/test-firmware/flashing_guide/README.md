## FLASHING INSTRUCTION FOR NRF52840

### Using J-Flash: 
1. download segger J-Link from https://www.segger.com/downloads/jlink/
2. Connect the nrf52840 through the J-link adapter to your PC or through micro USB if connected to MCU through Jlink hardware.
3. Open J-Link Flash and create a new project 
4. A prompt will open asking for target device , select nrf52480 as target device and leave the other setting as it is.
5. now drag the merged.hex file from the (flashing_guide->bin) folder to the box asking for hex file or you can also select it from file->Open data file
6. now naviagte to target-> and press Connect
7. now naviagte to target->manual programming-> program and verify 
8. if it shows target programmed and verify successfully ,you can run the application by target->manual programming-> Start Application

### Using VSCODE:
[flashing guide using vscode](https://github.com/eSpark-Consultants/kurtvogel-athletitech-fw/blob/Development-FW/Firmware-Development/test-firmware/flashing_guide/Flashing.and.Testing.Guide.usingVSCODE.pdf)


***Note: if the flashing fails ,try to erase the chip by navigating and selecting target->manual programming-> Erase chip    and then reprogramming it again.***


## MONITORING LOGS FOR NRF52840

### Using UART to USB birdge/adapter (CP210x):

1. install the CP210x driver using this [link](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)
2. now plugin the adapter to your pc
3. connect the txd pin of the adapter to the TRACE DATA 2 pin (p0.11)
4. connect the rxd pin of the adapter to the TRACE DATA 1 pin (p0.12)
5. now go to device manager->ports and see if the device is detected , if its already there then its okay if not then update driver and select the CP210x.zip file 
downloaded prevously
6. you will  be able to monitor logs using any serial monitor and selecting the COM port at which  Silicon Labs Cp210x is present 
and setting the baud rate to 115200 .
7. Logs will only appear after the merged.hex file is flashed and the NRF52840 is powered on.

On successfull setup following logs will be printed :(***Note: if the logs are not showing press reset the nrf and do it while monitoring***)

```bash
Scanning I2C0 bus...
Sensor detected at 0x76 Bus:I2C0
I2C0 scan complete.
Scanning I2C1 bus...
I2C1 scan complete.
ICM45686 found
Initializing ICM45686
Successfully data read from WHO_AM_I register
WHO_AM_I: 0x0
Buffer: 100000008000
Successfull data fetch from ACCEL_REG
Buffer2: 000000800000
Successfull data fetch from GYRO_REG
Accel: X=4096, Y=0, Z=-32768
Gyro: X=0, Y=128, Z=0
Buffer: 100000008000
Successfull data fetch from ACCEL_REG
Buffer2: 000000800000
Successfull data fetch from GYRO_REG
Accel: X=4096, Y=0, Z=-32768
Gyro: X=0, Y=128, Z=0
```
if the sensor fails to detect:
```bash
Scanning I2C0 bus...
Sensor detected at 0x76 Bus:I2C0
I2C0 scan complete.
Scanning I2C1 bus...
I2C1 scan complete.
ICM not found....
```