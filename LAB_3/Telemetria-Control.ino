unsigned int Reduccion;
volatile unsigned int Reductor = 0;

inline void Inicializar_Telemetria() {
  Serial.begin(115200); // la maxima estable

  /* Configuracion Timer 2
  * Esta configurado de la misma manera que se configuro
  * para el proyecto integrador, es un timer de 0,1ms
  */
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2A |= (1 << WGM21); // Elegir modo CTC
  TCCR2B |= (1 << CS21); // Elegir preescaler en 8
  OCR2A = 199;
  TIMSK2 |= (1 << OCIE2A);

  Reduccion = Segundos_a_Pulsos(10.0);
}

const int Escala = 10;
int Escalador = 1;

ISR(TIMER2_COMPA_vect) {

  Escalador += 1;
  if (Escalador == Escala) {
    Reductor += 1;
    Escalador = 0; 
  };

  if (Reductor >= Reduccion) {
    Reductor = 0;

    float Lectura = Leer_Temperatura();

    if (!Control_Manual_Activado) {
      Configurar_Duty_Cycle_Automatico(Lectura);
    };

    Serial.println("\n\n\n\n\n");

    Serial.print("La temperatura actual es: ");
    Serial.print(Lectura);
    Serial.println("C");

    Serial.print("El ciclo de trabajo del motor 1 es: ");
    Serial.print( ((float) OCR1B / ICR1) * 100 );
    Serial.println("%");

    Serial.print("El ciclo de trabajo del motor 2 es: ");
    Serial.print( ((float) OCR1A / ICR1) * 100 );
    Serial.println("%");

    Serial.println("La frecuencia del PWM es: 5kHz");

    Serial.print("El intervalo de adquisicion de muestras es: ");
    Serial.print(Pulsos_a_Segundos(Reduccion));
    Serial.println("s");
  };
}

inline void Ajustar_Reduccion(float segundos) {
  Reduccion = Segundos_a_Pulsos(segundos);
  Reductor = 0;
}