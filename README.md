# ESP8266_Misting_Valve_Controller

ESP8266 web-socket controller for a Solenoid valve. This controller has the ability to create "ON" and "OFF" schedules for a water flow valve of a misting system. Temperature and humidity sensors are also included.  


<img src="images/MistingValve.png" width="700"></img>


![valve](/images/iPhoneWaterValve.png)


![Device.html](/images/DeviceHTML.png)

When the device.html page is requested (e.g.“http://192.168.1.100/device.html”) the ESP8266 web server is instructed to look in the memory variable PAGE_DeviceControlPage in DeviceControl.h for the html data. 


An alternative approach is to use SPIFFS to store a physical device.html file on the ESP8266. To accomplish this, copy the html element (and content) from the devicecontrol.h and paste it into a new html document (replacing the existing HTLM element) and name it “Device.html”

![DeviceControl.h](/images/DeviceControl_h.png)



![Valve & Flow Sensor](/images/Sensor&Valve.png)

[Brass Liquid Solenoid Valve - 12V - 1/2 NPS](https://www.adafruit.com/product/996) and 
[Hall Effect Flow Sensor](https://www.amazon.com/gp/product/B01FJR9RRK/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1)


![Housing D! Mini DH22](/images/Housing_D1Mini.png)

D1 Mini, DH22 and PVC housing. The DH22 is isolated from the the D1 Mini is a PVC housing that allows air circulation but prevent water entering.

![DH22 Housing Exploded](/images/DH22EXP.png)


The HomeseerSend library is not required for normal autonomous operation of the device. It is used to send data to a home automation server called Homeseer running on my network. Homeseer monitors the temperatures received from that and other weather devices and knows when and how long (monitors water flow) to turn on the flow valve. It also sends voice and text alerts when the valve is turned on.

![Homeseer](/images/Homeseer.png)

Home assistant is a free software that will do the same thing.
