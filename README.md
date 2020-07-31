# F Prime: RC Car Retrofit

Author: M Starch

This project was to take an old RC car and turn it into a command/control rover
using the NASA F prime software. This rover will be able to control the motor
drive of the RC car, take pictures, and control the helm.

## Project Description

This project uses an Raspberry PI Zero to send the telemetry and events from the
down to the ground system running on a user's laptop. The same WiFi link is used
send commands from the laptop up to the RC car.

**Note:** transmission uses TCP packets, a configuration departing from F prime
because UDP drops detected in the packet transmission where making JPG photos
irrecoverable.

## Setup Instructions

The user must install F Prime, the F Prime GDS, and the Raspberry PI cross-
compile toolchain to `/opt/tools`. These instructions are available by
consulting the NASA F Prime installation instructions.

See: https://nasa.github.io/fprime

## Build Instructions

Change into the `Rover` directory and execute the following commands to generate
a build-cache, build and install the RPI software, and run Linux unit-tests.

```
fprime-util generate
fprime-util install
fprime-util check
```

Once all this passes, use `scp` to copy the binary located at `Rover/bin/*` onto
the RPI. The system should be ready to run at this time.

```
scp Rover/bin/arm-linux-gnueabihf/Rover pi@<address-of-pi>

## Execution Instructions

To run the software, first start the Ground Data System using the RC car
dictionaries to get the custom defined commands.

```
fprime-gds -d Rover/ -n
```

Then using an SSH session onto the PI, run the RPI software. This could be
started on-startup if desired. Although this is outside the scope of this
README.

```
ssh pi@<address-at-pi>
pi>nohup ./Rover -a <address-of-ground> -p 50000 &
```
