# Kelboy-AIO

Programa que llevan los ATMEL de las placas.

En la carpeta bootloader-adafruit tenemos un bootloader custom, en concreto está en la carpeta CDC.

....A seguir completando.


El programa en sí es lento pues la utilizacion de la libería HID medienta la interfaz Joystick es muy lenta, solo se introducen los elementos uno a uno, cuando en realidad el reporte se manda entero. En la propria libería se va llamando al HID report cada vez que hay una actualización.
Sería mucho más optimo llamarla cada vez que tenemos un cambio o cuando tenemos todos los cambios ya actualizados, mediante un array. Para ello voy modificar la libería y reescribir un par de funciones para aprovechar la potencia de la misma.
