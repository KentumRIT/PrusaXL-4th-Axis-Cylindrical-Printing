# Goal
With this method of switching power between the Z and θ axes, both of them will end up unpowered during printing. The Prusa microsteps its motors, so when they lose power they'll at least want to fall to the closest full-step position. With at least one Z/θ switch per layer and potentially hundreds of layers per part, this error may accumulate to be significant over time. I want to quantify this effect to understand if it will be significant at all.

# Potential Solutions
## Printing blocks with Z axis off for each layer
### Description
I printed 70mm high towers with 35mm x 35mm base. 5 towers arranged in an ‘X’ pattern on the build plate as follows:
| Tower | X pos (mm) | Y pos (mm) |
|-------|------------|------------|
| 1     | 45         | 45         |
| 2     | 45         | 315        |
| 3     | 315        | 45         |
| 4     | 315        | 315        |
| 5     | 180        | 180        |

### Control samples
I printed a set of 5 towers with standard Prusa G-code. The print settings were `0.2mm SPEED` at 10% infill, printer settings are `5T Input Shaper 0.4 nozzle`, material is `Generic PLA`.
TODO: Measure control samples

### Test samples
I printed test samples with modified G-code: `M84 Z` and `M17` were added to bracket each perimeter, external perimeter, infill, and solid infill print sequence such all movements during printing would have inactive Z-axis motors, with an example snippet here:
```
M84 Z
;TYPE:Perimeter
;WIDTH:0.499999
G1 F2400
G1 X28.407 Y331.593 E1.26128
G1 X28.407 Y298.407 E1.26128
G1 X61.593 Y298.407 E1.26128
G1 X61.593 Y331.533 E1.259
M204 P2500
M204 T250
G1 X62.05 Y332.05 F24000
M204 T5000
M204 P500
;TYPE:External perimeter
G1 F2400
G1 X27.95 Y332.05 E1.29601
G1 X27.95 Y297.95 E1.29601
G1 X62.05 Y297.95 E1.29601
G1 X62.05 Y331.99 E1.29373
M204 P2500
G1 E-.64 F2100
M17
```

I obseved the Z-axis motor with an oscilloscope during printing to verify that it was being properly disable, but it wasn't. Power was being delivered to the Z axis motor during printing (after the `M84 Z` G-code command). My initial theory is that the movement commands `G0/G1` automatically activate all steppers, even if the Z axis isn't used in the move. To verify this, I ran the above code snippet in [this G-code](Z%Enable%Via%G1.gcode) and made small changes until I found the issue was that I need a dwell before the `M17` command. Interestingly, the ammount of time the dwell lasts doesn't seem to matter, so even a 1ms delay of `G4 P1` is sufficient. I guess the G-code keeps executing past the current `G1` command until it finds the `M17` command and runs `M17` only milliseconds after it runs `M84 Z`, but adding the dwell term blocks that behavior. I updated the script that inserts the `M84 Z` and `M17` commands to include `G4 P1` before each to fix this behavior and then re-printed the test samples with the edited G-code.

Again, I monitored the Z axis motor during printing to ensure it was properly being disabled during printing and properly enabling for wipe moves and layer changes. It was not of course, because that would be way too easy. I tried now including `M400` "wait for moves" before every `M84 Z` and `M17` command as well as increasing the delay from 1ms to 100ms as well as adding a 100ms delay *after* the commands in addition to the one before. This also didn't work, so I tried just getting rid of all the `M17` commands. This did not work.

Given that when I remove the start G-code from the equation, the first layer code works fine, I think that there's likely a setting or something in the first layer that needs to be changed for this to work. I'll focus on that next.