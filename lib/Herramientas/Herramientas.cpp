#include <Herramientas.h>
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   12, 0,                   // Button Count, Hat Switch Count
                   true, true, false,       // X and Y, but no Z Axis
                   true, true, false,       // No Rx, Ry, or Rz
                   false, false,            // No rudder or throttle
                   false, false, false);    // No accelerator, brake, or steering

Herramientas::Herramientas()
{
  #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("CONSTRUCTOR");
   #endif
}

void Herramientas::StartPinOut()
{
   //Aqui vamos a definir los pines
   //Seteamos los pines.

   //luego tengo rotacion X/Y en el Vrx y Vry

   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("############## INICIALIZACION PINES #################");
   #endif

   //pinMode(7, OUTPUT); //LED


   pinMode(UP, INPUT_PULLUP);
   pinMode(DOWN, INPUT_PULLUP);
   pinMode(LEFT, INPUT_PULLUP);
   pinMode(RIGHT, INPUT_PULLUP);

   pinMode(A, INPUT_PULLUP);
   pinMode(B, INPUT_PULLUP);
   pinMode(X, INPUT_PULLUP);
   pinMode(Y, INPUT_PULLUP);


   pinMode(L, INPUT_PULLUP);
   pinMode(R, INPUT_PULLUP);

   pinMode(Start, INPUT_PULLUP);
   digitalWrite(Select, HIGH);  // set pullup on analog pin 0

   // Turn indicator light on.
   // digitalWrite(7, HIGH);

   //delay(100);
   // digitalWrite(7, LOW);


//Calcula el rango del joy
   RangeValue();
   //CalculateOffsets();

   #ifdef JoystickON
   Joystick.setXAxisRange(-_MaxXY, _MaxXY);
   Joystick.setYAxisRange(-_MaxXY, _MaxXY);

        #ifdef JoyXY
   //Definimos la rotacion x/y
   Joystick.setRxAxisRange(-_MaxZHoriVer, _MaxZHoriVer);
   Joystick.setRyAxisRange(-_MaxZHoriVer, _MaxZHoriVer);
        #endif

   x1Zero = analogRead(VRx);
   y1Zero = analogRead(VRy);
   x2Zero = analogRead(VRx2);
   y2Zero = analogRead(VRy2);



   Joystick.begin();
   #endif
}

void Herramientas::UpdateButtonState()
{
//Lo que tenemos que hacer es ir leyendo cada uno de los pines.
   int _CurrentState = 0;

   if(_ButtonState[UP] != (_CurrentState = !digitalRead(UP))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[UP] = _CurrentState;
      Joystick.setButton(0, _CurrentState);

      #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("UP");
    #endif
      }
   if(_ButtonState[DOWN] != (_CurrentState = !digitalRead(DOWN))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[DOWN] = _CurrentState;
      Joystick.setButton(1, _CurrentState);

            #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("DOWN");
          #endif
      }
   if(_ButtonState[LEFT] != (_CurrentState = !digitalRead(LEFT))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[LEFT] = _CurrentState;
      Joystick.setButton(2, _CurrentState);

                    #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("LEFT");
                  #endif
      }
   if(_ButtonState[RIGHT] != (_CurrentState = !digitalRead(RIGHT))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[RIGHT] = _CurrentState;
      Joystick.setButton(3, _CurrentState);

                               #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("RIGHT");
                             #endif
      }


/////BOTONES /////
   if(_ButtonState[A] != (_CurrentState = !digitalRead(A))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[A] = _CurrentState;
      Joystick.setButton(4, _CurrentState);

        #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("A");
        #endif
      }

   if(_ButtonState[B] != (_CurrentState = !digitalRead(B))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[B] = _CurrentState;
      Joystick.setButton(5, _CurrentState);

         #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("B");
         #endif
      }

   if(_ButtonState[X] != (_CurrentState = !digitalRead(X))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[X] = _CurrentState;
      Joystick.setButton(6, _CurrentState);

          #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("X");
          #endif
      }

   if(_ButtonState[Y] != (_CurrentState = !digitalRead(Y))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[Y] = _CurrentState;
      Joystick.setButton(7, _CurrentState);

          #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("Y");
         #endif
      }

   //START Select
   if(_ButtonState[Start] != (_CurrentState = !digitalRead(Start))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[Start] = _CurrentState;
      Joystick.setButton(8, _CurrentState);

             #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("Start");
            #endif
      }
   if(_ButtonState[0] != (_CurrentState = !digitalRead(Select))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[0] = _CurrentState;
      Joystick.setButton(9, _CurrentState);


        #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("Select");
        #endif
      }

   //Gatillo-l Gatillo-R
   if(_ButtonState[L] != (_CurrentState = !digitalRead(L))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[L] = _CurrentState;
      Joystick.setButton(10, _CurrentState);

                #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("Gatillo-L");
               #endif
      }
   if(_ButtonState[R] != (_CurrentState = !digitalRead(R))){
      //Si tenemos que el botón es distinto, entonces ejecutamos.
      //Eejemplo, tenemos de inicio un cero en la posicion up del array, y pulsamos el botón que sería ahora uno, por lo tanto tenemos un cambio
      _ButtonState[R] = _CurrentState;
      Joystick.setButton(11, _CurrentState);


           #ifdef I2CDEV_SERIAL_DEBUG
      Serial.println("Select");
           #endif
      }


////////////////////////////////JOYISTICK////////////////////////


   // Read analog sticks and respect the deadzone value
   int x1Final = analogRead(VRx) - x1Zero;
   int y1Final = analogRead(VRy) - y1Zero;
   int x2Final = analogRead(VRx2) - x2Zero;
   int y2Final = analogRead(VRy2) - y2Zero;
   if(abs(x1Final) < deadzone){
      x1Final = 0;
      }
   if(abs(y1Final) < deadzone){
      y1Final = 0;
      }
   if(abs(x2Final) < deadzone){
      x2Final = 0;
      }
   if(abs(y2Final) < deadzone){
      y2Final = 0;
      }



// Check how many analog sticks have been specified (0, 1, 2)
// and disable (center) any that aren't used.
   if(AnalogSticks != 1 && AnalogSticks != 2){
      x1Final = 0;
      y1Final = 0;
      x2Final = 0;
      y2Final = 0;
      }
   if(AnalogSticks == 1){
      x2Final = 0;
      y2Final = 0;
      }
      #ifdef I2CDEV_SERIAL_DEBUG
 Serial.print("X1Final "); Serial.println(x1Final);
 Serial.print("Y1Final ");Serial.println(y1Final);
 Serial.print("X2Final "); Serial.println(x2Final);
 Serial.print("Y2Final ");Serial.println(y2Final);
      #endif
// Parse autocalibrated analog stick values
//aquí creo que debería hacer un mapeo.
   Joystick.setXAxis(x1Final);
   Joystick.setYAxis(y1Final);
   Joystick.setRxAxis(x2Final);
   Joystick.setRyAxis(y2Final);


   //Joystick.setXAxis(analogRead(VRx) - _MaxXY);
   // Joystick.setYAxis(analogRead(VRy) - _MaxXY);
   //Joystick.setRxAxis(analogRead(VRx));
   //Joystick.setRyAxis(analogRead(VRy));
//////FIN CRUCETA///////


   // Joystick.setButton(0, !digitalRead(A));
}

void Herramientas::CalculateOffsets()
{
   //Aqui vamos a recoger los offset, como ya sabemos que valor tenemos asignado como maximo y como minimo entonces solo tenemos que recoger los offset.
   //la idea es saber el valor máximo y a partir de ahí calcular el offset para luego sumarlo/restarlo en el void loop cada vez que se actualiza el valor.
   //Este offset va a ser una media, asi que debemos almacenar X valores, yo creo que 100 son suficientes.

   double _OffX = analogRead(VRx);
   double _OffY = analogRead(VRy);

   //Calculo de offset XY
    #ifdef JoyXY
   for(int h = 0; h < 100; h++){
       _OffX += analogRead(VRx);
       _OffX  = _OffX / 2;

       _OffY += analogRead(VRy);
       _OffY  = _OffY / 2;
       }

   //Ahora que tenemos el valor medio de 100 medidas lo que hacemos es restarlo al maximo y redondearlo.
   _OffsetX = round(_MaxXY - _OffX);
   _OffsetY = round(_MaxXY - _OffY);

      #ifdef I2CDEV_SERIAL_DEBUG
   Serial.print("OFFset X-> ");
   Serial.println(_OffsetX);
   Serial.print("OFFset Y-> ");
   Serial.println(_OffsetY);
      #endif
    #endif
}

void Herramientas::PrintValueXYZ()
{
       #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("################### Valores ####################");
      #ifdef JoyXY
   Serial.print("Valor VRx ");
   Serial.print(analogRead(VRx));
   Serial.print(" Valor VRy ");
   Serial.println(analogRead(VRy));
   #endif
   #ifdef JoyZ
   Serial.print("Valor Z1 ");
   Serial.print(analogRead(Z1));
   Serial.print(" Valor Z2 ");
   Serial.println(analogRead(Z2));
   #endif
     #endif
}

void Herramientas::RangeValue()
{
   //NO ESTOY MUY SEGURO DE ESTA FUNCION, HABRIA QUE COMPROBAR CON MAS POTENCIOMETROS, la teoría es que su punto 0,0 es en realidad el valor medio del potenciometro, por tanto Este
   //Sera su rango maximo.
   //Los valores que recoge el potenciometro de inicio son cerca de su valor límite pues lo que hay que hacer luego es sumar o restar el valor maximo
   //De esto se encarga la libreria JOY.

   //Rangos de colocación, si tenemos un potenciometro que tiene un valor superior a 450 le ponemos el valor 512,


   //Vamos coger como referencias 1024, 512, 256
    #ifdef JoyXY
   int _X       = analogRead(VRx);
   int _Y       = analogRead(VRy);
   int Ref1024X = abs(1024 - _X);
   int Ref1024Y = abs(1024 - _Y);
   int Ref512X  = abs(512 - _X);
   int Ref512Y  = abs(512 - _Y);
   int Ref256X  = abs(256 - _X);
   int Ref256Y  = abs(256 - _Y);

   //El numero más pequeño es el que gana.

   if(Ref256X < Ref512X){
      if(Ref256Y < Ref512Y){
          #ifdef I2CDEV_SERIAL_DEBUG
         Serial.println("X/Y Valor maximo 256");
          #endif
         _MaxXY = 256;
         }
      }
   else if(Ref512X < Ref1024X){
           if(Ref512Y < Ref1024Y){
          #ifdef I2CDEV_SERIAL_DEBUG
              Serial.println("X/Y Valor maximo 512");
          #endif
              _MaxXY = 512;
              }
           }
   else{
       //Si no se cumplen las anteriores es 1024
      #ifdef I2CDEV_SERIAL_DEBUG
       Serial.println("X/Y Valor maximo 1024");
      #endif
       _MaxXY = 1024;
       }
   #endif


   //para el Z

   if(AnalogSticks == 1){
      return;
      }
   int _ZHor = analogRead(VRx2);
   int _ZVer = analogRead(VRy2);
   Ref1024X = abs(1024 - _ZHor);
   Ref1024Y = abs(1024 - _ZVer);
   Ref512X  = abs(512 - _ZHor);
   Ref512Y  = abs(512 - _ZVer);
   Ref256X  = abs(256 - _ZHor);
   Ref256Y  = abs(256 - _ZVer);

   //El numero más pequeño es el que gana.

   if(Ref256X < Ref512X){
      if(Ref256Y < Ref512Y){
      #ifdef I2CDEV_SERIAL_DEBUG
         Serial.println("Z Valor maximo 256");
      #endif
         _MaxZHoriVer = 256;
         }
      }
   else if(Ref512X < Ref1024X){
           if(Ref512Y < Ref1024Y){
      #ifdef I2CDEV_SERIAL_DEBUG
              Serial.println("Z Valor maximo 512");
      #endif
              _MaxZHoriVer = 512;
              }
           }
   else{
       //Si no se cumplen las anteriores es 1024
  #ifdef I2CDEV_SERIAL_DEBUG
       Serial.println("Z Valor maximo 1024");
  #endif
       _MaxZHoriVer = 1024;
       }
}

int Herramientas::ReadState(int pin)
{
   return(digitalRead(pin));
}
