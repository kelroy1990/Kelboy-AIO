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

   pinMode(7, OUTPUT); //LED


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


   WaitRoutineCalibration(); //Entramos en la rutina de, quieres calibrar el sistema?.
   // Turn indicator light on.
   // digitalWrite(7, HIGH);

   //delay(100);
   // digitalWrite(7, LOW);

   //CalibrateRoutine();
   //Vemos si tenemos la calibración del yoistick guardada
   if(EEPROM.read(12) == 0xFF){
      ReadEEPROMCalibration(); //Primero leelos los valores y actualizamos.
      CalculeRangeCalibrationON();
      }
   else{
       RangeValue();
       }

   #ifdef JoystickON
   Joystick.setXAxisRange(-_MaxXY, _MaxXY);
   Joystick.setYAxisRange(-_MaxXY, _MaxXY);

        #ifdef JoyXY
   //Definimos la rotacion x/y
   Joystick.setRxAxisRange(-_MaxZHoriVer, _MaxZHoriVer);
   Joystick.setRyAxisRange(-_MaxZHoriVer, _MaxZHoriVer);
        #endif



   //Aquí en el begin del yoistick, si lo dejamos tal cual por defecto está mandando todo el rato el estado cada vez que lo actualizamos, si no queremos
   //Que haga eso tenemos que declarlarlo como false. Una vez echo eso tenemos que llamar nosotros mismos al método sendstate.
   Joystick.begin(false);
   #endif
}

void Herramientas::CalculeRangeCalibrationON()
{
   int Range = (abs(x1Max - x1Zero));

   if(Range > 1024){
      _MaxXY = 1024;
      }
   if(Range > 512){
      _MaxXY = 512;
      }
   else{
       _MaxXY = 256;
       }

       #ifdef I2CDEV_SERIAL_DEBUG
   Serial.print("Rango final de los yoistick---- ");
   Serial.println(_MaxXY);
   #endif
}

void Herramientas::FastUpdateButtonState()
{
   //Misma funcion que UpdateButtonState pero con un algoritmo nuevo que pienso que es mucho más eficiente, utilizando una puerta OR.
   //La lógica sería tener una matriz de valores que se iran actualizando mediante un OR con el nuevo valor detectado. De esta forma si tenemos en un instante T un valor de 0
   //Y en un instante t+1 obtenemos un valor de 1, el valor final que tendrá el yoistick será de 0+1, osea 1. SI por el contrario en t tenemos un 1 y en t+1 un 1, obtendremos un valor final de
   // 1OR1 = 1 (al no ser una puerta XOR 1+1 da 1)

   //Tabla verdad-----OR
   //                  0 + 0   =   0
   //                  1 + 0   =   1 //Este es nuestro único problema, esto debe ser un CERO.
   //                  0 + 1   =   1
   //                  1 + 1   =   1

   //para ser la que deseamos tendría que tener una tabla de la verdad tal que...
   //                  0 + 0   =   0
   //                  1 + 0   =   0
   //                  0 + 1   =   1
   //                  1 + 1   =   1
   //Podemos sacar las funciones mediante Primera forma canónica o forma canónica disyuntiva (suma de productos o minterms). La cual nos dice que ((Negado(A)*B))+(A*B)=Resultado.
   //Si reducimos a la forma más simple nos da como resultado B, por lo tanto la salida solo depende del final. Vamos a probarlo.



   //Cruceta
   Joystick.setButton(0, !digitalRead(UP));
   Joystick.setButton(1, !digitalRead(DOWN));
   Joystick.setButton(2, !digitalRead(LEFT));
   Joystick.setButton(3, !digitalRead(RIGHT));

   //Botonera A,B,X,Y
   Joystick.setButton(4, !digitalRead(A));
   Joystick.setButton(5, !digitalRead(B));
   Joystick.setButton(6, !digitalRead(X));
   Joystick.setButton(7, !digitalRead(Y));

   //Botonera Start,Select
   Joystick.setButton(8, !digitalRead(Start));
   Joystick.setButton(9, !digitalRead(Select));

   //Gatillos L,R
   Joystick.setButton(10, !digitalRead(L));
   Joystick.setButton(11, !digitalRead(R));

   //Analogicos
   // Read analog sticks and respect the deadzone value
   int x1Final = map(analogRead(VRx), x1Min, x1Max, -_MaxXY, _MaxXY);
   int y1Final = map(analogRead(VRy), y1Min, y1Max, -_MaxXY, _MaxXY);
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

      #ifdef I2CDEV_SERIAL_DEBUG_VAR
   //Serial.print("Range value MaxXY-- ");
   //Serial.println(_MaxXY);
   //Serial.print("Range value MaxZHoriVer-- ");
   //Serial.println(_MaxZHoriVer);
   Serial.print("X1Final ");
   Serial.println(x1Final);
   Serial.print("Y1Final ");
   Serial.println(y1Final);
   //Serial.print("X2Final ");
   //Serial.println(x2Final);
   //Serial.print("Y2Final ");
   //Serial.println(y2Final);
      #endif
// Parse autocalibrated analog stick values
//aquí creo que debería hacer un mapeo.
//map(VALOR,ValorMas bajo posible, valor mas alto posible, valor mas bajo destino, valor mas alto destino);

   Joystick.setXAxis(x1Final);
   Joystick.setYAxis(y1Final);
   Joystick.setRxAxis(x2Final);
   Joystick.setRyAxis(y2Final);

   Joystick.sendState();
}

void Herramientas::WaitRoutineCalibration()
{
   double TimeStart = millis();

   while(millis() < TimeStart + 2000){
     #ifdef I2CDEV_SERIAL_DEBUG
         Serial.print("Estado boton A ");
         Serial.println(!digitalRead(A));
         Serial.print("Estado boton B ");
         Serial.println(!digitalRead(B));
         Serial.print("Estado boton X ");
         Serial.println(!digitalRead(X));
     #endif
         if(!digitalRead(A) == 1 && !digitalRead(B) == 1 && !digitalRead(X) == 1){
            CalibrateRoutine();
            }
         else{
             yield();
             }
         }
   return;
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
   int x1Final = map(analogRead(VRx), x1Min, x1Max, -_MaxXY, _MaxXY);
   int y1Final = map(analogRead(VRy), y1Min, y1Max, -_MaxXY, _MaxXY);
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

   //El punto medio debería ser 1024/2 =512. Detalle, siempre está desfasado, hay que ver este error y corregirlo.
   x1Final = map(analogRead(VRx), 150, (1024 - 150), -256, 256);
   y1Final = map(analogRead(VRy), 110, (1024 - 110), -256, 256);

   //x1Final = analogRead(VRx);
   //y1Final = analogRead(VRy);

      #ifdef I2CDEV_SERIAL_DEBUG_VAR
   //Serial.print("Range value MaxXY-- ");
   //Serial.println(_MaxXY);
   //Serial.print("Range value MaxZHoriVer-- ");
   //Serial.println(_MaxZHoriVer);
   //Serial.print("Range value XZero-- ");
   //Serial.println(x1Zero);
   //Serial.print("Range value Yzero-- ");
   //Serial.println(y1Zero);
   Serial.print("X1Final ");
   Serial.println(x1Final);
   Serial.print("Y1Final ");
   Serial.println(y1Final);
   //Serial.print("X2Final ");
   //Serial.println(x2Final);
   //Serial.print("Y2Final ");
   //Serial.println(y2Final);
      #endif
   // Parse autocalibrated analog stick values
   //aquí creo que debería hacer un mapeo.
   //Joystick.setXAxis(x1Final);
   // Joystick.setYAxis(y1Final);
   Joystick.setRxAxis(x2Final);
   Joystick.setRyAxis(y2Final);


   //Joystick.setXAxis(analogRead(VRx) - _MaxXY);
   // Joystick.setYAxis(analogRead(VRy) - _MaxXY);
   //Joystick.setRxAxis(analogRead(VRx));
   //Joystick.setRyAxis(analogRead(VRy));
   //////FIN CRUCETA///////


   // Joystick.setButton(0, !digitalRead(A));
   Joystick.sendState();
}

void Herramientas::CalibrateRoutine()
{
   //Ruta de calibracion del Joystick.
   //aquí estoy pensando como hacerla, lo más logico sería llamarla una vez empezamos con un estado 0, en donde durante unos segundos si tu pulsas A+B+Select accederás a esta rutina.

   //Se irán recogiendo los siguientes valores depurando con el led Rojo que erstá en la posicion de L.


   /*
    *                                        Y PRIMER valor
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    * CUARTO valor  -X-------------------------------------------X TERCER valor
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                        |
    *                                       -Y SEGUNDO valor
    *
    *
    */


   //Inicialmente se encenderá el led(PIN 7) parpadeando 5 veces, una vez se mantenga fijo tienes que mantener el joystick en la posicion del primer valor. Una vez terminado el led se apagará y se deberá soltar el yoistcik.
   //Momento en el cual el led volverá a parpadear 5 veces y se quedará fijo para el siguiente valor, que es -Y.


   int    AnalogNowY = 0;
   int    AnalogNowX = 0;
   int    valor      = 0;
   double TimeStart  = 0;

   for(int h = 0; h < 50; h++){
       AnalogNowY = analogRead(VRy);
       AnalogNowX = analogRead(VRx);
       }


   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("Ruta de calibración.....");
   Serial.print("AnalogNowX ");
   Serial.print(AnalogNowX);
   Serial.print(" AnalogNowY ");
   Serial.println(AnalogNowY);
   #endif


//Secuencia Espera EJE Y ---------------------------------------------------------

   ParpadeoLed(BlinkTime);

   while(analogRead(VRy) > (AnalogNowY - 100) && analogRead(VRy) < (AnalogNowY + 100)){
         yield();
         }

   digitalWrite(7, LOW);

   //Lectura de medida
   TimeStart = millis();
   valor     = 0;
   while(millis() < TimeStart + 1000){
         valor = analogRead(VRy);
         #ifdef I2CDEV_SERIAL_DEBUG
         Serial.print("Valor lectura..... ");
         Serial.println(valor);
         #endif
         //Mientras el momento actual sea menor al de inicio + 1 segundo vamos recogiendo variable.
         if(valor > AnalogNowY){ //Si estamos en el eje positivo añadimos este al positivo si lo supera.
            if(valor > y1Max){
               y1Max = valor;
               }
            }
         else if(valor < AnalogNowY){ //Si es menor al valor que tenemos estamos en el eje negativo.
                 if(valor < y1Min){ //Cogemos el mínimo, que como es superior al cero sería el maximo más cercano a cero.
                    y1Min = valor;
                    }
                 else if(y1Min == 0){
                         y1Min = valor;
                         }
                 }
         } //###fin while.

   ParpadeoLed(BlinkTime);


   while(analogRead(VRy) > (AnalogNowY - 100) && analogRead(VRy) < (AnalogNowY + 100)){
         yield();
         }
   digitalWrite(7, LOW);

   //Lectura de medida
   TimeStart = millis();
   valor     = 0;
   while(millis() < TimeStart + 1000){
         valor = analogRead(VRy);
         #ifdef I2CDEV_SERIAL_DEBUG
         Serial.print("Valor lectura..... ");
         Serial.println(valor);
         #endif
         //Mientras el momento actual sea menor al de inicio + 1 segundo vamos recogiendo variable.
         if(valor > AnalogNowY){       //Si estamos en el eje positivo añadimos este al positivo si lo supera.
            if(valor > y1Max){
               y1Max = valor;
               }
            }
         else if(valor < AnalogNowY){     //Si es menor al valor que tenemos estamos en el eje negativo.
                 if(valor < y1Min){       //Cogemos el mínimo, que como es superior al cero sería el maximo más cercano a cero.
                    y1Min = valor;
                    }
                 else if(y1Min == 0){
                         y1Min = valor;
                         }
                 }
         }       //###fin while.


   //Fin secuencia Y ----------------------------------------------------



   //Secuencia Espera EJE X ---------------------------------------------------------

   ParpadeoLed(BlinkTime);

   while(analogRead(VRx) > (AnalogNowX - 100) && analogRead(VRx) < (AnalogNowX + 100)){
         yield();
         }

   digitalWrite(7, LOW);

   //Lectura de medida
   TimeStart = millis();
   valor     = 0;
   while(millis() < TimeStart + 1000){
         valor = analogRead(VRx);
         #ifdef I2CDEV_SERIAL_DEBUG
         Serial.print("Valor lectura..... ");
         Serial.println(valor);
         #endif
         //Mientras el momento actual sea menor al de inicio + 1 segundo vamos recogiendo variable.
         if(valor > AnalogNowX){    //Si estamos en el eje positivo añadimos este al positivo si lo supera.
            if(valor > x1Max){
               x1Max = valor;
               }
            }
         else if(valor < AnalogNowX){  //Si es menor al valor que tenemos estamos en el eje negativo.
                 if(valor < x1Min){    //Cogemos el mínimo, que como es superior al cero sería el maximo más cercano a cero.
                    x1Min = valor;
                    }
                 else if(x1Min == 0){
                         x1Min = valor;
                         }
                 }
         }    //###fin while.


   ParpadeoLed(BlinkTime);


   while(analogRead(VRx) > (AnalogNowX - 100) && analogRead(VRx) < (AnalogNowX + 100)){
         yield();
         }

   digitalWrite(7, LOW);
   //Lectura de medida
   TimeStart = millis();
   valor     = 0;
   while(millis() < TimeStart + 1000){
         valor = analogRead(VRx);
         #ifdef I2CDEV_SERIAL_DEBUG
         Serial.print("Valor lectura..... ");
         Serial.println(valor);
         #endif
         //Mientras el momento actual sea menor al de inicio + 1 segundo vamos recogiendo variable.
         if(valor > AnalogNowX){          //Si estamos en el eje positivo añadimos este al positivo si lo supera.
            if(valor > x1Max){
               x1Max = valor;
               }
            }
         else if(valor < AnalogNowX){        //Si es menor al valor que tenemos estamos en el eje negativo.
                 if(valor < x1Min){          //Cogemos el mínimo, que como es superior al cero sería el maximo más cercano a cero.
                    x1Min = valor;
                    }
                 else if(x1Min == 0){
                         x1Min = valor;
                         }
                 }
         }          //###fin while.


   //Fin secuencia X ----------------------------------------------------

   ParpadeoLedOK(BlinkTime);


   //Detalle, si obtenemos un valor menor a AnalogNow entonces significa que estamos en el Minimo, si es mayor entonces estamos en el Maximo.

   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("");
   Serial.print("Punto medio en eje Y ");
   Serial.print(AnalogNowY);
   Serial.print(" -Punto medio en eje X ");
   Serial.println(AnalogNowX);
   Serial.print("Valor y1Max ");
   Serial.println(y1Max);
   Serial.print("Valor y1Min ");
   Serial.println(y1Min);
   Serial.print("Valor x1Max ");
   Serial.println(x1Max);
   Serial.print("Valor x1Min ");
   Serial.println(x1Min);
  #endif

   //Ahora debemos almacenar los valores en la EEPROM, pasando los valores medios.
   StoreEEPROMCalibration(AnalogNowX, AnalogNowY);
}

void Herramientas::StoreEEPROMCalibration(int MidPointX, int MidPointY)
{
   //La secuencia en esta función va a ser primero borrar todos los datos para posteriormente resubir la nueva calibración.
   //Para poder almacenar cada valor necesitamos un total 2 bytes por variable.
   //La construcción es como sigue.

   /* 0 byte superior MSB(Most significant Bit) |
    * 1 byte inferior LSB(Less significant Bit) | x1Max
    * 2 byte superior                           |
    * 3 byte inferior                           | x1Min
    * 4 byte superior                           |
    * 5 byte inferior                           | MidPointX
    * 6 byte superior                           |
    * 7 byte inferior                           | y1Max
    * 8 byte superior                           |
    * 9 byte inferior                           | y1Min
    * 10 byte superior                          |
    * 11 byte inferior                          | MidPointY
    * 12 byte de calibrado                      | 0xFF ==CalibradoTRUE, 0xA0 ==CalibradoFALSE
    */



   //Escribimos en la memoria según lo de arriba utilizando el desplazamiento de bits.

   //######################NOTA##########################
   //Utilizamos .uptade en vez de .write para no abusar el uso de la eeprom, pues tiene de vida util 10,000 ciclos, la diferencia frente al write es que comprueba el valor que tiene y si es el mismo no lo sobreescribe.

   EEPROM.update(0, highByte(x1Max));
   EEPROM.update(1, lowByte(x1Max));
   EEPROM.update(2, highByte(x1Min));
   EEPROM.update(3, lowByte(x1Min));
   EEPROM.update(4, highByte(MidPointX));
   EEPROM.update(5, lowByte(MidPointX));
   EEPROM.update(6, highByte(y1Max));
   EEPROM.update(7, lowByte(y1Max));
   EEPROM.update(8, highByte(y1Min));
   EEPROM.update(9, lowByte(y1Min));
   EEPROM.update(10, highByte(MidPointY));
   EEPROM.update(11, lowByte(MidPointY));
   EEPROM.update(12, 0xFF); //Ponemos un true en calibrado.
}

void Herramientas::ReadEEPROMCalibration()
{
   x1Max  = (unsigned int)((EEPROM.read(0) << 8) | EEPROM.read(1));
   x1Min  = (unsigned int)((EEPROM.read(2) << 8) | EEPROM.read(3));
   x1Zero = (unsigned int)((EEPROM.read(4) << 8) | EEPROM.read(5));
   y1Max  = (unsigned int)((EEPROM.read(6) << 8) | EEPROM.read(7));
   y1Min  = (unsigned int)((EEPROM.read(8) << 8) | EEPROM.read(9));
   y1Zero = (unsigned int)((EEPROM.read(10) << 8) | EEPROM.read(11));

   // x1Max = (unsigned int)((EEPROM.read(0) << 8) | (EEPROM.read(1)));
   #ifdef I2CDEV_SERIAL_DEBUG
   Serial.println("Valores obtenido de la memoria EEPROM: ");
   Serial.print("x1Maximo:");
   Serial.print(x1Max);
   Serial.print(" x1Minimo:");
   Serial.println(x1Min);
   Serial.print("y1Maximo:");
   Serial.print(y1Max);
   Serial.print(" y1Minimo:");
   Serial.println(y1Min);
   Serial.print("XZero:");
   Serial.print(x1Zero);
   Serial.print(" YZero:");
   Serial.println(y1Zero);
   #endif
}

void Herramientas::ParpadeoLed(double time)
{
   digitalWrite(7, HIGH);
   for(int h = 0; h < 4; h++){
       delay(time);
       digitalWrite(7, LOW);
       delay(time);
       digitalWrite(7, HIGH);
       }
}

void Herramientas::ParpadeoLedOK(double time)
{
   digitalWrite(7, HIGH);
   delay(time);
   digitalWrite(7, LOW);
}

void Herramientas::CalculateOffsets()
{
   //Aqui vamos a recoger los offset, como ya sabemos que valor tenemos asignado como maximo y como minimo entonces solo tenemos que recoger los offset.
   //la idea es saber el valor máximo y a partir de ahí calcular el offset para luego sumarlo/restarlo en el void loop cada vez que se actualiza el valor.
   //Este offset va a ser una media, asi que debemos almacenar X valores, yo creo que 100 son suficientes.


   //Funciona correctamente con el del PSP.

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

   //Si no tenemos nada calibrado cogemos valores por defecto, el cual va a ser algo bastante malo en su salida.

   x1Min = 150;
   y1Min = 110;
   x1Max = 1024 - 150;
   y1Max = 1024 - 110;
}

int Herramientas::ReadState(int pin)
{
   return(digitalRead(pin));
}
