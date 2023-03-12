
#include <ATDelay.h>

//Pines de los motores
int motorIalante = 5;
int motorIatras = 4;
int motorDalante = 7;
int motorDatras = 8;
int PWM_MotorD = 9;
int PWM_MotorI = 3;

int estanbay = 6;


//Pines de los interruptores para diferentes estrategias
int interruptor[] = {2, 10, 11, 12};

//Pines sensores de suelo (CNY70) visto desde arriba con la cuchilla hacia adelante
int sensores[] = {A2, A1, A6, A5};

int alante_I = 0;//A2
int alante_D = 0;//A1
int atras_I = 0;//A6
int atras_D = 0;//A5

//Pines sensores frontales (SHARP)
int frontal_D = A3;
int frontal_I = A4;

int enemigo_D = 0;
int enemigo_I = 0;

//Pin boton
int boton = A0;

//Variables calibracion
int lectura[] = {0, 0, 0, 0};
int negros[] = {0, 0, 0, 0};
int blancos[] = {1023, 1023, 1023, 1023};
int umbrales[] = {0, 0, 0, 0};

//variables de la estrategia
int estrategia = 0;
int binario = 1;
int pin[] = {0, 0, 0, 0};
bool arranque = false;

//variables generales
int vel_robot_D = 0;
int vel_robot_I = 0;
int vel_base = 100;
int vel_arranque = 195;
int vel_max = 255;
double millisInicio = 0;
double millisAnterior = 0;
double millisAnterior_D = 0;
double millisAnterior_I = 0;
int i = 0;

//banderas
bool bandera_Atdelay = true;
bool bandera11 = false;
bool bandera15 = false;


//array que contienen los valores de media onda senoidal en este caso están los valores correspondientes a media onda senoidal
//empezando en un pico de la onda y acabando en el pico contrario. contiene 127 posiciones.


byte Curva_seno[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11,
                     12, 14, 15, 16, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 42,
                     44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 78, 81, 84, 87, 90,
                     93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124, 127, 130, 133,
                     136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176,
                     178, 181, 184, 187, 190, 192, 195, 198, 200, 203, 205, 208, 210, 212,
                     215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239,
                     240, 242, 243, 244, 245, 247, 248, 249, 249, 250, 251, 252, 252, 253,
                     253, 253, 255, 255, 255, 255
                    };



ATDelay estrg_1(140);
ATDelay estrg_2(140);
ATDelay estrg_3(240);
ATDelay estrg_3_1(200);
ATDelay estrg_4(250);
ATDelay estrg_4_1(350);
ATDelay estrg_7(230);
ATDelay estrg_7_1(200);
ATDelay estrg_7_2(250);
ATDelay estrg_9(2000);
ATDelay estrg_10(2000);
ATDelay estrg_11(100);
ATDelay estrg_16(200);
ATDelay estrg_16_1(200);
ATDelay siguelineas(200);
ATDelay turrabot(500);
ATDelay floor_1(200);
ATDelay floor_2(500);
void setup() {
  pinMode(motorIalante, OUTPUT);
  pinMode(motorIatras, OUTPUT);
  pinMode(motorDalante, OUTPUT);
  pinMode(motorDatras, OUTPUT);

  pinMode( PWM_MotorD, OUTPUT);
  pinMode( PWM_MotorI, OUTPUT);

  pinMode( alante_I, INPUT);
  pinMode( alante_D, INPUT);
  pinMode( atras_I, INPUT);
  pinMode( atras_D, INPUT);

  pinMode( frontal_D, INPUT);
  pinMode( frontal_I, INPUT);

  pinMode( boton, INPUT_PULLUP);

  for (i = 0; i < 4; i++) {
    pinMode(interruptor[i], INPUT_PULLUP);
  }
  pinMode(13, OUTPUT);


  digitalWrite(motorIalante, LOW);
  digitalWrite(motorIatras, LOW);
  digitalWrite(motorDalante, LOW);
  digitalWrite(motorDatras, LOW);
  digitalWrite(estanbay, HIGH);

  Serial.begin(9600);
  delay(500);




  // ========================================================
  //  realizamos la lectura de la estrategia
  // ========================================================

  for (i = 3; i >= 0; i--) {
    pin[i] = !digitalRead(interruptor[i]);
    estrategia = estrategia + (pin[i] * binario);
    binario = binario * 2;
  }
  Serial.println(estrategia);



  // delay(5000);
} //fin del setup


void loop() {
  if (estrategia == 8){

        enemigo_D = analogRead(frontal_D);
        enemigo_I = analogRead(frontal_I);
        if (enemigo_I > 200 || enemigo_D > 200) {
          digitalWrite(13, HIGH);
        } else {
          digitalWrite(13, LOW);
        }
    return;
  }
  if (digitalRead(boton) == true) {
    digitalWrite(13, HIGH);
  }


  //=============================================================
  //Arrancar a los 5 segundos de la orden
  //La orden se activa en cuanto sueltes el boton, si lo vuelves a pulsar antes de 5s se anula
  //==============================================================

  // /*
  if (digitalRead(boton) == false) {
    digitalWrite(13, LOW);
    arranque = true;
    millisInicio = millis();
    bandera_Atdelay = true;
  } else if ((millis() >= (millisInicio + 5000)) && (arranque == true) && (millisInicio > 0)) {
    // */
    if (bandera_Atdelay == true) {
      estrg_1.reset();
      estrg_2.reset();
      estrg_3.reset();
      estrg_3_1.reset();
      estrg_4.reset();
      estrg_4_1.reset();
      estrg_7.reset();
      estrg_7_1.reset();
      estrg_7_2.reset();
      estrg_9.reset();
      estrg_10.reset();
      estrg_11.reset();
      bandera_Atdelay = false;
    }

    // =======================================================
    // Switch de la virgen con todas las estrategias, basicamente aqui esta el programa
    // =======================================================



    switch (estrategia) {

      case 0: //Salida de frente (rival mas lento)
        vel_robot_D = vel_base;
        vel_robot_I = vel_base;
        estrategia = 16;
        break;
      case 1: //Salida de lado giro a la derecha(rival mas lento)
        if (!estrg_1.check()) {
          vel_robot_D = -vel_arranque;
          vel_robot_I = vel_arranque;
        } else {
          estrategia = 16;
        }
        break;
      case 2: //Salida de lado giro a la izquierda (rival mas lento)
        if (!estrg_2.check()) {
          vel_robot_I = vel_arranque;
          vel_robot_D = -vel_arranque;
        } else {
          estrategia = 16;
        }
        break;
      case 3:  //Salida de espaldas (rival mas lento)
        if (!estrg_3.check()) {
          vel_robot_I = -vel_arranque;
          vel_robot_D = vel_arranque;
          estrg_3_1.reset();
        } else {
          if (!estrg_3_1.check()) {
            vel_robot_I = vel_max;
            vel_robot_D = vel_max;
          }
          else {
            estrategia = 16;
          }
        }
        break;
      case 4:  //Salida de frente (rival mas rapido)
        if (!estrg_4.check()) {
          vel_robot_I = vel_max;
          vel_robot_D = 80;
          estrg_4_1.reset();
        } else {
          if (!estrg_4_1.check()) {
            vel_robot_I = 50;
            vel_robot_D = vel_max;
          }
          else {
            estrategia = 16;
          }
        }
        break;
      case 5: //Salida de lado giro a la derecha (rival mas rapido)

        break;
      case 6: //Salida de lado giro a la izquierda (rival mas rapido)

        break;
      case 7: //Salida de espaldas (rival mas rapido)
        if (!estrg_7.check()) {
          vel_robot_I = -100;
          vel_robot_D = -vel_max;
          estrg_7_1.reset();
        } else {
          if (!estrg_7_1.check()) {
            vel_robot_I = -vel_max;
            vel_robot_D = -vel_max;
            estrg_7_2.reset();
          }
          else {
            if (!estrg_7_2.check()) {
              vel_robot_I = vel_max;
              vel_robot_D = vel_max;
            }
            else {
              estrategia = 16;
            }
          }
        }
        break;
      case 8:

        enemigo_D = analogRead(frontal_D);
        enemigo_I = analogRead(frontal_I);
        if (enemigo_I > 200 || enemigo_D > 200) {
          digitalWrite(13, HIGH);
        } else {
          digitalWrite(13, LOW);
        }
        break;
      case 9:  //Medir fuerza
        if (!estrg_9.check()) {
          vel_robot_I = vel_max;
          vel_robot_D = vel_max;
        } else {
          vel_robot_I = 0;
          vel_robot_D = 0;
        }
        break;
      case 10: //Medir giro
        if (!estrg_10.check()) {
          vel_robot_I = vel_max;
          vel_robot_D = -vel_max;
        } else {
          vel_robot_I = 0;
          vel_robot_D = 0;
        }
        break;

      case 11: //Medir frenada

        alante_I = analogRead (sensores[0]);
        alante_D = analogRead(sensores[1]);

        vel_robot_I = vel_max;
        vel_robot_D = vel_max;
        if (alante_I >= 900 || alante_D >= 900 || bandera11 == true) {
          bandera11 = true;
          if (!estrg_11.check()) {
            vel_robot_I = -vel_max;
            vel_robot_D = -vel_max;
          } else {
            vel_robot_I = 0;
            vel_robot_D = 0;
          }
        }
        break;
      case 12:  //Line follower
        alante_I = analogRead(sensores[0]);
        alante_D = analogRead(sensores[1]);
        enemigo_D = analogRead(frontal_D);
        enemigo_I = analogRead(frontal_I);

        if (enemigo_I > 200 || enemigo_D > 200) {
          estrategia = 16;
        }

        if (alante_I < 900 && alante_D < 900) {
          vel_robot_D = vel_base;
          vel_robot_I = vel_base;
        } else if (alante_I >= 900 && alante_D < 900) {
          vel_robot_D = -vel_base;
          vel_robot_I = vel_base;

        } else if (alante_I < 900 && alante_D >= 900) {
          vel_robot_D = vel_base;
          vel_robot_I = -vel_base;

        } else if (alante_I >= 900 && alante_D >= 900) {
          estrategia = 15;
        }

        break;
      case 13:  //Crazy

        break;
      case 14:  //Turrabot

        break;
      case 15:  //Don´t touch the floor
        alante_I = analogRead (sensores[0]);
        alante_D = analogRead(sensores[1]);
        enemigo_D = analogRead(frontal_D);
        enemigo_I = analogRead(frontal_I);

        if (enemigo_I > 200 || enemigo_D > 200) {
          estrategia = 16;
        }

        if (alante_I >= 980 || alante_D >= 980 || bandera15 == true) {
          bandera15 = true;
          vel_robot_D = -vel_max;
          vel_robot_I = -vel_max;
          if (floor_1.check()) {
            vel_robot_D = vel_base;
            if (floor_2.check()) {
              vel_robot_I = vel_base;
              floor_1.reset();
              floor_2.reset();
              bandera15 = false;
            }
          }
        }

        break;
      case 16:  //ataque (la idea es que aqui este la mayoria del tiempo. intentara no perder al rival e ir a por el)

        alante_I = analogRead (sensores[0]);
        alante_D = analogRead(sensores[1]);
        enemigo_D = analogRead(frontal_D);
        enemigo_I = analogRead(frontal_I);


        if (enemigo_I > 200 && enemigo_D > 200) {
          vel_robot_D = vel_max;
          vel_robot_I = vel_max;
        }
        if (enemigo_I > 200 && enemigo_D <= 200) {
          vel_robot_D = 200;
          vel_robot_I = 30;
        }
        if (enemigo_I <= 200 && enemigo_D > 200) {
          vel_robot_D = 30;
          vel_robot_I = 200;
        }
        if (enemigo_I <= 200 && enemigo_D <= 200) {
          estrategia = 15;
        }



        break;

    }




    // Serial.println(" ");
    // =========================================================
    // Asinacion de velocidad a los motores
    // ========================================================

    //asiganmos valores a la rueda derecha teniendo en cuenta de que si el valor es negativo va hacia atras
    if (vel_robot_D >= 0) {
      digitalWrite(motorDalante, HIGH);
      digitalWrite(motorDatras, LOW);
      analogWrite(PWM_MotorD, vel_robot_D);
    } else {
      digitalWrite(motorDalante, LOW);
      digitalWrite(motorDatras, HIGH);
      analogWrite(PWM_MotorD, abs(vel_robot_D));
    }

    //asiganmos valores a la rueda izquierda teniendo en cuenta de que si el valor es negativo va hacia atras
    if (vel_robot_I >= 0) {
      digitalWrite(motorIalante, HIGH);
      digitalWrite(motorIatras, LOW);
      analogWrite(PWM_MotorI, vel_robot_I);
    } else {
      digitalWrite(motorIalante, LOW);
      digitalWrite(motorIatras, HIGH);
      analogWrite(PWM_MotorI, abs(vel_robot_I));
    }


  } //if del arranque

} //fin del loop
