enum estados {
  Frio,
  Normal,
  Tibio,
  Caliente
};

void setup() {
  Serial.begin(9600);


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

}

void loop() {
  // put your main code here, to run repeatedly:

}

int Leer_Temperatura() {

  cli();

  ADCSRA |= (1<<ADSC); // Empezar la lectura

  while (ADCSRA & (1<<ADSC)) {}; // Esperar a que termine la lectura

  sei();

  return ADC / 10.0; // Retornar el valor de 10 bits
}