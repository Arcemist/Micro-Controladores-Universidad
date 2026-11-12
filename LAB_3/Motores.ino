struct Motor {
  const byte T_min;
  const byte T_max;
  const float Duty_min;
  const float Duty_max;
  volatile uint16_t *Duty_Cycle_Reg;
};

struct Motor D1 = {
  .T_min = 30,
  .T_max = 60,
  .Duty_min = 1.5,
  .Duty_max = 50,           // Teniamos miedo de romper el motorsito
  .Duty_Cycle_Reg = &OCR1A
};

struct Motor D2 = {
  .T_min = 40,
  .T_max = 60,
  .Duty_min = 59.5,
  .Duty_max = 100,
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

  // Ligar los pines 9 y 10 a A y B respectivamente
  DDRB |= (1 << PB1) | (1 << PB2);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
}

void Configurar_Duty_Cycle_Manual(float Porcentaje, byte n) {
  const Motor *Dn = Motores[n - 1];

  if (Porcentaje < Dn->Duty_min) { Porcentaje = 0; };
  if (Porcentaje > Dn->Duty_max) { Porcentaje = Dn->Duty_max; };

  *(Dn->Duty_Cycle_Reg) = floor(ICR1 * (Porcentaje / 100.0));
}

inline void Configurar_Duty_Cycle_Automatico(int Lectura) {

  for (int i = 0; i < sizeof(Motores); i++) {
    const Motor *Dn = Motores[i];

    if (Lectura < Dn->T_min) {

      *(Dn->Duty_Cycle_Reg) = 0;  // Desactivar completamente

    } else if (Lectura < Dn->T_max) {
      // En un trabajo real seguramente no se use una interpolacion linear
      // Pero para este sirve

      const float Porcentaje_Activacion = (Lectura - Dn->T_min) / (Dn->T_max - Dn->T_min);

      const float Duty_Cycle_Correspondiente = Dn->Duty_min + (Dn->Duty_max - Dn->Duty_min) * Porcentaje_Activacion;

      *(Dn->Duty_Cycle_Reg) = Floor(ICR1 * (Duty_Cycle_Correspondiente / 100));

    } else {

      *(Dn->Duty_Cycle_Reg) = floor(ICR1 * (Dn->Duty_max / 100.0));
    }
  };
}
