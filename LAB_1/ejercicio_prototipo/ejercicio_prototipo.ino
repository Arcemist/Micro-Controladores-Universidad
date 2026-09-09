enum Modo {
  AUTOMATICO = 0,
  MANUAL = 1
};
enum Modo estado;

int retardo_pulso;

const byte pin_boton = 2;
const byte pin_modo = 3;
const byte pin_luz = 5;
const int pin_analogo = A3;

bool encendido = false;

void setup() {
  Serial.begin(9600); // solo permanece aca para ver los valores de la resistencia
  estado = AUTOMATICO;

  retardo_pulso = 0;

  pinMode(pin_luz, OUTPUT);

  pinMode(pin_boton, INPUT_PULLUP);
  pinMode(pin_modo, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_boton), boton_presionado, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin_modo), boton_modo, FALLING);
}

void loop() {
  if (retardo_pulso > 0) {
    retardo_pulso--;
  };

  digitalWrite(pin_luz, encendido);

  if (estado == AUTOMATICO) {
    //Serial.println(analogread(pin_)); //Usar para ver los valores de la resistencia

    if (analogRead(pin_analogo) > 100 ) {
        encendido = false;
    } else {
        encendido = true;
    };
  };
}

void boton_presionado() {
  if ((retardo_pulso == 0) && (estado == MANUAL)) { // Esto sirve para asegurar pero conviene poner un capacitor igual
    retardo_pulso = 2000;
    encendido = !encendido;
  }; 
}

void boton_modo() {
  if (estado == AUTOMATICO) {
    estado = MANUAL;
  } else {
    estado = AUTOMATICO;
  };
}
