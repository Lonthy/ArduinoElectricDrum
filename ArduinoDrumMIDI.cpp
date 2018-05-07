/*Arduino drum MIDI
 *Equipo 1
 *Integrantes:
 *Elena Soledad Medina Favela
 *Rodrigo Moguel Gamboa
 *José Marcos Morales Gómez
 *Víctor Manuel Ortiz García
 *Hardware por: Damian Moreno Mireles*/

/*La biblioteca MIDI.h permite la comunicación MIDI a traves de un puerto USB. Ademas
de que proporciona funciones que facilitan la transferencia de datos MIDI.
En el proyecto se utiliza un puerto USB, pero la transferencia de datos es por medio
del protocolo MIDI.*/
#include <MIDI.h>

/*La funcion MIDI_CREATE_DEFAULT_INSTANCE Configura el puerto USB para la transferencia de datos MIDI*/
MIDI_CREATE_DEFAULT_INSTANCE();

/*Entradas analogicas del Arduino
Se asigna una etiqueta a los puertos analogicos del Arduino. El Arduino UNO consta de 6 puertos, pero en
este caso solamente utilizaremos 5.*/
const short drumPad1 = A0;
const short drumPad2 = A1;
const short drumPad3 = A2;
const short drumPad4 = A3;
const short drumPad5 = A4;

/*constante que representa el numero de Drumpads*/
const short NUMERO_DRUMPADS = 5;

/*Constante que define el volumen minimo o la fuerza minima de un golpe.
Sera usada para determinar si el usuario golpea el
drumpad o si el golpe/vibracion es producido por un agente externo. */
const int volumenMinimo = 280;

/*Mapeo de sonidos en FL Studio. Se le asigna una etiqueta al byte
que define el tono en el mapeo MIDI de FL studio*/
byte tarola = 38;
byte bombo = 36;
byte hihatCerrado = 53;
byte crash = 77;
byte hihatAbierto = 56;

/*Vectores que almacenan la informacion correspondiente a cada Drumpad.*/
short drumPads[NUMERO_DRUMPADS]; //Etiqueta con la que C va a identificar cada puerto analogico del Arduino.
int golpeActual[NUMERO_DRUMPADS];  //Golpe actual en milisegundos.
int golpeAnterior[NUMERO_DRUMPADS]; //Golpe anterior en milisegundos.
float velocidad[NUMERO_DRUMPADS];//Velocidad o volumen del golpe. ****con que se mide?
short nota[NUMERO_DRUMPADS];//Tono de la nota.

/*Prototipos de las funciones hechas por el equipo*/
void validar_datos();
void enviar_datos(short);

/*@brief: Primera funcion que llama el programa al ejecutarse.
  @param: No recibe ningun parametro.
  @return: No devuelve ningun valor.*/
void setup()
{
    MIDI.begin(4);

    /*Inicia la comunicacion serial entre el Arduino y el puerto USB serial.
    El numero representa la frecuencia de la comunicacion serial USB.*/
    Serial.begin(115200);

    /*Inicializacion de los 5 vectores principales que almacenan la informacion de cada Drumpad.*/
    drumPads[0] = drumPad1;
    drumPads[1] = drumPad2;
    drumPads[2] = drumPad3;
    drumPads[3] = drumPad4;
    drumPads[4] = drumPad5;

    int i;
    for(i=1; i<NUMERO_DRUMPADS: i++){
        golpeActual[i] = 0;
        golpeAnterior[i] = 0;
        velocidad[i] = 0;
    }

    nota[0] = tarola;
    nota[1] = bombo;
    nota[2] = hihatCerrado;
    nota[3] = crash;
    nota[4] = hihatAbierto;
}

/*@brief: Funcion que se repite infinitamente.
  @param: No recibe ningun parametro.
  @return: No devuelve ningun valor.*/
void loop(){
    validar_datos();
}

/*@brief: Funcion que se encarga de validar si los datos recibidos cumplen las
caracteristicas necesarias para ser considerados como un golpe.
  @param: No recibe ningun parametro.
  @return: No devuelve ningun valor.*/
void validar_datos(){
    short i;
    for(i=0; i<M; i++){
        velocidad[i] = analogRead(drumPads[i]);
        if(velocidad[i] > (volumenMinimo)){
            golpeActual[i] = millis();
            if(golpeAnterior[i] - golpeActual[i] > 50){
                enviar_datos(i);
                golpeAnterior[i] = golpeActual[i];
            }
        }
    }
}

/*@brief: Funcion que envia los datos que cumplen las caracteristicas para ser un golpe
a traves del puerto USB.
  @param: No recibe ningun parametro.
  @return: No devuelve ningun valor.*/
void enviar_datos(short i){
    /*Protocolo MIDI: Instruccion compuesta por 3 comandos (3 bytes) que representan:
    Byte 1.-Tono de nota Byte 2.-Velocidad/volumen de la nota Byte 3.- Byte de estado.*/
    MIDI.sendNoteOn(nota[i], velocidad[i], 1);
    MIDI.sendNoteOff(nota[i], 0, 1);
}
