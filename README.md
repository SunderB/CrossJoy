# "CrossJoy" - JoyCon Input Test
A proof-of-concept prototype of a cross-platform(?) C++ port of [BetterJoy](https://github.com/Davidobot/BetterJoy); to allow the JoyCon to be used with programs using the Cemuhook interface (e.g. Cemu, Dolphin).

* [Building](#building)
* [Running](#running)
* [Licence](#licence)
* [Attribution](#attribution)

## Building
```bash
cd crossjoy
# Create a build directory
mkdir build
cd build
# Build using CMake
cmake ..
cmake --build .
```
This has only been tested on Linux.


## Running
ℹ️ -  On Linux you may need to run the program as root (or change udev rules) to avoid "Permission Denied" errors.
```bash
# Run!
./joycon
```

## Licence
CrossJoy is distributed under the MIT Licence. Copyright (c) 2021 SunderB. All rights reserved.

### 3rd-party licences
* **[BetterJoy](https://github.com/Davidobot/BetterJoy)** - licensed under the MIT Licence. Copyright (c) 2018 David Khachaturov
#### Linked libraries
* **[hidapi](https://github.com/libusb/hidapi/)** - licensed under the [HIDAPI original licence](https://github.com/libusb/hidapi/blob/master/LICENSE-orig.txt). Copyright 2009, Alan Ott, Signal 11 Software. All Rights Reserved.

## Attribution
* [dekuNukem's Documentation on the Switch & JoyCons](https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/)
* [BetterJoy](https://github.com/Davidobot/BetterJoy)
* [hidapi](https://github.com/libusb/hidapi/)
