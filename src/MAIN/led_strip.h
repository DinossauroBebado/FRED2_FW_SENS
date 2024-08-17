#ifndef __CONTROLER__H__
#define __CONTROLER__H__

#include <MAIN/config.h>
#include "config.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 23

Adafruit_NeoPixel  pixels(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);
int long red = 0xFF0000 ;
int long off = 0x000000 ;
int long blue = 0x0000FF ;
int long green = 0x00FF00 ;
int long magenta = 0xFF1493 ;
int long yellow = 0xFFFF00 ;
int long orange = 0xFF4500 ;
int long purple = 0x6B238E  ;
int long white = 0xFFFFFF ;

void led_strip_controler_ros(float color){
    

    if(color == 0){
        pixels.fill(white);
        pixels.show();
    }
    else if(color == 1){
        pixels.fill(blue);
        pixels.show();
    }
    else if(color == 2){
        
        pixels.fill(red);
        pixels.show();
    }
    else if(color == 3){
        pixels.fill(green); 
        pixels.show();
    }
    else if(color == 4){
        pixels.fill(magenta); 
        pixels.show();
    }
    else if(color == 5){
        pixels.fill(yellow); 
        pixels.show();
    }
    else if (color == 6){
        pixels.fill(orange);
        pixels.show();
    }
    else if (color == 7)
    {
        pixels.fill(purple);
        pixels.show();
    }
    else{
        pixels.fill(0x000000);
        pixels.show();
    }
}

void led_strip_controler(int color){
   

    if(color != 0){
        pixels.fill(blue);
        pixels.show();
    }else{
        pixels.fill(0x000000);
        pixels.show();
    }
}
void led_strip_init(){

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.setBrightness(100); // not so bright
    led_strip_controler(0);
}
#endif  //!__CONTROLER__H__
