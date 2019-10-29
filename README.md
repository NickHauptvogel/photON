# Project photON
###### Smart Bike Light using an ESP8266 and Neopixel LEDs

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
