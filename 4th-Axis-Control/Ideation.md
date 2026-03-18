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
  - "Challenges" refers to forseen cons of the presented solution that are potentially solvable. Challenges have the following markings:
    - [Solved] describes challenges which have been completely solved and no longer pose any issue.
    - [Mitigated] describes challenges for which an answer has been found that only partially solves the issues.
    - [In Progress] describes challenges that are not solved and are currently being worked on.
    - [Unsolved] describes challenges that haven't been solved and aren't yet being worked on.

## Switching Z and θ axes
### Description
The PrusaXL firmware/hardware can already handle controlling XYZ steppers simultaneously through the basic `G0/G1` movement commands. We can simply switch the power from being delivered to the Z axis motor to the θ axis motor and vise versa in order to control the θ axis.

### Benefits
- No need for firmware modification. This both avoids a major software headache and means that in the future firmware can be readily updated without worry.
- Potentially zero impact to standard printer functionality when the switching capability is not being used. So long as the switching circuit itself doesn't impact the Z motor dynamics, this solution should be 'invisible' to printer functionality when the switching isn't used.

### Drawbacks
- No simultaneous Z axis and θ axis control. Any wiping motion that would naturally occur in the θ and Z axes simultaneously is no longer possible.
- Both Z and θ axes become unpowered at some point during printing.

### Challenges
- [In Progress] We need to trigger switching between Z and θ axis control via G-code.
  - I searched for a while to try and find a G-code command that could be used to control the switching behvaior, but couldn't find a simple toggleable digital signal that could be easily hijacked for this purpose. 
  - I've landed at using the `M118` command to send a Serial signal out to an external microcontroller. The microcontroller will then itself control the switching behavior.
    - I verified that my laptop recieves the serial data from `M118` via the USB-C port on the XL Buddy board.  
    - I've yet to test that the ESP32 can itself recieve this serial data. I'm not sure that the Prusa can natively recognize the UART bridge that the ESP32 uses for USB communication.
- [In Progress] When the θ and Z axes are unpowered, they may accumulate positional error.
  - The Prusa definitely microsteps its motors, so when they lose power they'll at least *want* to fall to the closest full-step position. With at least one Z/θ switch per layer and potentially hundreds of layers per part, this error may accumulate over time.
  - Friction in the Z/θ axes may prevent this error, but adding extra friction for the sake of it seems like a bad idea
- [Solved] When the Z/θ axes motors are unpowered they may cause large negative voltage spikes via magnetic field collapse.
  - `M84 Z` and `M17` can be used to disable the Z axis motors and reenable all steppers respectively. This allows the Z axis motors to be unpowered when they're switched.
