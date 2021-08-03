# Duckveloper 🐤

Say goodbye to the low-tech days of rubber duck debugging! **Duckveloper 🐤** is the coding partner you never knew you needed.

Simply press a button and the innovative, high-tech duck will generously provide you with words of "encouragement" to help you figure out what's really wrong with your code.

Arduino project with an MP3 shield, speaker, and rubber duck. Inspired by the strategy of [rubber duck debugging](https://en.wikipedia.org/wiki/Rubber_duck_debugging).

## Components

This project uses the [Arduino VS1053 Library](https://github.com/mpflaga/Arduino_Library-vs1053_for_SdFat) to decode MP3 audio and interface with the MP3 shield.

Serial | Name | Quantity | Description
--- | --- | --- | ---
| | Rubber Duck | As many as you need to be happy in life
DEV-11021 | [Arduino Uno - R3](https://www.sparkfun.com/search/results?term=arduino+uno) | 1
DEV-12660 | [SparkFun MP3 Player Shield](https://www.sparkfun.com/products/12660) | 1
PRT-11417 | [Arduino Stackable Header Kit - R3](https://www.sparkfun.com/products/11417) | 1 | For the MP3 Shield
COM-09151 | [Speaker (0.5W)](https://www.sparkfun.com/products/9151) | 1
PRT-10512 | Optional: [9V Battery Holder](https://www.sparkfun.com/products/10512) | 1

Other common generic components you should have:
* 1 button/switch (e.g. [Momentary Pushbutton Switch](https://www.sparkfun.com/products/9190))
* 1 LED (e.g. [Super Bright LED](https://www.sparkfun.com/products/11118))
* 1 breadboard
* Wires

## Setup

### Hardware Configuration

Solder the stackable headers onto the MP3 Player shield.

Mount the MP3 Player shield onto the Arduino Uno.


### Software Configuration

[Download the Arduino IDE](https://www.arduino.cc/en/software).

[Install the VS1053 library in the Arduino IDE](https://mpflaga.github.io/Arduino_Library-vs1053_for_SdFat/).

Format the SD card to FAT16 or FAT32.

Note: The audio files must be named as `track001.mp3`, `track002.mp3`, etc.




https://learn.sparkfun.com/tutorials/mp3-player-shield-hookup-guide-v15/all
