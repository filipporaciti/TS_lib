# TS_lib
This library can be used to connect Arduino (or other kind of microcontrollers) to [TunerStudio](https://www.tunerstudio.com/index.php/tuner-studio) software.

This library manage communication protocol ([MegaSquirt](https://www.msextra.com/doc/pdf/Megasquirt_Serial_Protocol-2014-10-28.pdf)). 
You have to define ECU definition firmware for TunerStudio ([doc](https://www.efianalytics.com/TunerStudio/docs/EFI%20Analytics%20ECU%20Definition%20files.pdf)).

## 🚀 Features 
You can:
- Set realtime data
- Set one or more pages (persistent data)
- Change firmware signature (code version)

## 📦 Requirements
Tested hardware:
| Hardware      | Works??? | Info          |
| ------------- |:--------:| ------------- |
| Arduino Nano  | ✅       | |
| Arduino Mega  | ✅       | |
| esp32         | ✅       | All pages must be 4 byte aligned. See examples |
| stm32         | |



## ✨ Examples
- [basic](https://github.com/filipporaciti/TS_lib/tree/develop/examples/basic)
- [more_pages](https://github.com/filipporaciti/TS_lib/tree/develop/examples/more_pages)
- [other_features](https://github.com/filipporaciti/TS_lib/tree/develop/examples/other_features)

## 📖 Wiki
Check out the project [wiki](https://github.com/filipporaciti/TS_lib/wiki) for documentation.
