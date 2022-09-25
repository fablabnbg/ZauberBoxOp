# Assembly of the box


## Lasercut

* *Optional:* If you want to adapt the dimensions, open and modify the FeeCAD model
  ("LCInterlocking" workbench is needed, it can be installed from market place). When finished, export the pieces as flattened SVG.
* Use the SVG with Visicut to cut the pieces. The SVG contains information about the needed plywood thickness. You may wish to add some extra markers or holes on the plywood, e.g. for a power supply socket.
  Use inkscape or similar to add paths to the drawing.
* Set Visicut to use colors to decided if to cut (red), mark (green) or ignore (blue).


## Electronics

* Connect the LED matrix to the D1 Mini. Pin layout uses defaults, so connect:

  * MOSI to PIN_MOSI (D
  * MISO to PIN_MISO
  * CLK to PIN_SCK
  * CS to to PIN_SS
  * It is strongly recommended to glue the jumper wires (e.g. with hot glue) or solder them. Otherwise loose connections **will** occur with strange effects on the LED matrix.

!! to be continued !!
