 #include <Arduino.h>
 #include <Joystick.h>
 #include <Settings.h>
 #include <Herramientas.h>
 #include <EEPROM.h>


// Create Joystick

Herramientas Interfaz;

int OffsetXY = 512;
int OffsetZ  = 512;


void setup()
{
   //calculo de offsets.

   //La lógica es inversa, es decir tenemos todo mediante PULLUP. Para poder hacer las rutas de calibración vamos a utlizar D0 y D1, los cuales son 20 y 21 respectivamente.


   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.begin(115200);
   //delay(4000);
   #endif
   #ifdef JoystickON
   Interfaz.StartPinOut();
   #endif

   //pinMode(A0, INPUT_PULLUP);
}

void loop()
{
   //Aquí llamamos a la interfaz para que actualize el estado de los botones cada vez que tenemos un loop en arduino.


   #ifdef NormalButtonState
   Interfaz.UpdateButtonState();
   #endif
   #ifdef FastButtonState
   Interfaz.FastUpdateButtonState();
   #endif

   #ifdef DelayCalibrate
   delay(TIME);
   #endif
}
