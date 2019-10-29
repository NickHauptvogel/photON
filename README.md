# Project photON
###### Smart Bike Light using an ESP8266 and Neopixel LEDs

In this guide, we will build a custom bike light with Neopixel LEDs and the WiFi Microcontroller ESP8266. This allows you to separately control every single LED with a custom color, make animations etc. 

After the building process, you will find my example code featuring startup animations, blink animations, light control and a accelerometer-controlled brake light, all accessible via a website after the uController connects to your phone's hotspot and acts as a Mini-Webserver.

**Let's start building a cheap bike light which you can control!**

## Hardware setup
### Shopping list



For the project, you need:
- ESP8266 Microcontroller: I used the NodeMCU Lua Amica Modul V2 ESP8266 ESP-12E WIFI, as it can be directly flashed via micro-USB and can handle input voltages above 3.3V (the powerbank has a 5V output)

- powerbank: I used a small 3000mAh Powerbank that fits into the saddle pocket. It has an output of 5V 1A.

- saddle bag: A small pocket to fit your microcontroller, power bank and (for me) mount the back-LEDs to.

- Neopixel LEDs: You don't have to use original Neopixel LEDs (that are much more expensive) and easily get some from eBay, only be aware that the colors can be a little bit different. **Important: The Neopixels must have the WS2812(B) Chipset.** I got a Neopixel Ring (16 LEDs) and a 1m strip that I cut through.

- Shrinking tubes

- Electrical tape (preferably in a color that matches your bike, I took black)

- stranded wire (preferably in 3 colors: 5V, GND, Data)

- Accessories: adhesive tape, cable ties, cable stripper, hot glue gun
