#ifndef _HERRAMIENTAS_H
#define _HERRAMIENTAS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Settings.h>



class Herramientas {
private:


   // If you have only one analog stick, set this to 1,
   // If you have 2, set this to 2,
   // If you do not have any at all, set this to 0.
   // In theory, 4 are possible, but this script is limited to 2.
   int AnalogSticks = 1;

   // Analog stick input pins, invert, and deadzone:
   int deadzone = 5; // Values under 5 will return 0

   // Variable declarations for analog sticks
   int x1Zero, y1Zero;
   int x1Value, y1Value;
   int x2Zero, y2Zero;
   int x2Value, y2Value;
   int x1Min, x1Max;
   int y1Min, y1Max;
   int x2Min, x2Max;
   int y2Min, y2Max;
   int x1F1, x1F2;
   int y1F1, y1F2;
   int x2F1, x2F2;
   int y2F1, y2F2;


// Variable declarations for analog sticks



   int _OffsetX     = 0;
   int _OffsetY     = 0;
   int _OffsetZHori = 0;
   int _OffsetZVer  = 0;
   int _MaxXY       = 0;
   int _MaxZHoriVer = 0;
   //tenemos un total de 32 botones posibles, nosotros vamos tener A,B,X,Y,Gatillo-L,Gatillo-R,Start,Select,
   int _ButtonState[32] =
   {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
   };

   void CalculateOffsets();
   void RangeValue();
   int ReadState(int pin);

public:

   Herramientas();
   void UpdateButtonState();
   void FastUpdateButtonState();
   void PrintValueXYZ();
   void StartPinOut();

   int GetOffSetX()
   {
      return(_OffsetX);
   };
   int GetOffSetY()
   {
      return(_OffsetY);
   };
};

#endif
