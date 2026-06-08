# AudioMultiChannel

Extend [Teensy Audio lib](https://github.com/PaulStoffregen/Audio) to enable multichannel recording on SD card.
This module address SD card over the SDIO protocol, which is faster than SPI.

> [!warning]
> For now, records are saved as raw files, but in the future I'm planning to support saving directly as WAV file.
> Support for other audio format won't be planned.

## How to use

Simply copy the AudioMultiChannel module (.cpp and .h files) into the same folder as your Arduino project folder.

## Example

> [!tip]
> Don't forget to copy AudioMultiChannel module files to the project folder.

- [MultipleSinewavesRecording](examples/MultipleSinewavesRecording/MultipleSinewavesRecording.ino): Generate 32 sine waves and record them in