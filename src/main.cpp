#include <Arduino.h>
#include <FreeRTOS.h>

// Pin del LED
const int LED_PIN = 2;

// Semáforo binario
SemaphoreHandle_t ledSemaphore;

// Tarea para encender el LED (se ejecuta en el núcleo 0)
void taskEncender(void *parameter) {
  for (;;) {  // Bucle infinito
    xSemaphoreTake(ledSemaphore, portMAX_DELAY);  // Espera el semáforo
    digitalWrite(LED_PIN, HIGH);  // Enciende el LED
    Serial.println("LED encendido (Núcleo " + String(xPortGetCoreID()) + ")");
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 1 segundo
    xSemaphoreGive(ledSemaphore);  // Libera el semáforo
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 1 segundo antes de repetir
  }
}

// Tarea para apagar el LED (se ejecuta en el núcleo 1)
void taskApagar(void *parameter) {
  for (;;) {  // Bucle infinito
    xSemaphoreTake(ledSemaphore, portMAX_DELAY);  // Espera el semáforo
    digitalWrite(LED_PIN, LOW);  // Apaga el LED
    Serial.println("LED apagado (Núcleo " + String(xPortGetCoreID()) + ")");
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 1 segundo
    xSemaphoreGive(ledSemaphore);  // Libera el semáforo
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Espera 1 segundo antes de repetir
  }
}

void setup() {
  Serial.begin(115200);

  // Configura el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Crea el semáforo binario
  ledSemaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(ledSemaphore);  // Inicializa el semáforo como "liberado"

  // Crea la tarea para encender el LED y la asigna al núcleo 0
  xTaskCreatePinnedToCore( 
    taskEncender,  // Función de la tarea
    "Encender LED",  // Nombre de la tarea
    1000,  // Tamaño del stack
    NULL,  // Parámetros
    1,  // Prioridad
    NULL,  // Task handle
    0  // Núcleo 0
  );

  // Crea la tarea para apagar el LED y la asigna al núcleo 1
  xTaskCreatePinnedToCore( 
    taskApagar,  // Función de la tarea
    "Apagar LED",  // Nombre de la tarea
    1000,  // Tamaño del stack
    NULL,  // Parámetros
    1,  // Prioridad
    NULL,  // Task handle
    1  // Núcleo 1
  );
}

void loop() {
  // No es necesario usar el loop() en este caso
}