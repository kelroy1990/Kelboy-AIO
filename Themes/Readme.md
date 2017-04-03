# Kelboy-AIO- Instalación tema

Su instalación es muy sencilla, solo debemos copiar esta carpeta a la dirección de la Raspberry Pi /etc/emulationstation/themes.
Igual que en el apartado 9 (Guía rápida) se va a necesitar la Raspberry Pi conectada a una wifi o un teclado a ella en la consola de comandos, si accedemos a ella mediante terminal  ssh como puty, o similares, escribiremos lo siguiente:
```
ssh pi@retropie.local
```
Contraseña por defecto: 'raspberry'.
Una vez dentro debemos escribir el siguiente comando, y ya tendremos nuestra carpeta copiada en el sitio correcto.
```
sudo cp -R /home/pi/Kelboy-AIO/Themes/ /etc/emulationstation/themes/                                     
```
Por último dentro del menu de retropie de nuestra Game Boy debemos pulsar estar y en el menu dirigirnos a UI-Settings. Una vez dentro de este menu debemos bajar abajo del todo hasta llegar a “Theme Set” y cambiar el tema pulsando la tecla derecha o izquierda hasta encontrar gbz35.

De forma que al volver al menú principal ya tendremos el tema en cuestión donde podremos ver los textos correctamente.
