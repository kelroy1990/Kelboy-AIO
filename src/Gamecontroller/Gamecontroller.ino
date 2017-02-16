 #include <Arduino.h>
 #include <Joystick.h>
 #include <Settings.h>
 #include <Herramientas.h>



// Create Joystick

Herramientas Interfaz;

int OffsetXY = 512;
int OffsetZ  = 512;


void setup()
{
   //calculo de offsets.



   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.begin(115200);
   Interfaz.StartPinOut();
   Interfaz.PrintValueXYZ();
   #endif
   #ifdef JoystickON
   Interfaz.StartPinOut();
   #endif

   //pinMode(A0, INPUT_PULLUP);
}

void loop()
{
   Interfaz.UpdateButtonState();



}

