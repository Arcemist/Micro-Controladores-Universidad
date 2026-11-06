struct Motor {
  const byte T_min;
  const byte T_max;
  bool Encendido;
};

struct Motor D1 = {
  .T_min = 30,
  .T_max = 60,
  .Encendido = false
};

struct Motor D2 = {
  .T_min = 40,
  .T_max = 60,
  .Encendido = false
};

unsigned int Reduccion;
volatile unsigned int Reductor = 1;

void setup() {
  Serial.begin(115200); // la maxima estable

  Reduccion = Segundos_a_Pulsos(10.0);

  DDRD = (1<<DDD5) | (1<<DDD6);

  Inicializar_ADC();

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



  /* Configuracion del Timer 1
  * Esta configurado a una frecuencia de 5kHz y en
  * modo PWM phase correct con top en ICR
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
  OCR1A = 1200;
  OCR1B = 100;

  // Ligar los pines 9 y 10 a A y B
  DDRB |= (1 << PB1) | (1 << PB2);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
}

char Comando;
void loop() {

  // Falta la parte de recibir comando por serial
  if (Serial.available() > 0) {
    String lectura_serial = Serial.readStringUntil('\n');
    
    float argumento = lectura_serial.substring(1).toFloat();

    Comando = lectura_serial[0];

    switch (Comando) {
      case 'T':
        Reduccion = Segundos_a_Pulsos(argumento);
      break;
    };
  };
}

ISR(TIMER1_COMPA_vect) {
  Reductor += 1;


  if (Reductor >= Reduccion) {
    Reductor = 0;

    int Lectura = Leer_Temperatura();


    Serial.print("La temperatura actual es: ");
    Serial.println(Lectura);

    Serial.print("El Led Verde esta: ");
    if (Led_Verde() == HIGH) {
      Serial.println("Encendido");
    } else {
      Serial.println("Apagado");
    };

    Serial.print("El Led Rojo esta: ");
    if (Led_Rojo() == HIGH) {
      Serial.println("Encendido");
    } else {
      Serial.println("Apagado");
    };

    int periodo = floor( Reduccion / 10000.0 );
    Serial.print("El intervalo de medida actual es: ");
    Serial.print(periodo);
    Serial.println("s");

    Serial.print("El tiempo encendido del Led Rojo es: ");
    Serial.print(periodo / 4.0);
    Serial.println("s");
  };
}