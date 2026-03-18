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
I printed test samples with modified G-code: `M84 Z` and `M17` were added to bracket each perimeter, external perimeter, infill, and solid infill print sequence such all movements during printing would have inactive Z-axis motors. The code I used for this modification can be found [here].

I obseved the Z-axis motor with an oscilloscope during printing to verify that it was being properly disable, but it wasn't. My theory is that the movement commands `G0/G1` automatically activate all steppers, even if the Z axis isn't used in the move. To verify this, I modified the [G-code used](4th-Axis-Control/Switching-Z-and-Theta-Control/Disabling-Motors-for-Switching/Z Disable Enable Test.gcode) to test if `M84` and `M17` were working to be the following repeated section
```
M84 Z
G4 P1000
G1 X20 Y20
G4 P100
```
If `G0/G1` does actually enable all steppers, then the above code should disable the Z motors, wait 1s, enable the Z motors, then wait 1s.
