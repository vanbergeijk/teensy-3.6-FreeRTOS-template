Teensy 3.6 FreeRTOS Project Template
====================================

Purpose
-------

Starting point for a Teensy 3.x RTOS project based on Teensyduino 1.32 and FreeRTOS 9.0.0
to be used without the Arduino IDE and build environment. 


Setup
-----

Install the Teensy udev rule: `sudo cp tools/49-teensy.rules /etc/udev/rules.d/`

Then unplug your Teensy and plug it back in.


Using
-----

1. Put your code in `src/main.cpp`
2. Put any libraries you need in `libraries`
3. Set the TEENSY variable in `Makefile` according to your teensy version
4. Build your code ```make```
5. Upload your code ```make upload```


Make Targets
------------

- `make` alias for `make hex`
- `make build` compiles everything and produces a .elf
- `make hex` converts the elf to an intel hex file
- `make post_compile` opens the launcher with the correct file
- `make upload` uploads the hex file to a teensy board
- `make reboot` reboots the teensy


Where everything came from
--------------------------

- This description and template project is taken from [Teensy 3.X Project Template](https://github.com/apmorton/teensy-template)
- The `FreeRTOS` sub-folder is taken from [FreeRTOS](http://www.freertos.org/)
- The `teensy3` sub-folder is taken from [The Teensy 3 Cores](https://github.com/PaulStoffregen/cores/tree/master/teensy3)
- The `tools` sub-folder is taken from [Teensyduino](http://www.pjrc.com/teensy/td_download.html)
- The `src/main.cpp` file is moved, unmodified from `teensy3/main.cpp`
- The `49-teensy.rules` file is taken from [PJRC's udev rules](http://www.pjrc.com/teensy/49-teensy.rules)

**Modifications to `Makefile` include**

- Add support for FreeRTOS

**Modifications to `teensy3/FreeRTOSConfig.h` include**

- Disable `configCREATE_LOW_POWER_DEMO`: to keep delay() and micros() functions in arduino files operational
- Keep `configTICK_RATE_HZ` at 1000: anything else than 1000 Hz causes timing trouble in arduino files
- Disabled several handlers not required for this initial port

**Modifications to `teensy3/mk20dx128.c` include**

>add

    #include "FreeRTOS.h"
    #include "task.h"

>add FreeRTOS handler prototypes and replace SysTickHandler

    // from FreeRTOS port
    void xPortPendSVHandler( void ) __attribute__ (( naked ));
    void xPortSysTickHandler( void );
    void vPortSVCHandler( void ) __attribute__ (( naked ));

    // from Arduino port
    extern volatile uint32_t systick_millis_count;

    __attribute__ (( weak, naked )) void systick_isr(void) {
	// increment the systick counter
	systick_millis_count += 1000/configTICK_RATE_HZ;

	// unconditionally branch to the systick handler
	__asm volatile (
		"B	xPortSysTickHandler"
	);
    }

>comment out isr's that will be replaced by FreeRTOS routines

    void usage_fault_isr(void)	__attribute__ ((weak, alias("fault_isr")));
    //void svcall_isr(void)		__attribute__ ((weak, alias("unused_isr")));
    void debugmonitor_isr(void)	__attribute__ ((weak, alias("unused_isr")));
    //void pendablesrvreq_isr(void)	__attribute__ ((weak, alias("unused_isr")));
    //void systick_isr(void)		__attribute__ ((weak, alias("systick_default_isr")));

>add FreeRTOS isr's

	fault_isr,					// 10 --
	vPortSVCHandler,				// 11 ARM: Supervisor call (SVCall)
	debugmonitor_isr,				// 12 ARM: Debug Monitor
	fault_isr,					// 13 --
	xPortPendSVHandler,				// 14 ARM: Pendable req serv(PendableSrvReq)
	systick_isr,					// 15 ARM: System tick timer (SysTick)

>comment out SysTick counter initialization

    // initialize the SysTick counter
    // next 4 lines are commented out for FreeRTOS port, FreeRTOS configures SysTick
    //	SYST_RVR = (F_CPU / 1000) - 1;
    //	SYST_CVR = 0;
    //	SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
    //	SCB_SHPR3 = 0x20200000;  // Systick = priority 32


**Modifications to `teensy3/mk66fx1m0.ld` include**

> added heap memory section (add this to other .ld files when porting to other Teensy 3.x boards)

	.heap (NOLOAD) : {
		. = ALIGN(4);
		*(.heapsection)
	} > RAM

**Modifications to `teensy3/pins-teensy.c` include**

>added delay_NoSysTick function that can safely be called before the SysTick counter is configured
- replace calls to delay() by call to delay_NoSysTick()

	analog_init();
	// for background about this startup delay, please see these conversations
	// https://forum.pjrc.com/threads/36606-startup-time-(400ms)?p=113980&viewfull=1#post113980
	// https://forum.pjrc.com/threads/31290-Teensey-3-2-Teensey-Loader-1-24-Issues?p=87273&viewfull=1#post87273
	delay_NoSysTick(400);
	usb_init();


Sources that were very helpful for this Teensy FreeRTOS port
--------------------------------------------------------------

- Rishi Franklins [blog](http://rishifranklin.blogspot.com/2014/03/freertos-on-teensy-31.html) and [repo](https://github.com/circuitsenses/Teensy-3.1-FreeRTOS)
- HydroSense [repo](https://github.com/HydroSense/teensy-freertos)
- bastl-instruments [repo](https://github.com/bastl-instruments/teensy-RTOS-template)

Thank you all for posting your experiences!

Disclaimer
----------
This FreeRTOS port has only been tested on a Teensy 3.6 board with no real world application yet. If you find issues please post them or create a pull request.

