## pi-cluster-one

This was an early attempt at implementing a Raspberry Pi cluster
with eurocard backplane.

The design loosely mimics the [Meiko CS/2](https://github.com/garlick/meiko-cs2)
management network design, implementing a CAN bus network for

* Remote Power/Reset
* Remote Serial console
* Remote Power (watts) monitoring
* Remote Beacon LED

The prototype is a 3U eurocard with the Pi 3 on standoffs, and
some other hardware including a
[teensy3.2](https://www.pjrc.com/store/teensy32.html) as a management
controller.  The teensy is attached to the local (L-CAN) CAN bus via a
[MCP2551 CAN transceiver](http://www.microchip.com/wwwproducts/en/en010405).
The teensy can
* pull down the Pi reset line with one of its GPIO's
* [hard/soft power control](doc/power.md)
* flash a beacon LED to identify a board that needs service
* read/write the Pi console serial port and buffer 32K of history
* monitor Pi external current draw
* read L-CAN address from DIP switch
* read X-CAN address from DIN backplane.
* answer CAN queries

The Pi itself also gets a L-CAN interface through a
[MCP2515 SPI CAN controller](http://www.microchip.com/wwwproducts/en/en010406)
and reads the L-CAN and X-CAN addresses via GPIO.

The Eurocard plugs into a DIN backplane which supplies power and interconnects
L-CAN endpoints.
