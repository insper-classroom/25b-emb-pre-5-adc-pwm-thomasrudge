/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/gpio.h"
 #include "hardware/timer.h"
 #include "hardware/adc.h"
 
 const int PIN_LED_B = 4;
 
 const float conversion_factor = 3.3f / (1 << 12);
 
 /**
  * 0..1.0V: Desligado
  * 1..2.0V: 150 ms
  * 2..3.3V: 400 ms
  */

volatile int timer_on = 0;
volatile int azul = 0;
bool timer_1_callback(repeating_timer_t *rt) {
    azul = !azul;
    gpio_put(PIN_LED_B , azul);
    timer_on = 0;
   
    
    return false; // keep repeating
}

 int main() {
    stdio_init_all();
    
    adc_init();
    adc_gpio_init(28);

    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_B , GPIO_OUT);
     

    uint16_t result;
    float tensao;
    repeating_timer_t timer_0;
    adc_select_input(2); // Select ADC input 1 (GPIO27)
    

    while (1) {

        
        result = adc_read();
        sleep_ms(100);
        tensao = result * conversion_factor;
        
        
        if (tensao < 1){
            timer_on = 0;
            gpio_put(PIN_LED_B , 0);
        }

        
        
        if (tensao > 1 && !timer_on){
            
            
            

            if(tensao < 2){
                
                add_repeating_timer_ms(150, timer_1_callback, NULL, &timer_0);
                timer_on = 1;

            }
            else{
                
                add_repeating_timer_ms(450, timer_1_callback, NULL, &timer_0);
                timer_on = 1;

            }

        }
        
        

     }
 }
