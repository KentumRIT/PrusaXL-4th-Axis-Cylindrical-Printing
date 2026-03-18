# Goal
I need to disable the Z/θ motors before switching them, as collapsing the magnetic field on the motor may induce weird voltage stuff on the motor driver, causing failure.

# Potential Solutions
## Disable Z/θ motors via G-code
### Description
`M84 Z` can be used to turn off the z axis steppers with `M17` turning all motors back on.

### Testing if M84 Really Turns Off Motors
Simple enough, I just need to monitor coil voltage on the Z motors when I give them the `M84 Z` command so that I know I’m really turning them off before potentially switching them while they’re powered and accidentally burning out the motor controller. I did this by measuring the voltage on the coils with an oscilloscope while running the following repeated G-code sippet:
```
M84 Z
G4 P1000
M17
G4 P100
```
The above code should disable the Z motors, wait 1s, enable the Z motors, then wait 1s. On the oscilloscope, we see this happen exactly. A periodic signal of ~24V peak for 1s then a constant signal of 0V for 1s.
