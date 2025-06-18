# Multiple bed types

You can enable it in printer settings.

Once enabled, you can select the bed type in the drop-down menu, corresponding bed temperature will be set automatically.
You can set the bed temperature for each bed type in the filament settings as demonstrated in the following image.

![bed-types](https://github.com/SoftFever/OrcaSlicer/blob/main/doc/images/bed-types.gif?raw=true)

Orca also supported a `curr_bed_type` variable in custom G-code.  With the new
build plate manager the preferred approach is to use `plate_z_offset` which
directly exposes the z offset configured for a plate.  The following sample
G-code automatically applies the configured offset if it is not zero:

```c++
{if plate_z_offset != 0}
  SET_GCODE_OFFSET Z={plate_z_offset}
{endif}
```

available bed types are:

```c++
"Cool Plate"
"Engineering Plate"
"High Temp Plate"
"Textured PEI Plate"
```
