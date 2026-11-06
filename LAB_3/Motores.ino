struct Motor {
  const byte T_min;
  const byte T_max;
  int *Duty_Cycle_Reg;
};

struct Motor D1 = {
  .T_min = 30,
  .T_max = 60,
  .Duty_Cycle_Reg = &OCR1A
};

struct Motor D2 = {
  .T_min = 40,
  .T_max = 60,
  .Duty_Cycle_Reg = &OCR1B
};

struct Motor *Motores[2] = { &D1, &D2 };

inline void Inicializar_Motores() {
  /* Configuracion del Timer 1
  * Esta configurado a una frecuencia de aprox 5kHz y en
  * modo PWM phase correct con TOP en ICR
  */
  TCCR1A = 0;
  TCCR1B = 0;

  // Elegir modo Phase correct con TOP en ICR
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM13);

  // Elegir la frecuencia
  TCCR1B |= (1 << CS10);
  ICR1 = 1600;

  // Inicializar Duty Cicles en 0
  OCR1A = 0;
  OCR1B = 0;

  // Ligar los pines 9 y 10 a A y B
  DDRB |= (1 << PB1) | (1 << PB2);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
}

inline void Configurar_Duty_Cycle_Manual(float Porcentaje, byte n) {
  if ( Porcentaje < 0.0 ) { Porcentaje = 0.0; }
  if ( Porcentaje > 100.0 ) { Porcentaje = 100.0; }

  *Motores[n + 1]->Duty_Cycle_Reg = floor( ICR1 * Porcentaje );
}

inline void Configurar_Duty_Cycle_Automatico(int Lectura, byte n) {
  const Motor *Dn = Motores[n + 1];

  if ( Lectura < Dn->T_min) {

    *Dn->Duty_Cycle_Reg = 0;

  } else if ( Lectura < Dn->T_max ) {

    // Modulado

  } else {

    *Dn->Duty_Cycle_Reg = ICR1;

  }
}
