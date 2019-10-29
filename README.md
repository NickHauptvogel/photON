# Project photON
###### Smart Bike Light using an ESP8266 and Neopixel LEDs

![requirements](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191006_234215.jpg)

In this guide, we will build a custom bike light with Neopixel LEDs and the WiFi Microcontroller ESP8266. This allows you to separately control every single LED with a custom color, make animations etc. 

After the building process, you will find my example code featuring startup animations, blink animations, light control and a accelerometer-controlled brake light, all accessible via a website after the uController connects to your phone's hotspot and acts as a Mini-Webserver.

**Let's start building a cheap bike light which you can control!**

## Hardware setup
### Shopping list

![requirements](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_122453.jpg)

For the project, you need:
- ESP8266 Microcontroller: I used the NodeMCU Lua Amica Modul V2 ESP8266 ESP-12E WIFI, as it can be directly flashed via micro-USB and can handle input voltages above 3.3V (the powerbank has a 5V output)

- powerbank: I used a small 3000mAh Powerbank that fits into the saddle pocket. It has an output of 5V 1A.

- saddle bag: A small pocket to fit your microcontroller, power bank and (for me) mount the back-LEDs to.

- Neopixel LEDs: You don't have to use original Neopixel LEDs (that are much more expensive) and easily get some from eBay, only be aware that the colors can be a little bit different. **Important: The Neopixels must have the WS2812(B) Chipset.** I got a Neopixel Ring (16 LEDs) and a waterproof 1m strip that I cut through.

- Shrinking tubes

- Electrical tape (preferably in a color that matches your bike, I took black)

- stranded wire (preferably in 3 colors: 5V, GND, Data)

- Accessories: adhesive tape, cable ties, cable stripper, hot glue gun


### Bike mounting

1. Cut your LEDs to the desired length (I cut the strip in 2x 10 LEDs for the front and 2x 3 LEDs for the back in addition to the ring)
2. Plan out your placement and measure the distance for the wire to run from one LED part to the next. **CAUTION: When measuring the distance from front to back, consider doing so with maximum steering angle so that the wire is long enough in any case**
My setup (the black connection represents the compound of 5V, GND and data):

![bike](https://github.com/NickHauptvogel/photON/blob/master/common/bike.png)

3. Cut the stranded wires to the lengths, unstrip the ends and solder them onto the LED contacts. **Thread the shrinking tubes onto the stranded wire compound before soldering**
4. On each soldering spot, apply a big drop of hot glue and spread it across the soldered spot. Repeat this from the bottom, so that the hot glue surrounds the complete soldering point.
5. Shrink the tube as close to the soldering point as possible with a lighter.
6. Stick the LEDs onto the desired position. I mounted the front LEDs in a 45 degree angle in order for them to be visible from the side. Then, to secure the strips, I tied the shrinking tube "ends" of the strips to my bike frame.

![backstrip](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_121302.jpg)
![frontstrip](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_121351.jpg)

I mounted the LED ring to the bag and secured the bag with my conventional back light. Before that, I wrapped the non-waterproof ring in adhesive tape to make it at least a bit water-resistant under the saddle:

![frontstrip](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_121103.jpg)

7. Wrap all cables with electrical tape to give them extra protection and tie them to the frame.
8. Solder the 5V Cable to "VIN" of the NodeMCU, GND to any "GND" and Data to "D6".
9. Place the powerbank into the saddle pocket.

![saddlepocketopen](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_121204.jpg)
![saddlepocket](https://github.com/NickHauptvogel/photON/blob/master/common/IMG_20191029_121132.jpg)

10. Done! Proceed to software setup!

## Software setup
### Installation

1. Download the CP2102 (CP210x) USB to UART driver (https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
2. Download ArduinoIDE (e.g. from Windows store)
3. In the IDE, go to File > Preferences.
4. Paste the URL http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Manager URLs field.
5. Go to Tools > Board > Board Manager and search for 'esp8266'. Select the newest version, and click install.
6. Install the Neopixel library: Go to Tools > Manage Libraries, search for "Adafruit NeoPixel" and install the newest version by "Adafruit".

### Included Software

Up to now, you are more or less finished and can start writing your own code. Just follow any example tutorial for NeoPixels.
However, if you want to use my code, feel free to do so. In the following I show you how to configure it.

1. Clone/Download the repo and open the sketch under /src/light_full or /src/light_only_back
2. Values you have to adapt:
  a. NUMPIXELS: total number of pixels you have connected
  b. STASSID: SSID of the hotspot from your phone
  c. STAPSK: Password of the hotspot
  d. indices (front/back left/right/ring): positions of the LEDs in your array. If you, e.g., only have 9 LEDs in the front, your NUMPIXELS has to be decreased and the front index have to be adapted (only 9 values, all decreased by 1 on one side)
  e. The rest is optional and mostly documented in the code, e.g. the color correction values for white (GREEN/BLUE_FACTOR_WHITE_xxx)
  f. **NOTE: If you copy my LED setup, you only have to change the Access Point Settings**
  
Activate the hotspot on your phone and open the serial monitor in the IDE. Then flash the program onto the node. After startup, it will connect to the hotspot and tell you its IP via the serial monitor. Enter that IP in a browser (recommended is Firefox for Android, as Chrome does not support Accelerometer data) and you will get the dashboard of your bike light. Play around!

Features: 
- Front Light On/Off
- Back Light On/Off
- Blink left/right
- Brake Light (Third bar, gets brighter when braking as well as the LEDs)
- Automatic On/Off via Ambient Light (Light Value inside the fourth bar)

### Change dashboard

If you want to change the dashboard, you can make your changes in the index.html in /src/webpage/. After that, go to http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html and convert the whole content. The copy the result into the .ino file. This is the string that will be sent by the NodeMCU if somebody connects to it.

## Prospect
First of all, I thank https://github.com/MrBennetable for the help.
I will continue to improve the code, and maybe add a brake switch for a manual trigger of the brake light. Moreover, I want to add a hard power off - function for the node in order to just power it off inside the software instead of always plugging the powerbank in and out.
