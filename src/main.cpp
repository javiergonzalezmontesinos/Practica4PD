#include <Arduino.h>

// Declaración anticipada de la función (prototipo)
void anotherTask(void *parameter);

void setup() {
  Serial.begin(115200);
  
  xTaskCreate(
    anotherTask,       // Función de la tarea
    "another Task",    // Nombre de la tarea
    10000,             // Tamaño del stack (bytes)
    NULL,              // Parámetros
    1,                 // Prioridad (0-24)
    NULL               // Task handle
  );                   // <-- Corregido el cierre
}

void loop() {
  Serial.println("this is ESP32 Task");
  delay(1000);         // Usar vTaskDelay sería mejor, pero funciona
}

// Implementación de la tarea
void anotherTask(void *parameter) {
  for (;;) {           // Bucle infinito
    Serial.println("this is another Task");
    delay(1000);       // Equivalente a vTaskDelay(1000/portTICK_PERIOD_MS)
  }
  // vTaskDelete(NULL); // Eliminar esta línea (es innecesaria)
}