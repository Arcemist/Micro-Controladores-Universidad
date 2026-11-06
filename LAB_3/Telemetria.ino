unsigned int Reduccion;
volatile unsigned int Reductor = 1;

inline void Inicializar_Telemetria() {
  Serial.begin(115200); // la maxima estable

  /* Configuracion Timer 2
  * Esta configurado de la misma manera que se configuro
  * para el proyecto integrador, es un timer de 0,1ms
  */
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS21);
  OCR2A = 199;
  TIMSK2 |= (1 << OCIE2A);

  Reduccion = Segundos_a_Pulsos(10.0);
}

ISR(TIMER2_COMPA_vect) {
  Reductor += 1;

  if (Reductor >= Reduccion) {
    Reductor = 0;

    int Lectura = Leer_Temperatura();

    // Texto a enviar
    
  };
}

inline void Ajustar_Reduccion(float segundos) {
  Reduccion = Segundos_a_Pulsos(segundos);
}