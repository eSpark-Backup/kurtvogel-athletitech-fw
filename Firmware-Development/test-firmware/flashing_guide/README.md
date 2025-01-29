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
