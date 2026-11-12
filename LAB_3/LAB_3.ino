void setup() {
 
  Inicializar_ADC();

  Inicializar_Telemetria(); 

  Inicializar_Motores();
  
}

bool Control_Manual_Activado = false;
void loop() {
  if (!Control_Manual_Activado) {
    Configurar_Duty_Cycle_Automatico(Leer_Temperatura());
  };

  if (Serial.available() > 0) {
    String lectura_serial = Serial.readStringUntil('\n');
    
    float argumento = lectura_serial.substring(1).toFloat();

    char Comando = lectura_serial[0];

    switch (Comando) {
      case 'T':
        Ajustar_Reduccion(argumento);
      break;
      case 'V':
        Configurar_Duty_Cycle_Manual(argumento, 1);
      break;
      case 'P':
        Configurar_Duty_Cycle_Manual(argumento, 2);
      break;
    };
  };
}