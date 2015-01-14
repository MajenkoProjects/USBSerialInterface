USB Serial Interface
====================

A sketch for the Majenko Technologies UltraNano Development Board
-----------------------------------------------------------------

This sketch, and the associated wiring diagram, turns the UltraNano
into a USB Serial Interface similar to boards based on the FT232
USB serial chip.

It supports an open-drain pulse output on port open/close for direct
connection to another MCU board (such as an Arduino or chipKIT
development board) as a RESET signal.  No capacitor is required as
you would with an FT232 board, which makes for much simpler wiring.

There are also connections for a DTR signal, both in active HIGH and
active LOW varieties.

The on-board LED shows activity, both for TX and RX.

The UART interface's baud rate is automatically configured to match
the requested USB interface baud rate.

![Wiring Diagram](raw/master/Fritzing/USB Serial Interface.png)

