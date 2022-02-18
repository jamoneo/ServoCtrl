# ServoCtrl
Libreria para controlar un servo como un Stepper. 
Permite realizar movimientos a velocidad controlada sobre un servo.
Podrá manera tanto servos conectados a placa principal, como servos conectados a PCA9685, pudiendo convinar las dos opciones o servos distribuidos entre diferentes PCA9685.
Podremos gestionar el refresco por poolling en el LOOP o por timer. Esto último asegurará movimientos más estables al no depender del ciclo del LOOP.

Funciones implementadas: 

    void Init(int pin);  //  si se inicializarona las variables  controlador y salida usada

    void Update();  //función de refresco para incluir en el timer o en el LOOP
    void MoveTo(float posicion);   //activa un movimiento a una posicion absoluta
    void MoveRel(float posicion);  //activa un movimiento relativo a la posicion actual
    void MovSpeedCtrl(int vel);  //Control de movimiento por velocidad. Para control con joysticks.
    void Stop();    //detiene el motor en el momento actual, sin terminar el ultimo movimiento
    void Set_Vel(float Vel);    // asigna Velocidad
    void Set_Pos(float Pos); // posiciona el servo de forma instantanea en un angulo

    void GoHome();  // busca el home si se defindo un pin para el sensor o pone la posición acutal como home si no se definió, dir dirección de busqueda home
    int Get_Vel();   // Toma Velocidad almacenada 
    float Get_Pos();   // Toma la posicion actual
    bool AtHome(); //indica si está en home   
    void SetLimMax(float v);
    void SetLimMin(float v);

    bool IsStoped(); //indica si acabó el movimiento
    bool Rest(float dist); //motor a una distancia del destino. Inica si el servo ya está a menos de la distancia solicitada de su destino programado en el movimiento.
