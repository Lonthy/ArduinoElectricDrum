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

/*Constante que representa el numero de Drumpads*/
const short NUMERO_DRUMPADS = 5;

/*Constante que define el volumen minimo o la fuerza minima de un golpe.
Sera usada para determinar si el usuario golpea el
drumpad o si el golpe/vibracion es producido por un agente externo. */
const int volumenMinimo = 30;

/*Mapeo de sonidos en FL Studio. Se le asigna una etiqueta al byte
que define el tono en el mapeo MIDI de FL studio*/
const byte tarola = 38;
const byte bombo = 36;
const byte hihatCerrado = 53;
const byte crash = 77;
const byte hihatAbierto = 56;

/*Estructura que representara a cada uno de los drumpads y sus propiedades*/
struct drumpad{
  short etiqueta, nota;
  float golpeActual, golpeAnterior, velocidad;
};

/*Cambio de nombre de struct drumpad a Drumpad para facilitar su uso*/
typedef struct drumpad Drumpad;

/*Declaracion de un vector de tipo "struct drumpad" que almacenara las propiedades de cada sensor*/
Drumpad vectorDrumpad[NUMERO_DRUMPADS];

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

    /*Inicializacion del vector que contiene las 5 propiedades de cada Drumpad.*/
    vectorDrumpad[0].etiqueta = drumPad1;
    vectorDrumpad[1].etiqueta = drumPad2;
    vectorDrumpad[2].etiqueta = drumPad3;
    vectorDrumpad[3].etiqueta = drumPad4;
    vectorDrumpad[4].etiqueta = drumPad5;

    int i;
    for(i=1; i<NUMERO_DRUMPADS; i++){
        vectorDrumpad[i].golpeActual = 0;
        vectorDrumpad[i].golpeAnterior = 0;
        vectorDrumpad[i].velocidad = 0;
    }

    vectorDrumpad[0].nota = tarola;
    vectorDrumpad[1].nota = bombo;
    vectorDrumpad[2].nota = hihatCerrado;
    vectorDrumpad[3].nota = crash;
    vectorDrumpad[4].nota = hihatAbierto;
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
    for(i=0; i<NUMERO_DRUMPADS; i++){
        vectorDrumpad[i].velocidad = analogRead(vectorDrumpad[i].etiqueta);
        if(vectorDrumpad[i].velocidad > (volumenMinimo)){
            vectorDrumpad[i].golpeActual = millis();
            if(vectorDrumpad[i].golpeActual - vectorDrumpad[i].golpeAnterior > 10){
                enviar_datos(i);
                vectorDrumpad[i].golpeAnterior = vectorDrumpad[i].golpeActual;
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
    MIDI.sendNoteOn(vectorDrumpad[i].nota, vectorDrumpad[i].velocidad, 1);
    MIDI.sendNoteOff(vectorDrumpad[i].nota, 0, 1);
}
