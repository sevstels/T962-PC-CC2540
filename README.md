# T962 PC
T962 soldering oven control and setup application for Windows.  
You can read the project description and files here: > [T-962 more improvements](https://en.gradient-sg.com/t962/)  

This version of the program uses a bluetooth dongle based on the TI CC2540 chip.   
The dongle is connected via COM port, the program needs a driver from TI.   
Read the TI documentation how to connect. 

Use a TI cc-debugger programmer: https://www.aliexpress.com/af/cc-debugger.html
The firmware must be loaded into the dongle: 
C:\Texas Instruments\BLE-CC254x-1.5.1.1\Accessories\HexFiles\CC2540_USBdongle_HostTestRelease_All.hex

Example can see here: https://t1.daumcdn.net/cfile/tistory/2161273557831B1D2B

If the Nordic nRF51822 address is correct, you will connect in 1-3 seconds after the program starts. 
Then by pressing the control buttons you can observe the data exchange. Don't forget to program into the nRF51822 the firmware from here: https://gradient-sg.com/files/t962/t962nrf.zip

![tibt](https://user-images.githubusercontent.com/13213368/205233465-f96a94d3-7341-4a24-a69e-5be093637277.png)