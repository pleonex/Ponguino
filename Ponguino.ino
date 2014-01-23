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
#define VERSION 0.1

// Pines para controlar la matriz de LED
int dataPin  = 7;
int latchPin = 6;
int clockPin = 5;

// Variables para controlar la pala
Boton btnArriba(4, true);
Boton btnAbajo(3, true);

// Constantes y variables de la pala
const int PALA_SIZE = 3;
const int PALA_X = 1;
int palaY = 0;

// Variables pelota
int pelotaX = 5;
int pelotaY = 6;
int pelotaVelocX = 1;
int pelotaVelocY = 1;

// Actualizacion del juego
long tiempoJuego = 0;
long tiempoJuegoDelay = 100;
int refrescoBola = 0;
int refrescoBolaDelay = 4;

// Variable con los colores RGB de la pantalla (1 bit depth)
const int ANCHO = 8;
const int ALTO  = 8;
int pantalla[ANCHO][ALTO];
int pantallaAntes[ANCHO][ALTO];

void setup() {
  // Establece el modo de funcionamiento de los pines
  pinMode(dataPin,  OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  for (int x = 0; x < ANCHO; x++) {
    for (int y = 0; y < ALTO; y++) {
      pantalla[x][y] = 0;
      pantallaAntes[x][y] = 0;
    }
  }

  pantalla[PALA_X][0] = 7;
  pantalla[PALA_X][1] = 7;
  pantalla[PALA_X][2] = 7;

  Serial.begin(9600);
}

void loop() {
  if (millis() - tiempoJuego > tiempoJuegoDelay) {
    for (int x = 0; x < ANCHO; x++) {
      for (int y = 0; y < ALTO; y++) {
        pantalla[x][y] = 0;
        pantallaAntes[x][y] = 0;
      }
    }

    controlNave();
    tiempoJuego = millis(); 
    
    for (int y = 0;  y < ALTO; y++)
      pantalla[PALA_X][y] = (y >= palaY && y < palaY + PALA_SIZE) ? 7 : 0;
      
    refrescoBola++;
    if (refrescoBola >= refrescoBolaDelay) {
      refrescoBola = 0;
      
      pelotaX += pelotaVelocX;
      pelotaY += pelotaVelocY;
      
      if (pelotaX == PALA_X && pelotaY >= palaY && pelotaY < palaY + PALA_SIZE) {
        pelotaX -= 2*pelotaVelocX;
        pelotaVelocX *= -1;
      }
      
      if (pelotaX >= ANCHO - 1) {
        pelotaX = ANCHO -1;
        pelotaVelocX = -1;
      } else if (pelotaX <= 0) {
        pelotaX = 0;
        pelotaVelocX = 1;  
      }
      
      if (pelotaY >= ALTO - 1) {
        pelotaY = ALTO - 1;    
        pelotaVelocY = -1;
      } else if (pelotaY <= 0) {
        pelotaY = 0;
       pelotaVelocY = 1; 
      }
    }
    
    pantalla[pelotaX][pelotaY] = 7;
  }
  
  pinta();
}

void controlNave() {
  if ( btnArriba.estaPulsado() )
    palaY = (palaY + PALA_SIZE >= ALTO) ? ALTO - PALA_SIZE : palaY + 1;
  
  if ( btnAbajo.estaPulsado() )
    palaY = (palaY <= 0) ? 0 : palaY - 1;
}

/* Refresca la imagen de la pantalla */
void pinta() {
  for (int x = 0; x < ANCHO; x++) {
    for (int y = 0; y < ALTO; y++) {
      if (pantalla[x][y] != 0 || pantalla[x][y] != pantallaAntes[x][y]) { 
        pintaPixel(x, y, pantalla[x][y]);
        pantallaAntes[x][y] = pantalla[x][y];
      }
    }
  }
}

/* Pinta un pixel en la pantalla */
void pintaPixel(int x, int y, int valor) {
  // Obtiene las tres componentes de color
  int r = (valor >> 2) & 1;
  int g = (valor >> 1) & 1;
  int b = (valor >> 0) & 1;
  
  // Convierto los colores en valores de registro
  r = r << x;
  g = g << x;
  b = b << x;
  
  // Obtengo el valor a escribir en el registro del eje Y
  y = ~(1 << y);
  
  // Los escribe en el registro de desplazamiento
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, b);
  shiftOut(dataPin, clockPin, LSBFIRST, y);
  digitalWrite(latchPin, HIGH);
}
