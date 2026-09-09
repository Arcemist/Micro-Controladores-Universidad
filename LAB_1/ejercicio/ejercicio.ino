enum Modo {
  AUTOMATICO = 0,
  MANUAL = 1
};
enum Modo estado = AUTOMATICO;

volatile byte retardo_pulso = 0;
volatile bool encendido = false;

// ISR es la macro para crear interrupciones
// y INT0_vect es la señal de interrupcion creada en el Pin 2
ISR(INT0_vect) {
  if (retardo_pulso == 0) { // Esto sirve para asegurar pero conviene poner un capacitor igual
    retardo_pulso = 2000;
    encendido = !encendido;
  }; 
}

ISR(INT1_vect) {
  if (estado == AUTOMATICO) {
    estado = MANUAL;
  } else {
    estado = AUTOMATICO;
  };
}

void setup() {
  DDRD &= (0<<PD2) | (0<<PD3); 
  DDRD |= (1<<PD5);
  PORTD = (1<<PD2) | (1<<PD3) | (1<<PD5); // Pin 2 en PULLUP y Pin 5 en LOW

  cli(); // Desabilitar las interupciones globales

  /* EIMSK:
  * INT0
  * INT1
  *
  * INT0 habilita interrupciones provocadas por el Pin 2
  * INT1 habilita las interrupciones para el Pin 3
  */

  EIFR = (1 << INTF0) | (1 << INTF1);
  EIMSK = (1<<INT0) | (1<<INT1);


  /* EICRA:
  * Este registro tiene 4 pines reservados que siempre deben de ser '0', denominados res
  *
  * res   4 veces
  * ISC11
  * ISC10
  * ISC01
  * ISC00
  *
  * Los ISC1* son para configurar el modo de interrupcion para INT1 que en esta placa es el pin 3
  * y los ISC0* son para INT0 que es el pin 2
  *
  * En este caso vamos a utilizar el pin 2 en flancos de subida asi que seria '11' ISC0
  */

  EICRA = (1<<ISC01) | (1<<ISC11);
  EICRA = (0<<ISC00) | (0<<ISC10);

  sei(); // Volver a habilitar las interrupciones globales


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
  * en este caso '0101' para la entrada 5
  */

  //ADMUX = 010?0101;
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

  //ADCSRA = 10???110;
  ADCSRA = (1<<ADEN) | (0<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);

  Serial.begin(9600);
}

void loop() {
  if (encendido == true) {
    PORTD |= (1<<PD5);
  } else {
    PORTD &= (0<<PD5);
  };

  if (retardo_pulso > 0) {
    retardo_pulso--;
  };

  if (estado == AUTOMATICO) {
    if (lectura_analoga() > 70 ) {
        encendido = false;
    } else {
        encendido = true;
    };
  };

  Serial.println(lectura_analoga());
}



int lectura_analoga() {
  ADCSRA |= (1<<ADSC); // Empezar la lectura

  while (ADCSRA & (1<<ADSC)) {}; // Esperar a que termine la lectura

  return ADC; // Retornar el valor de 10 bits
}
