# Kelboy-AIO

Programa que llevan los ATMEL de las placas.

En la carpeta bootloader-adafruit tenemos un bootloader custom, en concreto está en la carpeta CDC.

....A seguir completando.


El programa en sí es lento pues la utilizacion de la libería HID medienta la interfaz Joystick es muy lenta, solo se introducen los elementos uno a uno, cuando en realidad el reporte se manda entero. En la propria libería se va llamando al HID report cada vez que hay una actualización.
Sería mucho más optimo llamarla cada vez que tenemos un cambio o cuando tenemos todos los cambios ya actualizados, mediante un array. Para ello voy modificar la libería y reescribir un par de funciones para aprovechar la potencia de la misma.

Instalación del programa para el apagado "Suave", el objetivo es no estropear la imagen de la Pi.
-----------

Se va a necesitar la Raspberry Pi conectada a una wifi o un teclado a ella en la consola de comandos, si accedemos a ella mediante terminal ssh como puty escribiremos lo siguiente:

```
ssh pi@retropie.local
```

Contraseña por defecto: 'raspberry'. En la consola de comandos editar el archivo /boot/config.txt :

```
sudo nano /boot/config.txt
```

Añadiendo la siguiente línea:

```
dtoverlay=gpio-poweroff,gpiopin=22,active_low="y"
```

Presionar Ctrl+X para salir, pulsar Y para guardar y luego presionar "retorno", la tecla usada para borrar un caracter de texto cuando nos equivocamos.

Posteriormente debemos descargar la carpeta con el código, podemos introducirla de forma manual en la sd o descargar la carpeta de github entera del proyecto, que contiene ademas el programa en python.

```
cd ~;git clone https://github.com/kelroy1990/Kelboy-AIO.git
```

Ahora, para testearlo, debemos lanzarlo manualmente:
```
python ~/SoftPower/SoftOff.py
```

Una vez que estemos a gusto con el resultado debemos instalarlo para que quede como algo definitivo, para que surja efecto debesmo reiniciar la consola una vez terminado.

```
echo "@reboot /usr/bin/nice -n 19 /usr/bin/python ~/SoftPower/SoftOff.py" >> mycron; crontab mycron;rm mycron
```
