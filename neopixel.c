#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include <stdlib.h>
#include <time.h>

uint32_t framebuffer[150][15]; 
uint32_t snowflake_framebuffer[150][15]; //this stores snowflakes only

const uint panel_pins[10] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10};
uint8_t green = 0, red = 1, blue = 0;
void send_pixels(PIO pio, int sm, uint32_t *pixels) {
    for (int i = 0; i < 225; i++) {
        uint32_t color = pixels[i];
        pio_sm_put_blocking(pio, sm, color << 8u); // Shift left to match WS2812 timing
    }
}
void framebuffer_clear(void){
    for (int i = 0; i<150; i++){   //framebuffer clear
            for(int j = 0; j< 15; j++ ){
                framebuffer[i][j] = 0;
            }
        }
}
void snowflake_framebuffer_clear(void){
    for (int i = 0; i<150; i++){   //snowflake_framebuffer clear
            for(int j = 0; j< 15; j++ ){
                snowflake_framebuffer[i][j] = 0;
            }
        }
}
void snowy(int x) {
    int temp[15];
    for (int i = 0; i<15; i++){
        snowflake_framebuffer[149][i] = 0;
    }
    for (int i = 0; i < 15; i++) temp[i] = snowflake_framebuffer[150 - 1][i];
    for (int i = 150 - 1; i > 0; i--)
        for (int j = 0; j < 15; j++) snowflake_framebuffer[i][j] = snowflake_framebuffer[i - 1][j];
    for (int i = 0; i < 15; i++) snowflake_framebuffer[0][i] = temp[i];
    if (x==5){
        for (int i = 0; i < 2; i++) {
        int random_number = rand() % 15;
        snowflake_framebuffer[0][random_number] = ((uint32_t)20 << 16) | ((uint32_t)20 << 8) | 20;}
    }
}

int main() {
    stdio_init_all();
    srand(time(NULL));
    
    snowflake_framebuffer_clear();
    framebuffer_clear();

    // Initialize PIO and state machines for each strip
    PIO pios[10];
    int sm[10];
    uint offset[10];
    for (int i = 0; i < 10; i++) {
        if (i < 4) {
            pios[i] = pio0; // First 4 strips on pio0
        } else if (i < 8) {
            pios[i] = pio1; // Next 4 strips on pio1
        } else {
            pios[i] = pio2; // Last 2 strips on pio2
        }

        sm[i] = i % 4; // Each PIO has 4 state machines
        offset[i] = pio_add_program(pios[i], &ws2812_program);
        ws2812_program_init(pios[i], sm[i], offset[i], panel_pins[i], 800000, false);
    }   
    int counter = 0;
        int row = 0;
        while (true) {
            snowy(counter);
            uint32_t data_of_225_leds[225];
            for (int strip = 0; strip < 10; strip++) {
                int z = 0;
                for (int i = ( 0 + (strip*15)) ; i < (15 + (strip*15)) ; i++ ){
                    for( int j = 0; j < 15;j++){
                    if (j % 2 ==0){
                        if (z%2==0){
                            data_of_225_leds[z] = framebuffer[i][14-j];
                        }
                        else {
                            data_of_225_leds[z] = framebuffer[i][j];
                        }}
                    else{
                        if (z%2==1){
                            data_of_225_leds[z] = framebuffer[i][14-j];
                        }
                        else {
                            data_of_225_leds[z] = framebuffer[i][j];
                        }}
                        z++ ;
                    }}
            send_pixels(pios[strip], sm[strip], data_of_225_leds ); //need to send an array of 225 led's data
            
        }
        
        
        
        
        for (int i = 0; i<150; i++){
            for(int j = 0;j<15;j++){
                framebuffer[i][j] = snowflake_framebuffer[i][j];
            }
            
        }

        for (int i = 6; i < 9; i++){    //Golden
                framebuffer[148][i] = ((uint32_t)10 << 16) | ((uint32_t)30 << 8) | 0;
                framebuffer[149][i] = ((uint32_t)10 << 16) | ((uint32_t)30 << 8) | 0;  

                }

        framebuffer[122][7] = ((uint32_t)20 << 16) | ((uint32_t)50 << 8) | 0;
        for (int i = 6; i < 9; i++){
                framebuffer[123][i] = ((uint32_t)20 << 16) | ((uint32_t)50 << 8) | 0;  
                framebuffer[126][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[124][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
        }
        for (int i = 5; i < 10; i++){
                framebuffer[125][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[129][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[127][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;

        }
        for (int i = 4; i < 11; i++){
                framebuffer[128][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[133][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[131][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[130][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                
        }
        for (int i = 3; i < 12; i++){
                framebuffer[132][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[137][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[135][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[134][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;

        }
        for (int i = 2; i < 13; i++){
                framebuffer[136][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[141][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[139][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[138][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;

        }
        for (int i = 1; i < 14; i++){   //Green
                framebuffer[140][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[142][i] = ((uint32_t)20 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[143][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[145][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[147][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;       
                }

        for (int i = 0; i < 15; i++){
                framebuffer[146][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;
                framebuffer[144][i] = ((uint32_t)10 << 16) | ((uint32_t)0 << 8) | 0;      
                }

        if (counter<5){counter++;}
        else{counter=0;}

        if (row<14){row++;}
        else{row=0;}

        sleep_ms(10);
        }
    return 0;
}