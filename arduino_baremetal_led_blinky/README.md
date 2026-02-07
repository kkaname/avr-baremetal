# Bare-Metal LED Blink on Arduino Uno

A **bare-metal LED blinking program** for the Arduino Uno (ATmega328P), written **without the Arduino framework or any libraries**.

The code directly manipulates ATmega328P registers to configure Timer1 in CTC mode with interrupts to toggle the onboard LED at a 1-second interval.

If you're looking for `digitalWrite()`, this is not that project.

---

## Target Hardware

| Component | Value |
|-----------|-------|
| Board | Arduino Uno R3 |
| MCU | ATmega328P |
| Clock | 16 MHz external crystal |
| LED Pin | PORTB5 (Arduino digital pin 13, onboard LED) |

---

## What This Project Demonstrates

- Direct register-level programming (no Arduino core, no libraries)
- Configuring GPIO using `DDRB` and `PORTB`
- Timer1 configuration in CTC (Clear Timer on Compare) mode
- Hardware interrupt handling with custom ISR (`__vector_11`)
- Prescaler configuration for timing calculations
- Low-power idle using `sleep` instruction
- Minimal bare-metal setup for learning AVR internals

---

## How It Works

The LED blinks at exactly **1 Hz** (1 second on, 1 second off) using Timer1:

```
Timer Frequency = F_CPU / (Prescaler × (1 + OCR1A))
                = 16,000,000 / (256 × 62,500)
                = 1 Hz
```

When Timer1 reaches the compare value (62499), it triggers an interrupt that toggles the LED and resets the counter automatically (CTC mode).

---

## Prerequisites

You need the AVR toolchain installed:

- `avr-gcc` — AVR C compiler
- `avr-binutils` — Binary utilities (includes `avr-objcopy`)
- `avr-libc` — C library for AVR
- `avrdude` — Programmer utility

### Installation

**Debian/Ubuntu:**
```bash
sudo apt install gcc-avr avr-libc binutils-avr avrdude
```

**Fedora/RHEL:**
```bash
sudo dnf install avr-gcc avr-libc avrdude
```

**Arch Linux:**
```bash
sudo pacman -S avr-gcc avr-libc avrdude
```

**macOS (Homebrew):**
```bash
brew tap osx-cross/avr
brew install avr-gcc avrdude
```

---

## Building the Project

Navigate to the `src/` directory:

```bash
cd src
```

### Step 1: Compile to ELF

```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o blink.elf blink.c
```

| Flag | Description |
|------|-------------|
| `-mmcu=atmega328p` | Target microcontroller |
| `-DF_CPU=16000000UL` | Define CPU frequency (16 MHz) |
| `-Os` | Optimize for size |

### Step 2: Convert to Intel HEX format

```bash
avr-objcopy -O ihex -R .eeprom blink.elf blink.hex
```

### Step 3: (Optional) Check binary size

```bash
avr-size --mcu=atmega328p --format=avr blink.elf
```

---

## Uploading to Arduino

Connect your Arduino Uno via USB, then:

### Linux

```bash
avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:blink.hex:i
```

> **Note:** The port might be `/dev/ttyUSB0` or `/dev/ttyACM0` depending on your system.  
> Check with `ls /dev/tty*` or `dmesg | tail` after plugging in.

### macOS

```bash
avrdude -c arduino -p atmega328p -P /dev/cu.usbmodem* -b 115200 -U flash:w:blink.hex:i
```

### Windows

```cmd
avrdude -c arduino -p atmega328p -P COM3 -b 115200 -U flash:w:blink.hex:i
```

> Replace `COM3` with the correct COM port (check Device Manager).

---

## Quick Build & Flash (One-liner)

**Linux:**
```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o blink.elf blink.c && \
avr-objcopy -O ihex -R .eeprom blink.elf blink.hex && \
avrdude -c arduino -p atmega328p -P /dev/ttyACM0 -b 115200 -U flash:w:blink.hex:i
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| `avrdude: ser_open(): can't open device` | Check the port path, ensure Arduino is connected |
| `avrdude: stk500_recv(): programmer is not responding` | Press reset button on Arduino just before upload |
| Permission denied on `/dev/ttyACM0` | Add yourself to `dialout` group: `sudo usermod -aG dialout $USER` (then log out/in) |
| LED not blinking | Verify connections, check if upload succeeded |

---

## Project Structure

```
arduino_baremetal_led_blinky/
├── README.md
└── src/
    └── blink.c      # Main source file
```

---

## License

This project is provided as-is for educational purposes. Feel free to use and modify.
