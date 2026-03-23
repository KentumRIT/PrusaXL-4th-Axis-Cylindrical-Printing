# Goal
We need some way of controlling the 4th axis (θ axis) motor that will drive the angular motion of the cylindrical mandrels that serve as the print bed for this mod. Here are some general requirements for the chosen control method (including mechanical, electrical, and software components):
- Don't interfere with standard un-modded printer functionality or be easy to un-mod and re-mod. It's important to this project that the printer can still function normally with the mandrels removed.
- Sync with at least the X and Y motions of the printer. G-code motion commands (i.e. `G0/G1`) interpolate between defined positions, so the chosen control method must somehow match with that interpolated movement. For example, simply sending position data for θ to an external motor controller via G-code serial commands would be insufficient, as the motor controller wouldn't know at what speed or with what interpolation scheme to move between its current position and the next position.
- Provide theoretical linear positional resolition of at least 0.1mm at a radius of 18.35mm off the rotational axis of the mandrel; this corresponds to an angular resolution of ~0.312°. These values are chosen to keep very good positional resolution at the outer skin of a 15mm thick print on a 1/4" mandrel.
- Provide real positional repeatability of at least 0.312° (same logic as above).
- Provide maximum real linear velocity of at least 60mm/s at a radius of 1/4" (6.35mm) off the rotational axis; this corresponds to an angular speed of ~1.504 RPS. These values are chosen for printing at the surface of a 1/4" mandrel (you can always move slower as your radius increases).

# Potential Solutions
## Formatting Notes
- There are four subcategories for each potential solution.
  - "Description" is just a brief description of the solution.
  - "Benefits" refers to pros inherent to the presented solution that will persist no matter the answers chosen for the solutions' challenges.
  - "Drawbacks" refers to cons inherent to the presented solution that are not changeable within the scope of the solution.
  - "Challenges" refers to foreseen cons of the presented solution that are potentially solvable. 
    - Challenges have the following markings:
      - **[Solved]** describes challenges which have been completely solved and no longer pose any issue.
      - **[Mitigated]** describes challenges for which an answer has been found that only partially solves the issues.
      - **[In Progress]** describes challenges that are not solved and are currently being worked on.
      - **[Unsolved]** describes challenges that haven't been solved and aren't yet being worked on.
    - Challenges in the **[Solved]**, **[Mitigated]**, or **[Unsolved]** states should be linked to a doc or folder containing documentation of the decisions made and work done.

## [Switching Z and θ axes](Switching-Z-and-Theta-Control)
### Description
The PrusaXL firmware/hardware can already handle controlling XYZ steppers simultaneously through the basic `G0/G1` movement commands. We can simply switch the power from being delivered to the Z axis motor to the θ axis motor and vise versa in order to control the θ axis.

### Benefits
- No need for firmware modification. This both avoids a major software headache and means that in the future firmware can be readily updated without worry.
- Potentially zero impact on standard printer functionality when the switching capability is not being used. So long as the switching circuit itself doesn't impact the Z motor dynamics, this solution should be 'invisible' to printer functionality when the switching isn't used.
- Perfect sync between XYθ axes.

### Drawbacks
- No simultaneous Z axis and θ axis control. Any wiping motion that would naturally occur in the θ and Z axes simultaneously is no longer possible.
- Both Z and θ axes become unpowered at some point during printing.

### Challenges
- **[In Progress](Switching-Z-and-Theta-Control/Prusa-Switching-Control/journal.md)** We need to trigger switching between Z and θ axis control via G-code.
- **[In Progress](Switching-Z-and-Theta-Control/Testing-Z-Repeatability/journal.md)** When the θ and Z axes are unpowered they may accumulate positional error.
- **[Solved](Switching-Z-and-Theta-Control/Disabling-Motors-for-Switching/journal.md)** When the Z/θ axes motors are unpowered they may cause large negative voltage spikes via magnetic field collapse.
- **[Unsolved]** We need to match the system characteristics of the θ to the Z axis so that the Z axis motor driver tuning works for both the Z and θ axes.
- **[Unsolved]** It's possible the Z axis drive controller doesn't have enough power for the θ axis.
- **[Unsolved]** We need to find the relationship between Z axis movement to θ axis movement.
- **[Unsolved]** Need to find a way to power ESP32 (USB-C on back of Prusa doesn't provide power)

## Hijack Extruder Stepper Motor
### Description
The PrusaXL has 5 toolheads, so we can just use the extruder stepper on one printhead to drive the theta axis stepper motor.

### Benefits
- No external electronics needed.
- Potential for perfect sync between XYZθ axes given appropriate firmware modification.

### Drawbacks
- The native PrusaXL firmware doesn't let you extrude from a parked tool, so this would require firmware modification.
- This requires a connection to the printhead stepper motor port on the XL Dwarf board, which would render that toolhead non-functional while the printer is modified.

### Challenges
- **[Unsolved]** Updating the firmware so that a parked tool's extruder can be controlled at the same time as the XYZ axes.
- **[Unsolved]** It's possible the extruder drive controller doesn't have enough power for the θ axis.
- **[Unsolved]** We need to match the system characteristics of the θ to the E axis so that the E axis motor driver tuning works for the θ axis.
- **[Unsolved]** We need to find the relationship between extruder movement to θ axis movement.
- **[Unsolved]** Does the PrusaXL hardware allow control of 5 steppers simultaneously?

## [Send Positional Data Via Serial](Theta-Control-Via-Serial)
### Description
Before each movement command involving the θ axis, send a packet of serial data to an external microcontroller which can drive the θ axis motor.

### Benefits
- Absolutely no interference with normal printer function when θ axis not in use.
- No need to worry about compatibility of θ axis and on-board Prusa motor drivers.
- No need for firmware modification. This both avoids a major software headache and means that in the future firmware can be readily updated without worry. 

### Drawbacks
- No inherent syncing of θ axis and XYZ axes during movement commands. The serial packet itself has to contain all the data necessary to create in sync motion.

### Challenges
- **[Unsolved]** Need to find way to sync θ axis and XYZ axes when providing only a position and speed command via serial.
- **[Unsolved]** Need to find a way to power ESP32 (USB-C on back of Prusa doesn't provide power)
