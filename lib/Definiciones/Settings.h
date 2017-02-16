//Aqui vamos a definir los distintos defines del modelo robot.


//Documentar toda variable que queremos que no exista.
#define I2CDEV_SERIAL_DEBUG
#define JoystickON
#define JoyXY

///////////////////////////DEFINICIONES DE PINES ////////////////////

//Joystick//

//Joystick eje X/Y
#define VRx     A3
#define VRy     A2

#define VRx2    A5
#define VRy2    A4



//Defines botones
#define A         5
#define B         13
#define X         10
#define Y         9
#define Start     8
#define Select    A0 //Este es un analógico introducido como un digital por pullup
#define L         6
#define R         12
#define Z         A1//Este es analogico introducido como un digital por pullup
#define D0Rx      0
#define D1Tx      1

//Cruceta
#define UP        2 //i2c
#define DOWN      4
#define LEFT      3 //i2c
#define RIGHT     11
