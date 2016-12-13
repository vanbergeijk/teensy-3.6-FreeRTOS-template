#include <kinetis.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>

// extern tasks declared elsewhere in program
void LEDTask(void* args);
void SerialTask(void* args);

int main() {
  // create the tasks
  xTaskCreate(LEDTask, "LT", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(SerialTask, "ST", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // start scheduler, main should stop functioning here
  vTaskStartScheduler();

  for(;;);

  return 0;
}

/*
// FreeRTOS malloc port function
void* pvPortMalloc(size_t size) {
  return malloc(size);
}

// FreeRTOS free port function
void vPortFree(void* ptr) {
  free(ptr);
}
*/

/*
#include "WProgram.h"
#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

FlexCAN CANbus0(250000, 0);
FlexCAN CANbus1(250000, 1);

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";


// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}

extern "C" int main(void)
{
#ifdef USING_MAKEFILE

uint8_t msgCnt = 0;

  CANbus0.begin();
  CANbus1.begin();

  delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

	pinMode(13, OUTPUT);

	while (1) {
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);


    msg.id = 0x18FEEF80;
    msg.ext = 1;
    msg.len = 8;
    msg.timeout = 0; // milliseconds, zero will disable waiting
    msg.buf[0] = msgCnt;
    msg.buf[1] = msgCnt;
    msg.buf[2] = msgCnt;
    msg.buf[3] = msgCnt;
    msg.buf[4] = msgCnt;
    msg.buf[5] = msgCnt;
    msg.buf[6] = msgCnt;
    msg.buf[7] = msgCnt;
    msgCnt++;

    CANbus0.write(msg);
    CANbus1.write(msg);

  if(CANbus0.available()) 
  {
    CANbus0.read(msg);
    Serial.print("CAN bus 0: "); hexDump(8, msg.buf);
    CANbus1.write(msg);
  }

  if(CANbus1.available()) 
  {
    CANbus1.read(msg);
    Serial.print("CAN bus 1: "); hexDump(8, msg.buf);
    CANbus0.write(msg);
  }


	}


#else
	// Arduino's main() function just calls setup() and loop()....
	setup();
	while (1) {
		loop();
		yield();
	}
#endif
}
*/


