# Project Name

Brief project description.

## Table of Contents

- [Introduction](#introduction)
- [Hardware](#hardware)
- [Development Environment](#development-environment)
- [Getting Started](#getting-started)
- [Code Structure](#code-structure)
- [Dependencies](#dependencies)
- [License](#license)

## Introduction

Provide a brief overview of the project, its purpose, and key features.

## Hardware

- MCU: GD32F30x
- LTE+GPS: Quectel EC200M
- RFID: CR95HF

## Development Environment

- IDE: KEIL C

## Getting Started

<h2>1. Download Keil C</h2> 

If you haven't already, download and install Keil C from the official website: [Keil C Download](https://www.keil.com/download/)

### 2. Unzip GPIO+EXTI+USART.rar

- Locate the `GPIO+EXTI+USART.rar` file on your computer.
- Extract the contents to a convenient location.

### 3. Open Project with Keil C

- Open Keil C.
- Navigate to `Project` > `Open Project...`.
- Select the `GPIO.uvprojx` file from the extracted contents and click "Open."

### 4. Review Project Files

Explore the project files, including source code files (`.c`), header files (`.h`), and any other relevant files.

### 5. Build the Project

Click on the "Build" button or go to `Project` > `Build Target` to compile the project.

### 6. Configure Target Device

Ensure that the target device (GD32F30x MCU) is correctly configured in the project settings.

### 7. Connect Hardware

Connect the GD32F30x MCU or the target hardware to your computer.

### 8. Flash the Program

Use the appropriate flashing tool or method to flash the compiled program onto the target device.

### 9. Run the Program

After flashing the program, run the application on the target device. Monitor the output through the USART or any other communication interface.

### 10. Troubleshooting

If there are any issues during compilation or execution, review the error messages in the Keil C output window. Check hardware connections and configurations.

### 11. Explore Documentation

Check for any accompanying documentation (such as `readme.md` or HTML documentation) for additional instructions, usage details, or specific features of the project.

## Code Structure

The project follows a modular structure. Each functionality is encapsulated in a separate module.

- [AT Commands for Quectel EC200M](./atc_e25)
- [EEPROM/Flash Handling](./e24_flash)
- [GPS Module](./GPS)
- [TCP Communication](./tcp)
- [CR95HF RFID Module](./CR95HF)

For more detailed information about each module, refer to their respective documentation.

## Dependencies

The project relies on the following dependencies:

- [gd32f30x.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_gpio.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_misc.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_pmu.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_rcu.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_spi.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_usart.h](https://github.com/gd32-community/gd32-pkg)
- [gd32f30x_it.h](https://github.com/gd32-community/gd32-pkg)
- <string.h>

Ensure that you have the necessary versions of these dependencies installed or included in your project. If any links are provided, consider checking them for the latest versions or documentation.

## License

Specify the project's license information.
