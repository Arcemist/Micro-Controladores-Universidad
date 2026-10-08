enum estados {
  Frio,
  Normal,
  Tibio,
  Caliente
};
volatile enum estados Estado;

const byte umbral = 4;

const byte Tmax_frio = 20 - umbral / 2;
const byte Tmin_normal = 20 + umbral / 2;
const byte Tmax_normal = 40 - umbral / 2;
const byte Tmin_tibio = 40 + umbral / 2;
const byte Tmax_tibio = 60 - umbral / 2;
const byte Tmin_caliente = 60 + umbral / 2;

const byte Reduccion = 40;
volatile byte Reductor = 0;

bool led_timer;

void setup() {
  Serial.begin(9600);


  DDRD = (1<<DDD5) | (1<<DDD6);

  //PORTD |= (1<<PD5);
  //PORTD &= ~(1 << PD5);


  /* ADMUX:
  * REF1
  * REF0
  * ADLAR
  * (espaciador)
  * MUX3
  * MUX2
  * MUX1
  * MUX0
  * 
  * Los valores de REF determinan que referencia utilizara el ADC
  * En este caso se usa '01' para que use AVCC que es el voltaje de operacion
  *
  * El valor de ADLAR determina como se guarda la lectura en los registros ADCH y ADCL
  * en este caso se utiliza '0' porque permite usar 'ADC' para leer los 2 registros al
  * mismo tiempo
  *
  * Y los registros MUX deciden cual entrada ADC 0-5 se esta utilizando
  * en este caso '0011' para la entrada A3
  */
  ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (1<<MUX0);


  /* ADCSRA:
  * ADEN
  * ADCS
  * ADATE
  * ADIF
  * ADIE
  * ADPS2
  * ADPS1
  * ADPS0
  *
  * ADEN abilita el uso del ADC
  *
  * ADCS empieza una lectura cuando es puesto en 1 y se vuelve a 0 cuando termina
  *
  * ADATE, ADIF y ADIE son para abilitar funciones que no precisamos utilizar en esto
  *
  * ADPS es para elegir el factor de reduccion del reloj del microcontrolador al reloj del modulo ADC
  * numeros mas chicos permiten mediciones mas rapidas pero imprecisas, mientras que numeros mas grandes
  * permiten mediciones mas precisas pero mas lentas.
  * Aca se eligio 64 que corresponde a '110' porque es un valor mas o menos en el medio del rango.
  */
  ADCSRA = (1<<ADEN) | (0<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);


  /* Configuracion Timer 1
  * Se Cambian los directorios de configuracion (TCCR1 A y B) para elegir el
  * modo CTC y el preescaler de 256.
  *
  * Despues se asigna el valor por defecto al tope del contador A del Timer 1.
  * En este caso es un valor calculado para un periodo de 10s tomando en
  * cuenta el Preescaler de 256 y un Reductor de 40.
  *
  * Y por ultimo se habilitan las interrupciones por el canal A del timer 1
  */
  TCCR1A = 0;
  TCCR1B = (1<<WGM12) | (1<<CS12);
  OCR1A = 15625;
  TIMSK1 = (1<<OCIE1A);
}

void loop() {

  // Falta la parte de recibir comando por serial
  if (Serial.available() > 0) {
    int lectura_serial = Serial.read();

    switch (lectura_serial) {
      case '2':
        OCR1A = 3125;
      break;

      case '4':
        OCR1A = 3125 * 2;
      break;

      case '6':
        OCR1A = 3125 * 3;
      break;

      case '8':
        OCR1A = 3125 * 4;
      break;

      case '10':
        OCR1A = 3125 * 5;
      break;

      case '12':
        OCR1A = 3125 * 6;
      break;

      case '14':
        OCR1A = 3125 * 7;
      break;

      case '16':
        OCR1A = 3125 * 8;
      break;
    };
  };

  switch (Estado) {
    case Frio:
      PORTD &= ~(1 << PD5);
      PORTD &= ~(1 << PD6);
    break;

    case Normal:
      PORTD |= (1<<PD5);
      PORTD &= ~(1 << PD6);
    break;

    case Tibio:
      PORTD |= (1<<PD5);

      if (led_timer) {
        PORTD |= (1<<PD6);
      } else {
        PORTD &= ~(1 << PD6);
      };
    break;

    case Caliente:
      PORTD |= (1<<PD5);
      PORTD |= (1<<PD6);
    break;
  }
}

ISR(TIMER1_COMPA_vect) {
  Reductor += 1;

  if (Reductor < Reduccion / 4) {
    led_timer = true;
  } else {
    led_timer = false;
  };

  if (Reductor >= Reduccion) {
    Reductor = 0;

    int Lectura = Leer_Temperatura();
    if ((0 < Lectura) && (Tmax_frio > Lectura))                   { Estado = Frio; }
    else if ((Tmin_normal < Lectura) && (Tmax_normal > Lectura))  { Estado = Normal; }
    else if ((Tmin_tibio < Lectura) && (Tmax_tibio > Lectura))    { Estado = Tibio; }
    else if (Tmin_caliente < Lectura)                             { Estado = Caliente; }

    Serial.print("La temperatura actual es: ");
    Serial.println(Lectura);

    Serial.print("El Led Verde esta: ");
    if (((PIND & (1 << PIND5)) >> PIND5) == 1) {
      Serial.println("Encendido");
    } else {
      Serial.println("Apagado");
    };

    Serial.print("El Led Rojo esta: ");
    if (((PIND & (1 << PIND6)) >> PIND6) == 1) {
      Serial.println("Encendido");
    } else {
      Serial.println("Apagado");
    };

    Serial.println("");
  };
}

int Leer_Temperatura() {

  cli();

  ADCSRA |= (1<<ADSC); // Empezar la lectura

  while (ADCSRA & (1<<ADSC)) {}; // Esperar a que termine la lectura

  sei();

  return ADC; // Retornar el valor de 10 bits
}