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

// Actualizacion del juego
long tiempoJuego = 0;
long tiempoJuegoDelay = 16;  // ~ 60 fps
int refrescoPala = 0;
int refrescoPalaDelay = 10;  // Determina la velocidad de la pala
int refrescoBola = 0;
int refrescoBolaDelay = 15;  // Determina la velocidad de la bola

void setup() {
  // Posicion inicial de la pala
  pala.setPosX(0);
  pala.setPosY(0);
  pala.setVeloY(1);
  
  // Posicion inicial de la bola
  bola.setPosX(5);
  bola.setPosY(6);
  bola.setVeloX(1);
  bola.setVeloY(1);
  
  Serial.begin(9600);
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
      
    // Actualiza la bola si procede
    if (++refrescoBola >= refrescoBolaDelay) {
      refrescoBola = 0;
      bola.mueve();
      bola.rebota(pantalla.ANCHO, pantalla.ALTO);
      bola.rebota(pala);
    }
    pantalla.pinta(bola);
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
