#ifdef ARDUINO_ARCH_ESP32
  TaskHandle_t Task1;

  void Task1code( void * parameter) {
    setup1();
  for(;;) {
    #if CONFIG_FREERTOS_UNICORE
        yieldIfNecessary();
    #endif

    loop1();  
  }
}
#endif

void MultiCoreInit()
{
  #ifdef ARDUINO_ARCH_ESP32

  int core = xPortGetCoreID();

  if (core == 0)
    core = 1;
    else
    core = 0;


  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
       getArduinoLoopTaskStackSize() ,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &Task1,  /* Task handle. */
      core); /* Core where the task should run */

  #endif
}
