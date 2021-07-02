#include "proyInstrumentacion.h"

void setup(){
  Serial.begin(BAUDRATE);                // Inicializamos la comunicaciÃ³n serial
  analogReference(INTERNAL);
  bufferString.reserve(BUFFER_STRING);         // Buffer de datos Serial
  DDRD = CNF_PORTD;                  // Inicializa el puerto D.
  PORTD = INI_PORTD;
  DDRB = CNF_PORTB;
  PORTB = INI_PORTB;
  // Configuración de Timer1 
  TCCR1A = 0;                        // El registro de control A queda todo en 0, pines OC1A y OC1B deshabilitados
  TCCR1B = 0;                        // Limpia el registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);   // Configura prescaler para 1024: CS12 = 1 e CS10 = 1 
  TCNT1 = INI_TNT;                    // Inicia timer para desbordamiento 1ms --> 65536-(16MHz/1024/1KHz) = 65520 = 0xFFF0  
  TIMSK1 |= (1 << TOIE1);            // Habilita la interrupcion del TIMER1
  // Tareas
  tareas.actionClock = TAREA_DES;
  tareas.sendData = TAREA_DES;
  tareas.led1On = TAREA_DES;
  tareas.led1Off = TAREA_DES;
  delay(100); //Los retardos son necesarios en la practica para mejorar desempeño
}

void loop(){
  if (tareas.actionClock){
    tareas.actionClock = TAREA_DES;
    // Leer sensor !
    volt = analogRead(A0) * factVolt;
    //temp = (150+55)/(2.905 - 0.85)*(volt - 0.85) - 55;
    temp = 1.0/10e-3 * volt;
    if (tareas.sendData) sendData();
    if (temp < 36.0)tareas.led1On = TAREA_ACT;
    else tareas.led1Off = TAREA_ACT;
  }
  if (tareas.led1On) led1On();
  if (tareas.led1Off)led1Off();
}

void serialEvent(){
  while (Serial.available()){
    char inChar = (char)Serial.read();
    bufferString += inChar;
    if(inChar == '\n'){  //End line ! 
      //Serial.print(bufferString);  
      if (bufferString.equals("led1On\n")){
        tareas.led1On = TAREA_ACT;
      }if (bufferString.equals("led1Off\n")){
        tareas.led1Off = TAREA_ACT;
      }else if (bufferString.equals("getData\n")){
        tareas.sendData = TAREA_ACT;
      }else if (bufferString.equals("endData\n")){
        tareas.sendData = TAREA_DES;
      }
      bufferString = "";
    }
  }
}

ISR(TIMER1_OVF_vect){                 // Interrupcion del TIMER1
  TCNT1 = INI_TNT;                    // Renicia TIMER 1
  k++;
  if(k >= PERIOD && tareas.sendData){  
    tareas.actionClock = TAREA_ACT;
    PORTB ^= MASK_LED_CLOCK;
    k = TAREA_DES;
  }
}

void led1On(){
  tareas.led1On = TAREA_DES;
  PORTD &= MASK_LED_ON;
}
void led1Off(){
  tareas.led1Off = TAREA_DES;
  PORTD |= MASK_LED_OFF;
}

void sendData(){
  Serial.println(temp);
}
