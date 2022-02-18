/* ========================================================================== */
/*                                                                            */
/*   ServoCtrl.c                                                              */
/*   (c) 2016 Josï¿½e Angel Moneo                                             */
/*                                                                            */
/*   Clase control Motores Servo                                              */
/*   optimizado para microprocontroladores                                    */
/*   Controla un servo como un motor                                          */
/*   permitiendo el control de posicion instantaneo o con velocidad           */
/*   de movimiento programada.                                                */
/*   Permite a su vez, el control del moviemiento del servo por velocidad     */
/*   de manera que se mueva como un motor de continua controlado por tension  */
/*   Permite asignar los limittes del movimiento                              */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */


#ifndef _ServoCtrl_H
#define _ServoCtrl_H


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include <Servo.h>

    

#define AVANCE 1
#define RETROCESO 0



class ServoCtrl
{  

private:
  
     Servo _myservo;
 
     int _DS;
    int _Vel_Inst,_Vel; //Tiempos entre pasos control instantanea y programada
    float _PosicionTo,_Posicion,_LimMax,_LimMin;   // distancia a destino y posicon absoluta  actual en pasos
    unsigned long _TMov; //tiempo espera entre pasos
    bool _WithPos,_Dir;    //Contol de forma de moviemineto, Por posición y por velocidad
  //DesHabilita los limitadores de posicion//
public:
    //Constructor limpio, para usar en clases derivadas
    ServoCtrl(){};   //Constructor sin inicialización
   
 
    void Init(int pin);  //  si se inicializarona las variables  controlador y salida usada

    void Update();  //funciï¿½n para incluir en el timer
    void MoveTo(float posicion);   //activa un movimiento a una posicion absoluta
    void MoveRel(float posicion);  //activa un movimiento relativo a la posicion actual
    void MovSpeedCtrl(int vel);  //Control de movimiento por velocidad
    void Stop();    //detiene el motor en el momento actual, sin terminar el ultimo movimiento
    void Set_Vel(float Vel);    // asigna Velocidad
    void Set_Pos(float Pos); // posiciona el servo de forma instantanea en un angulo

    void GoHome();  // busca el home si se defindo un pin para el sensor o ponela posición acutal como home si no se definió, dir dirección de busqueda home
    int Get_Vel();   // Toma Velocidad almacenada 
    float Get_Pos();   // Toma la posicion actual
    bool AtHome(); //indica si está en home   
    void SetLimMax(float v);
    void SetLimMin(float v);

    bool IsStoped();
    bool Rest(float dist); //motor a una distancia del destion
   
      
};
#endif
