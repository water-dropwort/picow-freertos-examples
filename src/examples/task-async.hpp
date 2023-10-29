#ifndef TASK_ASYNC_HPP_
#define TASK_ASYNC_HPP_

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

TaskHandle_t g_taskhandle = NULL;
SemaphoreHandle_t g_semaphore = NULL;
bool g_taskCompleted = false;
long g_rand = 0;

void taskCode(void* pvParams)
{
  while(true) {
    if(xSemaphoreTake(g_semaphore, (TickType_t)0) == pdTRUE) {
      g_rand = random(1024);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      Serial.printf("TASK:%ld\n", g_rand);
      xSemaphoreGive(g_semaphore);
    }

    g_taskCompleted = true;
    vTaskSuspend(g_taskhandle);
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(15, INPUT_PULLUP);

  g_semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(g_semaphore);

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
    delay(20);
    if(digitalRead(15) == LOW) {
      vTaskResume(g_taskhandle);
    }
    while(digitalRead(15) == LOW);
  }

  if(g_taskCompleted) {
    g_taskCompleted = false;
    if(xSemaphoreTake(g_semaphore, (TickType_t)0) == pdTRUE) {
      Serial.printf("C0:%ld\n", g_rand);
      xSemaphoreGive(g_semaphore);
    }
  }
}

#endif
