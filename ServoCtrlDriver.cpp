/* ========================================================================== */
/*                                                                            */
/*   ServpCtrlDriver.c                                                              */
/*   (c) 2016 Josee Angel Moneo                                          */
/*                                                                            */
/*   Clase control Motores Servo                                              */
/*   optimizado usar con pwm 16 canales PCA9685                                */
/*   Utiliza el driver Adafruit 16-channel PWM & Servo driver
/*   Controla un servo como un motor                                          */
/*   permitiendo el control de posicion instantaneo o con velocidad           */
/*   de movimiento programada.                                                */
/*   Permite a su vez, el control del moviemiento del servo por velocidad     */
/*   de manera que se mueva como un motor de continua controlado por tension  */
/*   Permite asignar los limittes del movimiento                              */
/* Todos los parámetros se dan en grados y grados/segundo                     */
/* ========================================================================== */
#include "ServoCtrlDriver.h"


void ServoCtrlDriver::Init(PWMServoDriver *hpwm,int pin)
{  _pwm=hpwm;
   _DS=pin;

  //  limites
  _LimMax=90;
  _LimMin=-90;
   GoHome();
}

bool ServoCtrlDriver::IsStoped() 
{  if(!_WithPos)  return (_Vel_Inst==0) ;   //Si está comandado por velocidad siempre esta en marcha
   return  (abs(_Posicion-_PosicionTo)==0); // si la diferencia entre la posicion deseada y la actual es menor o igual a 2º consideramos posicionado al servo

 }

//test de distancia al destino
bool ServoCtrlDriver::Rest(float dist)
{  return (abs(_Posicion-_PosicionTo)<=dist);
}


//devuelve true si está posicionado en 0
bool ServoCtrlDriver::AtHome()
{return(_Posicion==0) ;

 }
 
 
//Busqueda de home
void ServoCtrlDriver::GoHome()
{   Set_Vel(100);
      Set_Pos(0);
      MoveTo(0);
     _PosicionTo=_Posicion;
}

//posiciona el servo de forma instantanea en un angulo
void ServoCtrlDriver::Set_Pos(float ang)
{    if(ang>_LimMax||ang<_LimMin) return;
 // PwmANG=(Ang+90)*A+B

//Serial.print(_DS);
//   Serial.print("Vel ");
 //  Serial.println(_Vel);
  //Serial.print(" ang ");
 // Serial.println(ang);
  _pwm->setPWM(_DS, 0,(ang+90)*2+104); 
     _Posicion=ang;
}

//Lee la posicion actual devolviendola en las unidades indicadas Grados/s o Rev/s
float ServoCtrlDriver::Get_Pos()
{  return _Posicion;

}

//Asigna limite maximo del movimiento
void ServoCtrlDriver::SetLimMax(float v)
{_LimMax=v;
}

//Asigna limite minimo del movimiento
void ServoCtrlDriver::SetLimMin(float v)
{_LimMin=v;
}

//Asigna velocidad. 
// Dado uqe _Vel y _Vel_Inst son en realidad tiempo entre pulsos en ms
// y el parámetro de velocidad son grados por segundo
// hemos de hacer la conversión al guardar la velocidad y al devolverla
//como avanzamos de 0,5º T=1000/Vel/2  Vel=1000/2/T
void ServoCtrlDriver::Set_Vel(float Vel)
{  if (Vel>0)  _Vel_Inst=_Vel=int(500/Vel);   //Permitimos cambiar la velocidad instantanea, para poder retocar la velocidad una vez iniciado el movimiento
   else  _Vel_Inst=_Vel=0;
}




//Lee la velocidad de consigna, de olviendola en las unidades indicadas Grados/s
int ServoCtrlDriver::Get_Vel()
{   return 500/_Vel;  //devuelve la velocidad convirtiendo el tiempo entre pulsos a 
}




//Mueve el servo a una posición absoluta
void ServoCtrlDriver::MoveTo(float posicion)
{ _WithPos=true;    
  _Vel_Inst=_Vel;  //Asigna la velocidad del movimiento a la programada     
 if (_Vel_Inst>0) 
 {_TMov=millis()+_Vel_Inst;
 _PosicionTo=posicion;
  //Si pasamos el límite solo llegamos al límite
  if(_PosicionTo>_LimMax) _PosicionTo=_LimMax;
  if(_PosicionTo<_LimMin) _PosicionTo=_LimMin;
  //Actualizamos bandera de dirección demovimiento, por si se desea consultar
 if(posicion>0) _Dir=true;
 else _Dir=false;
 }

}

//Mueve el servo sin controlar posición. Solo con control de velocidad
void ServoCtrlDriver::MovSpeedCtrl(int vel)
{  
  _WithPos=false;
  if(vel>0) _Dir=true;
  else _Dir=false;
  if (vel!=0)  _Vel_Inst=abs(int(500/vel));  //Asigna la velocidad de movimiento a la indicada en ese instante
  else _Vel_Inst=0;
   _TMov=millis()+_Vel_Inst;
}

//Mueve el servo una posición relativa
void ServoCtrlDriver::MoveRel(float posicion)
{_WithPos=true;
  _Vel_Inst=_Vel;  //Asigna la velocidad del movimiento a la programada 
if (_Vel_Inst>0) 
 {  _TMov=millis()+_Vel_Inst;
   _PosicionTo=(posicion+_Posicion); 
     //Si pasamos el límite solo llegamos al límite
  if(_PosicionTo>_LimMax) _PosicionTo=_LimMax;
  if(_PosicionTo<_LimMin) _PosicionTo=_LimMin;
 //Actualizamos bandera de dirección demovimiento, por si se desea consultar
 if(posicion>_Posicion) _Dir=true;
 else _Dir=false;
 //Serial.print(_PosicionTo);
 }
}

//orden de parada para caso de movimiento sin posicionamiento
void ServoCtrlDriver::Stop()
{//para almacenar temporalemente la velocidad de referencia  

   _PosicionTo = _Posicion;     //orden deparar

}

//Actualización de posición
//Esta rutina debe de ser llamada periódicamente mientras el servo no esté
//posicionado
void ServoCtrlDriver::Update()
{ float nPos;
  if(_Vel_Inst>0)   //solo realiza movimiento si hay velocidad
  if (!IsStoped())   //Testeamos si está parado. Ha llegado a su posición o la Velocidad indicada es 0.
       if(_TMov<millis()){
          //Control de moviemiento según posición dada o dirección indicada. Según el tipo de control
           if (_WithPos)  // con control de posicion
                    {  if (_PosicionTo>_Posicion)   nPos=_Posicion+0.5;  //Avanza un grado
                       if (_PosicionTo<_Posicion)  nPos=_Posicion-0.5;  //retrocede medio grado
                    }
           else //Sin control de posicion 
                    {  if (_Dir)   nPos=_Posicion+0.5;  //Avanza un grado
                       if (!_Dir)  nPos=_Posicion-0.5;  //retrocede medio grado
                    }

                    
          // Serial.println(nPos*180/PI);
            Set_Pos(nPos);
          _TMov=millis()+_Vel_Inst;   //Tiempo para el sigueinte movimiento
          }
 }
