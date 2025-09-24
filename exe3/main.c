#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int A[5] = {0,0,0,0,0};
    double soma = 0;

    while (true) {


        if (xQueueReceive(xQueueData, &data, 100)) {
            // implementar filtro aqui!
            
            
            int temp1 = A[1];
            int temp2 = A[2];
            int temp3 = A[3];
            int temp4 = A[4];

            soma -= A[0];
            soma += data;

            A[0] = temp1;
            A[1] = temp2;
            A[2] = temp3;
            A[3] = temp4;
            A[4] = data;
            
            int media;

            

            media = soma / 5;

            printf("media movel = %d\n" , media);
            

            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
