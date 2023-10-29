#ifndef TASK_SEMAPHORE_HPP_
#define TASK_SEMAPHORE_HPP_

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

TaskHandle_t g_taskhandle = NULL;
SemaphoreHandle_t g_semaphore1 = NULL;

void taskCode(void* pvParams)
{
  while(true) {
    if(xSemaphoreTake(g_semaphore1, (TickType_t)0) == pdTRUE) {
      Serial.println("TASK:Succ Semaphore Take");
      
      vTaskDelay(2000 / portTICK_RATE_MS);
      xSemaphoreGive(g_semaphore1);
    }
    else {
      Serial.println("TASK:Fail Semaphore Take");
    }
    vTaskDelay(2000 / portTICK_RATE_MS);
  }
}


void setup()
{
  Serial.begin(115200);
  pinMode(15, INPUT_PULLUP);

  delay(5000);

  g_semaphore1 = xSemaphoreCreateBinary();
  if(g_semaphore1 == NULL) {
    Serial.println("Semaphore is NULL");
  }
  else {
    Serial.println("Semaphore is NOT NULL");
  }
  xSemaphoreGive(g_semaphore1);

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
      if(xSemaphoreTake(g_semaphore1, (TickType_t)0) == pdTRUE) {
        Serial.println("C0:Succ Semaphore Take");
        Serial.println("C0:Start Delay");
        delay(5000);
        Serial.println("C0:End Delay");
        xSemaphoreGive(g_semaphore1);
      }
      else {
        Serial.println("C0:Fail Semaphore Take");
      }
    }
    while(digitalRead(15) == LOW);
  }
}


#endif
