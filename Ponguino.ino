/*
    Ponguino. Multiplayer Pong game with Arduino
    Copyright (C) 2014 Benito Palacios (benito356@gmail.com)
  
    This file is part of Ponguino.

    Ponguino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ponguino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ponguino.  If not, see <http://www.gnu.org/licenses/>. 
*/
#include "Boton.h"
#include "Pantalla.h"
#include "Objeto.h"
#define VERSION 0.2

// Matriz de LED
Pantalla pantalla(7, 6, 5);

// Botones para controlar la pala
Boton btnArriba(4, true);
Boton btnAbajo(3, true);

// Objetos
Objeto pala(1, 3);
Objeto bola(1, 1);

// Pin indicador de victoria
int pinLed = 10;

// Variables multijugador
boolean CLIENTE = false;
boolean bolaEnCampo = !CLIENTE;

// Actualizacion del juego
long tiempoJuego = 0;
long tiempoJuegoDelay = 16;  // ~ 60 fps
int refrescoPala = 0;
int refrescoPalaDelay = 10;  // Determina la velocidad de la pala
int refrescoBola = 0;
int refrescoBolaDelay = 15;  // Determina la velocidad de la bola

void setup() {
  pinMode(pinLed, OUTPUT);
  
  // Posicion inicial de la pala
  pala.setPosX(0);
  pala.setPosY(0);
  pala.setVeloY(1);
  
  // Posicion inicial de la bola
  bola.setPosX(5);
  bola.setPosY(4);
  bola.setVeloX(-1);
  bola.setVeloY(-1);
  
  Serial.begin(9600);
  
  if (CLIENTE)
    esperaPing();
  else
    realizaPing();
}

void loop() {
  // Determina si se debe actualizar el juego (fps)
  if (millis() - tiempoJuego > tiempoJuegoDelay) {
    tiempoJuego = millis();  // Reincia el contador
    pantalla.limpia();

    // Actualiza y repinta la pala
    if (++refrescoPala >= refrescoPalaDelay) {
      refrescoPala = 0;
      controlPala();    
    }   
    pantalla.pinta(pala);
    
    if (bolaEnCampo) {
      // Actualiza la bola si procede
      if (++refrescoBola >= refrescoBolaDelay) {
        refrescoBola = 0;
        bola.mueve();
        
        bola.rebota(pala);
        if (bola.getPosX() == 0)
          enviaPunto();
          
        bola.rebota(pantalla.ANCHO, pantalla.ALTO);
        
        // Comprueba si la bola ha pasado al otro campo
        if (bola.getPosX() == pantalla.ANCHO - 1)
          enviaPosicion();
      }
      pantalla.pinta(bola);
    } else {
      recibePosicion(); 
    }
  }
  
  // La pantalla se pinta siempre por la multiplexacion
  pantalla.pinta();
}

/* Comprueba si los controles de la pala han sido pulsados */
void controlPala() {
  if ( btnArriba.estaPulsado() ) {
    pala.setVeloY(1);
    pala.mueve();
    pala.limita(pantalla.ANCHO, pantalla.ALTO);
  }
  
  if ( btnAbajo.estaPulsado() ) {
    pala.setVeloY(-1);
    pala.mueve();
    pala.limita(pantalla.ANCHO, pantalla.ALTO);
  }
}

void enviaPunto() {
  Serial.print('L'); 
}

void recibePosicion() {  
  // Comprueba primero que no sea un punto
  if (Serial.peek() == 'L') {
    Serial.read();
    digitalWrite(pinLed, HIGH);
    delay(1000);
    digitalWrite(pinLed, LOW);
  }
  
  if (Serial.available() < 2)
    return;
  
  bola.setPosX( 7 );
  bola.setPosY( char2int(Serial.read()) );
  bola.setVeloX( -1 );
  bola.setVeloY( char2int(Serial.read()) - 1 );
  bolaEnCampo = true;
}

void enviaPosicion() {
  Serial.print(int2char( bola.getPosY() ));
  Serial.print(int2char( bola.getVeloY() + 1 ));
  bolaEnCampo = false; 
}

void realizaPing() {
  boolean recibido = false;
  
  while (!recibido) {
    // Envia ping
    Serial.print("HOLA");
    
    // Espera dos segundos a una respuesta
    delay(2000);
    
    // Si ha recibido datos los comprueba
    if (Serial.available() >= 4) {
      char msg[5] = "\0\0\0\0";
      Serial.readBytes(msg, 4);
      if (strcmp(msg, "PLAY") == 0)
        recibido = true;
    }
    
    // Limpia el buffer
    while (Serial.available() > 0)
      Serial.read();
  }
}

void esperaPing() {
  boolean recibido = false;
  
  while (!recibido) {
    // Espera a recibir datos
    while (Serial.available() < 4) ;
   
    // Lee y comprueba el buffer
    char msg[5] = "\0\0\0\0";
    Serial.readBytes(msg, 4);
    if (strcmp(msg, "HOLA") == 0)
      recibido = true;
    
    // Limpia el buffer
    while (Serial.available() > 0)
      Serial.read();
  }
  
  Serial.print("PLAY");
}

byte char2int(char ch) {
  if (ch >= '0' && ch <= '9')
   return ch - '0';
  if (ch >= 'A' && ch <= 'F')
   return 0xA + (ch - 'A');
  if (ch >= 'a' && ch <= 'f')
   return 0xA + (ch - 'a');

  return 0xFF; 
}

char int2char(int x) {
  if (x >= 0 && x <= 9)
    return x + '0';
  if (x >= 10 && x <= 15)
    return (x - 10) + 'A';
  
  return 'F';
}
