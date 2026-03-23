# Goal
I need a way of triggering the switch between Z and θ control via G-code, as that's the only way the slicer can interact with the printer.

# Potential Solutions
## Use serial data with an external microcontroller
### Description
Prusa outputs serial with the `M118` command via a USB-C port on the Buddy board in the back of printer which could also power an ESP32 to read the serial line and control motor switching.

### Verifying M118 works
I connected my laptop to the Prusa XL via the USB-C port and listened recorded serial data via the "Serial Monitor" extension of VSCode. This is what was output for a minimalist G-code script that included `M118 Hello World` between the manditory start and end G-code sections:
```
File opened: /usb/SERIAL~1.BGC Size:1342
FIRMWARE_NAME:Prusa-Firmware-Buddy 6.2.6+8948 (Github) SOURCE_CODE_URL:https://github.com/prusa3d/Prusa-Firmware-Buddy PROTOCOL_VERSION:1.0 MACHINE_TYPE:Prusa-XL EXTRUDER_COUNT:5 UUID:cede2a2f-41a2-4748-9b12-c55c62f367ff
Cap:SERIAL_XON_XOFF:0
Cap:BINARY_FILE_TRANSFER:0
Cap:EEPROM:0
Cap:VOLUMETRIC:1
Cap:AUTOREPORT_TEMP:1
Cap:PROGRESS:0
Cap:PRINT_JOB:1
Cap:AUTOLEVEL:1
Cap:Z_PROBE:1
Cap:LEVELING_DATA:1
Cap:BUILD_PERCENT:0
Cap:SOFTWARE_POWER:0
Cap:TOGGLE_LIGHTS:0
Cap:CASE_LIGHT_BRIGHTNESS:0
Cap:EMERGENCY_PARSER:0
Cap:PROMPT_SUPPORT:0
Cap:AUTOREPORT_SD_STATUS:0
Cap:THERMAL_PROTECTION:1
Cap:MOTION_MODES:0
Cap:CHAMBER_TEMPERATURE:0
Hello World
Done printing file
```
As you can see above, the serial output works!

### Verify the ESP32 can recieve serial data from Prusa
First, I wrote [some code](../../code/serial-communication-testing/) that would flash a hardware LED on the ESP32 when it recieved `Flash` via the serial line.

I then wrote [G-code](serial%20test.gcode) to blink the LED via serial. I first connected the printer to my laptop to read its serial output to verify the G-code was working as expected, which it was. When plugging the ESP32 into the Prusa, it did not power on, indicating that USB-C on the back of the prusa doesn't provide power to peripheral components. For the sake of running the test, I connected the ESP32 to a 5V power supply via the Vin and Gnd pins. The ESP32 did not respond to the serial data from the printer.

The issue is likely that the ESP32 is not and can't be a USB host. I need an ESP32 S2 to do that. I'll try

## Use the print fan of a docked extruder to do switching control
### Description
Hijack the power supplied to the print fan of a docked relay to control relays which switch motor control.

### Critical issue!
As far as I'm aware, there's no way to control the part fan of a *docked* extruder or any other signals via G-code that could be hijacked to control a set of relays.
