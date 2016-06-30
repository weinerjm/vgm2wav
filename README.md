# vgm2wav
## Description
vgm2wav is a simple command-line interface for essential funcitonality of blargg's [game-music-emu](https://bitbucket.org/mpyne/game-music-emu/wiki/Home) library. It builds off of the demo code that is included in game-music-emu.

vgm2wav is able to export video game music files (e.g., NES NSF or Super Nintendo SPC) to uncompressed WAV format. The output can be to either a file or a stream (`stdout`).

vgm2wav also can separate a track into individual voices.

## Purpose
I could not find a suitable simple command-line interface for exporting NSF and SPC files to WAV. Most of the programs out there are Windows-only or GUI programs that are currently under development. Importantly, vgm2wav can output data to `stdout` which enables the easy batch conversion of music files.

It also game me a chance to brush up on my C programming and learn a few things about music formats! 

vgm2wav is a part of a larger project that involves analysis of large numbers of songs for procedural music generation, among other things. (More to come on that front soon!)

## Requirements

1. [CMake](http://cmake.org/) version 2.6 or higher

2. game-music-emu 0.6.0 library (included here).

## Building
Clone the repository using
```
git clone ...
```
Then when you are in the vgm2wav directory, run
```
cmake .
```
and finally `make`.
`make clean` should allow you to start from scratch.

This build has only been tested on Mac OS X 10.11 with CMake 3.5.2 and gcc 5.3.0.

## Usage
vgm2wav supports several options.
### Basic Input/Output
vgm2wav supports specifying input and output files with the `-i` and `-o` options:
```shell
./vgm2wav -i my_file.spc -o out.wav
```
If no input is specified, it defaults to `test.wav`. If no output is specified, the default is `out.wav.` Specifying `-o -` outputs to `stdout`.

### Voices
To export all voices separately as files labeled `Voice#.wav`, you can run
```shell
./vgm2wav -i my_file.spc -v
```
To select a single voice and have it exported as `Voice#.wav`, use the `-s [number]` option, e.g.,
```shell
./vgm2wav -s 1
```
The voices are zero-indexed. Note that a single voice can be exported to `stdout`. 

### Track Length
By default, vgm2wav will try to output the full length of the track. If it is not able to find track length information (because of the format) then it defaults to 30 seconds. To override this and specify a length of the output, the `-t [seconds]` option can be used.

### Other
Verbose output is enabled with the `-b` option.

### Output
By default, the output is to 44.1 kHz RIFF Wave format. 

## License
This software is available under the Lesser GNU Public License.