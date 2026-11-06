inline void Led_Verde(bool encendido) {
  if (encendido) {
    PORTD |= (1<<PD5);
  } else {
    PORTD &= ~(1 << PD5);
  }
}

inline void Led_Rojo(bool encendido) {
  if (encendido) {
    PORTD |= (1<<PD6);
  } else {
    PORTD &= ~(1 << PD6);
  }
}

inline bool Led_Verde() {
  return ((PIND & (1 << PIND5)) >> PIND5);
}

inline bool Led_Rojo() {
  return ((PIND & (1 << PIND6)) >> PIND6);
}


inline unsigned int Segundos_a_Pulsos(float segundos) {
  return floor( segundos * 10000.0 );
}


int Leer_Temperatura() {

  cli();

  ADCSRA |= (1<<ADSC); // Empezar la lectura

  while (ADCSRA & (1<<ADSC)) {}; // Esperar a que termine la lectura

  sei();

  return ADC; // Retornar el valor de 10 bits
}