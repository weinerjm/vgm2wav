# vgm2wav
vgm2wav is a simple command-line interface for essential functionality of blargg's [game-music-emu](https://bitbucket.org/mpyne/game-music-emu/wiki/Home) library. It builds off of the demo code that is included in game-music-emu.

vgm2wav converts video game music files (e.g., NES NSF or Super Nintendo SPC) to uncompressed WAV format. 
The conversion is achieved by running a sound emulator and converting the raw resulting stream to an audio file.
The output can be to either a file or a stream (`stdout`).

vgm2wav also can separate a track into individual voices.

## Example
Convert the packaged `test.nsf` to a 10-second long WAV file:
```
./vgm2wav -i test.nsf -o out.wav -t 10
```
Isolate the melody voice:
```
./vgm2wav -i test.nsf -s 0 -o melody.wav -t 10
```
Convert the melody to MIDI using [WaoN](http://waon.sourceforge.net/) or to MP3 using [LAME](http://lame.sourceforge.net/) without intermediate files:
```
./vgm2wav -i test.nsf -s 0 -t 10 -o - | waon -o melody.mid
./vgm2wav -i test.nsf -s 0 -t 10 -o - | lame --preset standard - melody.mp3
```

## Purpose
I could not find a suitable simple command-line interface for exporting NSF and SPC files to WAV. 
Most of the programs out there are Windows-only or GUI programs that are currently under development. 
Importantly, vgm2wav can output data to `stdout` which enables the easy batch conversion of music files.

It also game me a chance to brush up on my C programming and learn a few things about music formats! 

vgm2wav is a part of a larger project that involves analysis of large numbers of songs for procedural music generation, among other things. 
(More to come on that front soon!)

## Requirements

1. [CMake](http://cmake.org/) version 3.14 or higher

2. [game-music-emu](https://bitbucket.org/mpyne/game-music-emu/wiki/Home) 0.6.0 library (linked as a submodule).

## Build Instructions
Clone the repository using
```
git clone https://github.com/weinerjm/vgm2wav.git
cd vgm2wav
git submodule init
git submodule update
cmake -S . -B build
cmake --build build
```
You can copy the executable from `app/vgm2wav` to the location of your choosing (e.g., `/usr/local/bin`). `make install` doesn't work because I don't know how to use new cmake.

This build has only been tested on Mac OS X 10.14.6 with CMake 3.15.4 and clang 10.0.1. Also, only a few NSF and SPC files have been tested.

## Usage
vgm2wav supports several options for file input/output and selecting voices from the source sound file.
### Basic Input/Output
vgm2wav supports specifying input and output files with the `-i` and `-o` options:
```
./vgm2wav -i my_file.spc -o out.wav
```
If no input is specified, it defaults to `test.wav`. If no output is specified, the default is `out.wav.` Specifying `-o -` outputs to `stdout`.

### Voices
To export all voices separately as files labeled `Voice#.wav`, you can run
```
./vgm2wav -i my_file.spc -v
```
To select a single voice and have it exported as `Voice#.wav`, use the `-s [number]` option, e.g.,
```
./vgm2wav -s 1
```
The voice labels are zero-indexed. Note that a single voice can be exported to `stdout` in conjunction with the `-o -` option. 

### Tracks
The default track is track 0. For files (e.g., NES NSF) with multiple tracks, the `-r [track_num]` option will isolate a single track.

### Track Length
By default, vgm2wav will try to output the full length of the track. If it is not able to find track length information (because of the format) then it defaults to 30 seconds. To override this and specify a length of the output, the `-t [seconds]` option can be used.

### Other
Verbose output to stderr is enabled with the `-b` option.

### Output
The output is 44.1 kHz RIFF WAVE format. 

## License
This software is available under the Lesser GNU Public License v2.1.
