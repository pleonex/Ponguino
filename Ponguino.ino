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
long tiempoJuegoDelay = 100;
int refrescoBola = 0;
int refrescoBolaDelay = 4;

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
  if (millis() - tiempoJuego > tiempoJuegoDelay) {
    pantalla.limpia();

    controlNave();
    tiempoJuego = millis(); 
    
    pantalla.pinta(pala);
      
    refrescoBola++;
    if (refrescoBola >= refrescoBolaDelay) {
      refrescoBola = 0;
      
      bola.mueve();
      bola.rebota(pantalla.ANCHO, pantalla.ALTO);
      bola.rebota(pala);
    }
    
    pantalla.pinta(bola);
  }
  
  pantalla.pinta();
}

void controlNave() {
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
