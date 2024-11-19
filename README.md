# fxTTY
Serial terminal for USB Power Graphics calculators

The program provides some basic serial receiving (newline, VERY BASIC ANSI codes) and sending (with backspace handling).
If you want to build it yourself, use the official CASIO fx-9860G SDK, otherwise the G1A is ready to run.

Communication is currently implemented via the Mini-USB port on the calculators.

I have only tested on my 9750-GIII, but I think as long as it has an USB port, it will run well.

The setup.sh is for Linux systems; it adds an udev rule to set the calculator as a serial terminal.


(I have no clue who this might prove useful for)
