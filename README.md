# pw-lcd

Emulate the Pokewalker LCD, based on the SSD1858 controller.
Can either display Pokewalker-encoded image as ASCII-output in the terminal,
or it can convert an 8-bit greyscale bitmap to Pokewalker format.

LCD is 96x64 pixels, terminal is 196x64 characters.

[Datasheet](https://cdn.datasheetspdf.com/pdf-down/S/S/D/SSD1854_ETC.pdf)

## Compilation

Compile with `make`.

```
$ make
```

## Usage

```
Usage: pw-lcd -i <input> [-o <output] [-v]\n\
    -i <input>  Input file.\n\
    -o <output> Output file\n\
    -v          View image\n\
    -w          Image width\n\
    -h          Image height\n\
```

Examples:

To display a pokewalker-encoded imageinside `test.bin` at default 96x64 pixels.
```
$ ./pw-lcd -i test.bin -v
```

To display a pokewalker-encoded imageinside `test.bin` at specified resolution 8x16 pixels.
```
$ ./pw-lcd -i test.bin -v -w 8 -h 16
```

To convert an 8-bit greyscale bitmap image in `myimage.bmp` to Pokewalker image in `myimage.bin`.
```
$ ./pw-lcd -i myimage.bmp -o myimage.bin
```

To do the above, but also view the resulting conversion:
```
$ ./pw-lcd -i myimage.bmp -o myimage.bin -v
```

### Note when converting images

Make sure that the compression flag is `0` and the bit depth is `8`.
Otherwise, the program will produce inaccurate results or break.

It should work coming straight out of _GIMP_ with it set to greyscale and saved as a `.bmp` file.

Your image will be inverted on the actual Pokewalker screen (ie white in _GIMP_ corresponds to black on
the LCD).

Colour mapping:

- 192-255: white
- 128-191: light grey
- 64-127: dark grey
- 0-63: black



Expected output:
```
$ ./pw-lcd -i test.bin -v -w 48 -h 48
 0: ..................................##########################....................................
 1: ............................######oooooooooooooooooooooooooo######..............................
 2: ........................####oooooooooooooooooooooooooooooooooooooo####..........................
 3: ....................####oooooooooooooooooooooooooooooooooooooooooooooo####......................
 4: ..................####oooooooooooooooooooooooooooooooooooooooooooooooooo####....................
 5: ................##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##..................
 6: ..............##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##................
 7: ............##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##..............
 8: ..........##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##............
 9: ........##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##..........
10: ......####oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo####........
11: ......##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##........
12: ....##oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo##......
13: ....##oooooooooooooooo##################################################oooooooooooooooo##......
14: ..##oooooooooooooooo##..................................................##oooooooooooooooo##....
15: ..##oooooooooooooooo##..................................................##oooooooooooooooo##....
16: ..##oooooooooooooooo##..................................................##oooooooooooooooo##....
17: ##oooooooooooooooooo##..................................................##oooooooooooooooooo##..
18: ##oooooooooooooooooo##..............##....................##............##oooooooooooooooooo##..
19: ##oooooooooooooooooo##..............##....................##............##oooooooooooooooooo##..
20: ##oooooooooooooooooo##..............##....................##............##oooooooooooooooooo##..
21: ##oooooooooooooooooo##..................................................##oooooooooooooooooo##..
22: ######################......................########....................######################..
23: ######################..................####........####................######################..
24: ##..................##................##................##..............##..................##..
25: ##..................##..............##....................##............##..................##..
26: ##..................##..............##....................##............##..................##..
27: ##..................##..................................................##..................##..
28: ##..................##..................................................##..................##..
29: ##....................##################################################....................##..
30: ..##......................................................................................##....
31: ..##......................................................................................##....
32: ..##......................................................................................##....
33: ....##..................................................................................##......
34: ....##........................####............####............####......................##......
35: ......##....................########........########........########..................##........
36: ......####..................########......############......########................####........
37: ........##....................####........############........####..................##..........
38: ..........##................................########..............................##............
39: ............##................................####..............................##..............
40: ..............##..............................................................##................
41: ................##..........................................................##..................
42: ..................####..................................................####....................
43: ....................####..............................................####......................
44: ........................####......................................####..........................
45: ............................######..........................######..............................
46: ..................................##########################....................................
47: ................................................................................................
```
