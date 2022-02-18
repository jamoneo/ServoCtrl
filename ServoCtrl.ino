/* Ejemplo de uso de ServCtl
 *  By José Angel Moneo (2016)
 *  Permite usar un servo como si fuera un motor stepper.
 *  De esta forma se peodrá realizar movimientos a velocidad controlada
 *  Velocidad maxima depende del servo y su alimentación
 */


#define PCA9685  //opción con driver 16 canales PWM
#define WITH_TIMER  //Opción para que el control se haga por timer

#ifdef WITH_TIMER
  #include <TimerOne.h>
#endif  
#ifdef  PCA9685
    #include "ServoCtrlDriver.h"
    PWMServoDriver   PWM_Driver;  //Driver usado para servo, Pueden ser distintos por motor
    ServoCtrlDriver Motor[2];       //Para caso de usar 2 servos 

#else
   #include "ServoCtrl.h"
    ServoCtrl Motor[2];       //Para caso de usar 2 servos 

 #endif   

 


// Control de posición y movimientos
void Rfsh()
{
  Motor[0].Update();
 Motor[1].Update(); 
}



void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN,false);
#ifdef  PCA9685
   //configura los dirvers y los asigna al control indicando el pin. Si es el mismo driver solo hace falta una inicialización de este
   PWM_Driver.Init(0x40);   //inicializa la tarjeta en la direcci?n 40h
   PWM_Driver.begin();   //inicializamos la tarjeta de control PWM de la cabeza
   PWM_Driver.setPWMFreq(46);  //  Asignamos la frecuencia al PWM  seg?n lo calculado para los servos
   Motor[0].Init(&PWM_Driver,1); //Inicializamos y asigamos las salidas en el PCA9685 a los servos
   Motor[1].Init(&PWM_Driver,2); //Inicializamos y asigamos las salidas en el PCA9685 a los servos
 #else
  // Configura el control de los controles incicando el pin de servo
   Motor[0].Init(9); //Inicializamos y asigamos las salidas en el PCA9685 a los servos
   Motor[1].Init(10); //Inicializamos y asigamos las salidas en el PCA9685 a los servos
#endif  


#ifdef WITH_TIMER
//Si se realiza con timer, el tiempo de LOOP no influirá
//en la respuesta del servo
 Timer1.initialize(5000);  //Resolucion servo 5ms
  Timer1.attachInterrupt(Rfsh); // blinkLED to run every 0.15 seconds
#endif
  //LLeva ahome el driver
Motor[0].GoHome();
Motor[0].Set_Vel(10); //Asigna 10g/s de velocidad
Motor[0].MoveTo(30); //Mueve el servo a la posición 30 a la velocidad seleccionada

}




void loop() {
//Enciende el led cuando queden 15 grados para llegar
if(Motor[0].Rest(15)) digitalWrite(LED_BUILTIN,true);

//Parpadea cuando llega a su destino
if(Motor[0].IsStoped()) digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

//Sin timer
#ifndef WITH_TIMER
//la reacción depende del tiempo de ciclo del loop
  Rfsh();
#endif 
}
