# T962 PC
T962 soldering oven control and setup application for Windows.  
You can read the project description and files here: > [T-962 more improvements](https://en.gradient-sg.com/t962/)  

This version of the program uses a bluetooth dongle based on the TI CC2540 chip.   
The dongle is connected via virtual COM port, the program needs a driver from TI.   
Read the TI documentation how to use TI CC2540:    
https://www.ti.com/tool/BLE-STACK    
https://www.ti.com/product/CC2540    
   

Use a TI cc-debugger programmer: https://www.aliexpress.com/af/cc-debugger.html  
For download firmware file into the dongle: https://gradient-sg.com/files/t962/cc2540fw.zip  
Download firmware file to nRF51822: https://gradient-sg.com/files/t962/t962nrf.zip

Example can see here: https://t1.daumcdn.net/cfile/tistory/2161273557831B1D2B

If all is correct, you will connect in 1-3 seconds after the program starts. 
Then by pressing the buttons use oven control. 

![tibt](https://user-images.githubusercontent.com/13213368/205233465-f96a94d3-7341-4a24-a69e-5be093637277.png)
