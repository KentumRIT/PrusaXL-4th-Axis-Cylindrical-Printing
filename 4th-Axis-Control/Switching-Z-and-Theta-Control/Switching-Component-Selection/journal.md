# Goal
I need a set of components to actually switch power delivery between the Z and θ axes without risking damage to either the Prusa's motor controllers, the motors, or the switching components.

# Potential Solutions
## Tandem DPDT Relays driven by microcontrolled MOSFET
### Description
I couldn't find relays on Digikey that have a coil voltage and current the ESP32 can provide (3.3V, 20mA) that are capable of switching the motor load (24V, 5A). I'm getting the 5A figure from the 5A fuse tied to the motor power section of the Prusa Sandwich board. While I'm not sure that 5A is just for the single Z stepper motor, it seems like it'd be good practice to design around blowing the fuse before anything else. To switch the high-power motor lines with the microcontroller, I'll need an intermediate MOSFET controlled by the ESP32 that can provide power to the coils of the relays which switch the motor power.

### Component Selection
- Main switching relay: [J114AF2CS24VDC.53](https://www.digikey.com/en/products/detail/cit-relay-and-switch/J114AF2CS24VDC-53/14001715), 24V coil voltage @ 22.9mA, 110V DC max switching voltage @ 8A
- Flyback diode: TODO
- Microcontrolled MOSFET: TODO
