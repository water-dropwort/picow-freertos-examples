#ifndef SIMPLE_TASK_HPP_
#define SIMPLE_TASK_HPP_

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

void taskCode(void* pvParams)
{
  while(true) {
    Serial.printf("TASK:%ld\n", (uintptr_t)pvParams);
    vTaskDelay(2000 / portTICK_RATE_MS);
  }
}

TaskHandle_t g_taskhandle = NULL;

void setup()
{
  Serial.begin(115200);
  pinMode(15, INPUT_PULLUP);

  xTaskCreate(taskCode
              ,"Test"
              ,configMINIMAL_STACK_SIZE
              ,(void*)123
              ,tskIDLE_PRIORITY
              ,&g_taskhandle);
}

void loop()
{
  if(digitalRead(15) == LOW) {
    vTaskDelete(g_taskhandle);
    Serial.println("Task has been deleted.");
  }
}

#endif
