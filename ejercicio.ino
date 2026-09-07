enum Modo {
  AUTOMATICO = 0,
  MANUAL = 1
};
enum Modo estado;

byte retardo_pulso;

const byte pin_boton = 2;
const byte pin_luz = 5;
const int pin_analogo = A3;

void setup() {
  Serial.begin(9600);
  estado = AUTOMATICO;

  retardo_pulso = 0;

  pinMode(pin_luz, OUTPUT);

  pinMode(pin_boton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_boton), boton_presionado, RISING);
}

void loop() {
  if (retardo_pulso > 1) {
    retardo_pulso--;
  };
  delay(10);

  if (estado == AUTOMATICO) {
    Serial.println(analogRead(pin_analogo)); //Usar para ver los valores de la resistencia

    //if (analogRead(pin_analogo) > VALOR QUE TODAVIA NO SE ) {
    //    APAGAR LED
    //} else {
    //    ENCENDER LED
    //};
  };
}

void boton_presionado() {
  estado = MANUAL;
  if (retardo_pulso == 0) { // Esto sirve para asegurar pero conviene poner un capacitor igual
    retardo_pulso = 50;


  }; 
}