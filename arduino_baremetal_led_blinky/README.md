# Bare_Metal LED Blink on Arduino

This repository contains a **bare-metal LED blinking program** for the Arduino Uno which uses ATmega328P, written **without using the Arduino framework**.  
The code directly programs the **ATmega328P** microcontroller registers to toggle the onboard LED connected to **PORTB5**.

If you're looking for `digitalWrite()`, this is not that project.

---

## Target Hardware

- **Board:** Arduino Uno R3
- **MCU:** ATmega328P
- **Clock:** 16 MHz external crystal
- **LED Pin:** PORTB5 (Arduino digital pin 13, onboard LED)

---

## What This Project Demonstrates

- Direct register-level programming (no Arduino core, no libraries)
- Configuring GPIO using `DDRB`
- Toggling output pins via `PORTB`
- Busy-wait delay using simple loops
- Minimal bare-metal setup suitable for learning AVR internals

---

## Build Requirements

You need the AVR toolchain installed:

- `avr-gcc`
- `avr-binutils`
- `avr-libc`
- `avrdude`

On Debian/Ubuntu:
```bash
sudo apt install avr-gcc avr-libc avrdude
```

## Building the Project

To build the project, navigate to the project directory and run the following commands:

First create a executable file named `blink.elf`:
```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o blink.elf blink.c
```

Then create a hex file from the executable:
```bash
avr-objcopy -O ihex blink.elf blink.hex
```

Finally, upload the hex file to the Arduino Uno using `avrdude`:
(make sure to connect the Arduino Uno to your computer via USB)
```bash
avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -b 115200 -U flash:w:blink.hex:i
```
