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
