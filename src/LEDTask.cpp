
#include <kinetis.h>

#include <FreeRTOS.h>
#include <task.h>

int LED_CONFIG = 0x00000102;
int LED_MASK   = 0x00000020;
int counter = 0;

#include <Arduino.h>

int ledPin = 13;

/** calibration factor for delayMS */
#define CAL_FACTOR (F_CPU/6007)
/** delay between led error flashes
 * \param[in] millis milliseconds to delay
 */
void testDelayMS(uint32_t millis) {
  uint32_t iterations = millis * CAL_FACTOR;
  uint32_t i;
  for(i = 0; i < iterations; ++i) {
    asm volatile("nop\n\t");
  }
}

void LEDTask(void* args) {
  pinMode(ledPin, OUTPUT);

  for(;;){
//    if (counter++ % 2) {
//      digitalWrite(ledPin, LOW);
//    } else {
//      digitalWrite(ledPin, HIGH);
//    }

    // test different delays, monitor with oscilloscope to verify timing
    digitalWrite(ledPin, LOW);
    testDelayMS(1);
    digitalWrite(ledPin, HIGH);
    testDelayMS(1);

    digitalWrite(ledPin, LOW);
    delay(1);
    digitalWrite(ledPin, HIGH);
    delay(1);

    digitalWrite(ledPin, LOW);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    digitalWrite(ledPin, HIGH);
    vTaskDelay(1 / portTICK_PERIOD_MS);

    digitalWrite(ledPin, LOW);
    delay(5);
    digitalWrite(ledPin, HIGH);
    delay(5);

    digitalWrite(ledPin, LOW);
    testDelayMS(5);
    digitalWrite(ledPin, HIGH);
    testDelayMS(5);

    digitalWrite(ledPin, LOW);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    digitalWrite(ledPin, HIGH);
    vTaskDelay(5 / portTICK_PERIOD_MS);

    digitalWrite(ledPin, LOW);
    testDelayMS(20);
    digitalWrite(ledPin, HIGH);
    testDelayMS(20);

    digitalWrite(ledPin, LOW);
    testDelayMS(100);
    digitalWrite(ledPin, HIGH);
    testDelayMS(100);

    digitalWrite(ledPin, LOW);
    testDelayMS(500);
    digitalWrite(ledPin, HIGH);
    testDelayMS(500);

//    vTaskDelay(500);
  }
}

/* pure ARM configuration
  // setup
  PORTC_PCR5 = LED_CONFIG;
  GPIOC_PDDR |= LED_MASK;

  // execution
  for(;;) {
    if (counter++ % 2) {
      GPIOC_PDOR |= LED_MASK;
    } else {
      GPIOC_PDOR &= ~LED_MASK;
    }
  }
*/
