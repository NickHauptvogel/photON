#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> 
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// -------- DEFINES ---------
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        D6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 22

#ifndef STASSID
#define STASSID "BennetsiPhone"
#define STAPSK  "NodeMCUWiFi"
#endif

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// -------- GLOBALS ---------
// SSID of the Node AP
const char *ssid = STASSID;

// No password for the WiFi
const char *password = STAPSK;

// Instance of the webserver
ESP8266WebServer server(80);

// The number of steps that animations on the Ring have
int BLINKSTEPS = 8;

// The brightness value that a step increases or decreases the brightness (x/255)
int ANIMATION_DELTA = 10;

int ring_index[] = {0, 1, 2, 3, 4, 5, 6, 7};

int back_right_index[] = {15, 16, 17, 18, 19, 20, 21};
int back_left_index[] = {8, 9, 10, 11, 12, 13, 14};

int NUMBER_RING = sizeof(ring_index)/sizeof(int);
int NUMBER_BACK = sizeof(back_left_index)/sizeof(int);

//The Factors to set the desired yellow color (Factor affects Green Value, the smaller the less green light and the more red light)
const float GREEN_FACTOR_YELLOW_RING = 0.25;
const float GREEN_FACTOR_YELLOW_BAR = 0.6;
const float BLUE_FACTOR_YELLOW_RING = 0;
const float BLUE_FACTOR_YELLOW_BAR = 0;

int red[] = {100, 0, 0};
int yellow[] = {100, 50, 0};
int green[] = {0, 100, 0};
int bright_red[] = {255, 0, 0};
int white[] = {150, 255, 210};
int blue_white[] = {50, 100, 255};
int off_color[] = {0, 0, 0};

/*
 * Sets the color of a pixel array.
 * @param pixel Array of pixel indices to set
 * @param number Number of pixels in the array
 * @param color array of RGB values
 */
void set_color(int * pixel, int number, int color[]) {
  for (int i = 0; i < number; i++) {
    pixels.setPixelColor(pixel[i], pixels.Color(color[0], color[1], color[2])); 
  }
}

void set_color(int pixel, int color[]) {
  int tmp[] = {pixel};
  set_color(tmp, 1, color);
}

void clear_color(int * pixel, int number) {
  set_color(pixel, number, off_color);
}

void fade_on(int * pixel, int number, int color[], boolean reverse){
  for (int m = 0; m < number; m++) {
    if (reverse) set_color(pixel[number-1-m], color);
    else set_color(pixel[m], color);
    pixels.show();
    delay(60);
  } 
}

void flash(int * pixel, int number, int color[], int times){
    for(int i = 0; i < times; i++) {
      set_color(pixel, number, color);
      pixels.show();
      delay(300); 
      clear_color(pixel, number);  
      pixels.show();
      delay(300);
    }
}

/**
 * Sets a relative color of a pixel array. The given RED-Value is the reference (its absolute value can be considered as brightness), 
 * the GREEN and BLUE value are relative factors that are multiplied with the reference.
 * @param pixel Array of pixel indices to set
 * @param number Number of pixels in the array
 * @param reference R Value of the color
 * @param greenFactor G Factor of the color
 * @param blueFactor B Factor of the color
 */
void set_relative_color(int * pixel, int number, int reference, float greenFactor, float blueFactor) {
  
  float greendivisor = 1.0;
  float bluedivisor = 1.0;
  // computation of the respective green and blue value
  int greenvalue = reference * greenFactor;
  int bluevalue = reference * blueFactor;

  //If one or both computated values exceed the limit of 255, they have to be divided down to 255
  if (greenvalue > 255) {
    greendivisor = greenvalue / 255;
  }
  if (bluevalue > 255) {
    bluedivisor = bluevalue / 255;
  }

  //The bigger divisor is taken to bring all values under 255, if no value is exceeded the initialised value 1.0 has no effect on the values
  if(greendivisor > bluedivisor) {
    reference /= greendivisor;
    greenvalue /= greendivisor;
    bluevalue /= greendivisor;
  } else {
    reference /= bluedivisor;
    greenvalue /= bluedivisor;
    bluevalue /= bluedivisor;
  }
  //Construct the color array
  int color[] = {reference, greenvalue, bluevalue};
  set_color(pixel, number, color);
}

void set_relative_color(int pixel, int reference, float greenFactor, float blueFactor) {
  int tmp[] = {pixel}; 
  set_relative_color(tmp, 1, reference, greenFactor, blueFactor);
}

void running_light_animation(int * pixel, int number, int highlightcolor[], int backgroundcolor[], boolean clockwise) {

  if (clockwise) {
    set_color(pixel[0], highlightcolor);
  } else {
    set_color(pixel[number-1], highlightcolor);
  }
  pixels.show();
  delay(60);
  for (int k = 1; k <= number-1; k++) {
    if(clockwise) {
      set_color(pixel[k], highlightcolor);
      set_color(pixel[k-1], backgroundcolor);
    } else {
      set_color(pixel[number-1-k], highlightcolor);
      set_color(pixel[number-k], backgroundcolor);
    }
    pixels.show();
    delay(60);
  }
  if(clockwise) {
    set_color(pixel[number-1], backgroundcolor);
  } else {
   set_color(pixel[0], backgroundcolor); 
  }
  pixels.show();
}

void running_light_animation(int * pixel, int number, int highlightcolor[], boolean clockwise) {
  running_light_animation(pixel, number, highlightcolor, off_color, clockwise);
}

void startup_animation() {
  //First a red running light on the ring twice
  running_light_animation(ring_index, NUMBER_RING, red, true);
  
  fade_on(ring_index, NUMBER_RING, red, true);
  
  running_light_animation(ring_index, NUMBER_RING, bright_red, red, false);

  for (int k = 0; k < NUMBER_BACK; k++) {
    set_color(back_right_index[k], red);
    set_color(back_left_index[k], red);
    pixels.show();
    delay(120);
  }  
}

void send_page() {
  String html ="<html> <head> <title>Project photON</title> <style> body { font-family: Montserrat, Helvetica, Arial, sans-serif; background-color: #222222; } h1 { text-align: center; color: white; } div.container { width: 95vw; height: 200px; margin-top: 20px; margin: 0 auto; /*background-color: blue;*/ } div.lrButton { background-color: #e07f00; width: 49%; color: white; border-radius: 50px; font-size: 72pt; text-align: center; float: left; line-height: 200px; } div.frontButton { background-color: white; width: 95vw; height: 200px; color: black; border-radius: 50px; font-size: 72pt; text-align: center; margin: 0 auto; margin-top: 20px; line-height: 200px; } div.backButton { background-color: rgb(160, 0, 0); width: 95vw; height: 200px; color: white; border-radius: 50px; font-size: 72pt; text-align: center; margin: 0 auto; margin-top: 20px; line-height: 200px; } </style> <script type=\"text/javascript\" language=\"javascript\"> function setButtons() { var url_string = window.location.href; var url = new URL(url_string); var blinkState = url.searchParams.get(\"blink\"); var frontState = url.searchParams.get(\"front\"); var backState = url.searchParams.get(\"back\"); if (blinkState == \"left\") { document.getElementById(\"buttonLeft\").style.background = \"yellow\"; document.getElementById(\"buttonLeft\").style.color = \"black\"; document.getElementById(\"buttonRight\").style.background = \"#e07f00\"; document.getElementById(\"buttonRight\").style.color = \"white\"; } else if (blinkState == \"right\") { document.getElementById(\"buttonLeft\").style.background = \"#e07f00\"; document.getElementById(\"buttonLeft\").style.color = \"white\"; document.getElementById(\"buttonRight\").style.background = \"yellow\"; document.getElementById(\"buttonRight\").style.color = \"black\"; } else { document.getElementById(\"buttonLeft\").style.background = \"#e07f00\"; document.getElementById(\"buttonLeft\").style.color = \"white\"; document.getElementById(\"buttonRight\").style.background = \"#e07f00\"; document.getElementById(\"buttonRight\").style.color = \"white\"; } if (frontState == \"off\") { document.getElementById(\"buttonFront\").style.background = \"#666666\"; document.getElementById(\"buttonFront\").style.color = \"white\"; } else { document.getElementById(\"buttonFront\").style.background = \"white\"; document.getElementById(\"buttonFront\").style.color = \"black\"; } if (backState == \"off\") { document.getElementById(\"buttonBack\").style.background = \"rgb(100, 0, 0)\"; document.getElementById(\"buttonBack\").style.color = \"black\"; } else { document.getElementById(\"buttonBack\").style.background = \"rgb(160, 0, 0)\"; document.getElementById(\"buttonBack\").style.color = \"white\"; } } function insertParam(key, value){ key = encodeURI(key); value = encodeURI(value); var kvp = document.location.search.substr(1).split('&'); var i=kvp.length; var x; while(i--) { x = kvp[i].split('='); if (x[0]==key) { x[1] = value; kvp[i] = x.join('='); break; } } if(i<0) {kvp[kvp.length] = [key,value].join('=');} document.location.search = kvp.join('&'); } function blinkLeft(){ var url_string = window.location.href; var url = new URL(url_string); var blinkState = url.searchParams.get(\"blink\"); if (blinkState == \"left\") { insertParam(\"blink\", \"off\"); } else { insertParam(\"blink\", \"left\"); } } function blinkRight(){ var url_string = window.location.href; var url = new URL(url_string); var blinkState = url.searchParams.get(\"blink\"); if (blinkState == \"right\") { insertParam(\"blink\", \"off\"); } else { insertParam(\"blink\", \"right\"); } } function frontTrigger(){ var url_string = window.location.href; var url = new URL(url_string); var frontState = url.searchParams.get(\"front\"); if (frontState == \"off\") { insertParam(\"front\", \"on\"); } else { insertParam(\"front\", \"off\"); } } function backTrigger(){ var url_string = window.location.href; var url = new URL(url_string); var backState = url.searchParams.get(\"back\"); if (backState == \"off\") { insertParam(\"back\", \"on\"); } else { insertParam(\"back\", \"off\"); } } </script> </head> <body onload=\"setButtons()\"> <div class=\"container\"> <div class=\"lrButton\" id=\"buttonLeft\" onclick=\"blinkLeft();\"> &lt; </div> <div class=\"lrButton\" style=\"float: right\" id=\"buttonRight\" onclick=\"blinkRight();\"> &gt; </div> </div> <div class=\"frontButton\" id=\"buttonFront\" onclick=\"frontTrigger()\"> FRONT </div> <div class=\"backButton\" id=\"buttonBack\" onclick=\"backTrigger()\"> BACK </div> </body> </html>";
  server.send(200, "text/html", html);
}

void _blink(boolean left) {
    int brightness[BLINKSTEPS];
    memset(brightness,0,sizeof(brightness));
    
    for(int i = 0; i < BLINKSTEPS; i++) {
      brightness[i] = ANIMATION_DELTA;
      for(int j = 0; j < i; j++) {
        brightness[i-j-1] += ANIMATION_DELTA;
      }
      set_blink_leds(left, brightness, sizeof(brightness)/sizeof(int)); 
    }

    for (int l = 1; l < BLINKSTEPS; l++) {
      brightness[l]=brightness[l-1];
      for(int m=0; m<l; m++) {
        brightness[l-m-1]-=ANIMATION_DELTA;
      }
      for(int n=0; n<BLINKSTEPS-1-l; n++) {
        brightness[l+n+1]+=ANIMATION_DELTA;
      }
      set_blink_leds(left, brightness, sizeof(brightness)/sizeof(int)); 
    }

    for(int o = 0; o < BLINKSTEPS; o++) {
      for(int p = 0; p < BLINKSTEPS-o; p++) {
        brightness[o+p] -= ANIMATION_DELTA;
      }
      set_blink_leds(left, brightness, sizeof(brightness)/sizeof(int)); 
    }
    
    delay(500);
}

void set_blink_leds(boolean left, int brightness[], int number) {
  if(left) {
    set_relative_color(back_left_index, NUMBER_BACK,brightness[0], GREEN_FACTOR_YELLOW_BAR, BLUE_FACTOR_YELLOW_BAR);
    
  } else {
    set_relative_color(back_right_index, NUMBER_BACK, brightness[BLINKSTEPS-1], GREEN_FACTOR_YELLOW_BAR, BLUE_FACTOR_YELLOW_BAR);
    
  }

  for (int z = 0; z < number; z++) { 
    if (left) {
      set_relative_color(ring_index[z], brightness[z], GREEN_FACTOR_YELLOW_RING, BLUE_FACTOR_YELLOW_RING);
    }
    else {
      set_relative_color(ring_index[BLINKSTEPS-1-z],brightness[z], GREEN_FACTOR_YELLOW_RING, BLUE_FACTOR_YELLOW_RING);
    } 
  }
  pixels.show();
  delay(40);
}

void shut_down() {
  fade_on(back_left_index, NUMBER_BACK, off_color, true);
  fade_on(back_right_index, NUMBER_BACK, off_color, true);
  fade_on(ring_index, NUMBER_RING, off_color, true);
  ESP.deepSleep(10000000);
}

void setup() {

  Serial.begin(115200); //Initialize Serial interface
  delay(1000);
  Serial.println("\n");
  Serial.flush();
  Serial.println("----------------------------");
  Serial.println("Project: photON");
  Serial.println("\n");
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED && cnt < 30) {
    delay(500);
    Serial.print(F("."));
    cnt++;
  }
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println(F("WiFi connected"));
    server.on("/", send_page);
    server.begin();
    Serial.println(F("Server started"));
    // Print the IP address
    Serial.println(WiFi.localIP());
    flash(ring_index, NUMBER_RING, green, 2);
  } else {
    flash(ring_index, NUMBER_RING, yellow, 2);
  }
  startup_animation();
}

enum blink_state {
  off,
  left,
  right
};
enum blink_state curr_blink_state;

enum light_back_state {
  back_on,
  back_off
};
enum light_back_state back_state;

void loop() {
  server.handleClient();
  if (server.arg("shutdown")=="true") {
    shut_down();
  }
  else if (server.arg("back") == "on" && back_state == back_off) {
    fade_on(back_left_index, NUMBER_BACK, red, false);
    fade_on(back_right_index, NUMBER_BACK, red, false);
    fade_on(ring_index, NUMBER_RING, red, true);
    back_state = back_on;
    return;
  }
  else if (server.arg("back") == "off" && back_state == back_on) {
    fade_on(back_left_index, NUMBER_BACK, off_color, false);
    fade_on(back_right_index, NUMBER_BACK, off_color, false);
    fade_on(ring_index, NUMBER_RING, off_color, true);
    back_state = back_off;
    return;
  }
  switch(curr_blink_state){
    case off:
      if(server.arg("blink")=="left") {
        curr_blink_state = left;
      }
      else if(server.arg("blink")=="right") {
        curr_blink_state = right;
      }
      break;
    case left:
      if(server.arg("blink")=="left") {
        _blink(true);
      }
      else if(server.arg("blink")=="right") {
        if(back_state == back_on) fade_on(back_left_index, NUMBER_BACK, red, false);
        curr_blink_state = right;
      }
      else if(server.arg("blink")=="off") {
        if(back_state == back_on) fade_on(back_left_index, NUMBER_BACK, red, false);
        if(back_state == back_on) fade_on(ring_index, NUMBER_RING, red, true);
        curr_blink_state = off;
      }
      break;
    case right:
      if(server.arg("blink")=="left") {
        if(back_state == back_on) fade_on(back_right_index, NUMBER_BACK, red, false);
        curr_blink_state = left;
      }
      else if(server.arg("blink")=="right") {
        _blink(false);
      }
      else if(server.arg("blink")=="off") {
        if(back_state == back_on) fade_on(back_right_index, NUMBER_BACK, red, false);
        if(back_state == back_on) fade_on(ring_index, NUMBER_RING, red, true);
        curr_blink_state = off;
      }
      break;
  }
}
