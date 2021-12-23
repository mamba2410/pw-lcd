# pw-lcd

Emulate the Pokewalker LCD, based on the SSD1858 controller.
Outputs as ascii-rendered image.

LCD is 96x64 pixels, terminal is 196x64 characters.

[Datasheet](https://cdn.datasheetspdf.com/pdf-down/S/S/D/SSD1854_ETC.pdf)

## Usage
Give it a file (supplied `test.bin`) and it prints it to screen as an ascii-rendered image

Run it with 
```
$ make run
```


