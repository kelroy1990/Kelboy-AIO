//Aqui vamos a definir los distintos defines del modelo robot.


//Documentar toda variable que queremos que no exista.
//#define I2CDEV_SERIAL_DEBUG
//#define I2CDEV_SERIAL_DEBUG_VAR  //Esta variable define la salida por pantalla de las variables x-y leidas del yoistick.
#define JoystickON
#define JoyXY

#define FastButtonState    //Si definimos esto estaremos usando la actualización rapida, NO TERMINADA HASTA QUE SE MODIFIQUE LA LIBRERIA JOYSTICK EN CONSECUENCIA.
//#define NormalButtonState    //Actualizacion de botones normal

#define DelayCalibrate         //Definiendo esto metemos un delay en cada loop, por si la tasa de actualización es demasiado rapida en el Arduino
#define TIME               15  //Tiempo en ms que se utiliza para retardar la funcion FastButtonState.
#define BlinkTime          500 //Tiempo de parpadeo del led de la rutina de calibrado
#define deadzone           5

///////////////////////////DEFINICIONES DE PINES ////////////////////



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
#define Z         A1 //Este es analogico introducido como un digital por pullup
#define D0Rx      0
#define D1Tx      1

//Cruceta
#define UP        2 //i2c0
#define DOWN      4
#define LEFT      3 //i2c
#define RIGHT     11
