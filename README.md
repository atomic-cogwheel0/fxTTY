# fxTTY
Serial terminal for USB Power Graphics calculators

This program performs basic serial receive (newline, VERY BASIC ANSI codes) and send (with backspace handling) functions.
If you want to build it yourself, use the official CASIO fx-9860G SDK, otherwise the G1A is ready to run.

Communication is currently implemented via the Mini-USB port on the calculators.

I have only tested on my 9750-GIII, but as long the calculator has an USB port, it's likely going to run well.

`setup.sh` is for Linux systems, it adds an udev rule to setup the calculator as a serial terminal.

(I have no clue why this is useful)
