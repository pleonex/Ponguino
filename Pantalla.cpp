/*
  Pantalla.cpp - Libreria para controlar una matriz RGB de LED
  Created by Benito Palacios (pleonex), 23/01/2014..
  Licenciado con GPL v3
*/
#include "Arduino.h"
#include "Pantalla.h"

Pantalla::Pantalla(int pinData, int pinLatch, int pinClock) {
  _pinData  = pinData;
  _pinLatch = pinLatch;
  _pinClock = pinClock;
  
  pinMode(pinData,  OUTPUT);
  pinMode(pinLatch, OUTPUT);
  pinMode(pinClock, OUTPUT);
  
  limpia();
}

/* Limpia la pantalla. Apaga todos los LED */
void Pantalla::limpia() {
  // Establece todos los LED en apagado.
  for (int x = 0; x < ANCHO; x++)
    for (int y = 0; y < ALTO; y++)
      pantalla[x][y] = 0;
}

/* Establece el valor de un pixel de la pantalla */
void Pantalla::pixel(int x, int y, int r, int g, int b) {
  // Asegura que la posicion esta en los limites de la matriz
  if (x < 0 || x >= ANCHO || y < 0 || y >= ALTO)
    return;
  
  // Asegura que cada componente tiene un unico bit.
  r &= 1;
  g &= 1;
  b &= 1;
  
  // Establece el pixel
  pantalla[x][y] = (r << 2) | (g << 1) | b;
}

/* Refresca la imagen de la pantalla */
void Pantalla::pinta() {
  // Recorrer todos los pixels de la pantalla (multiplexacion)
  for (int x = 0; x < ANCHO; x++)
    for (int y = 0; y < ALTO; y++)
      if (pantalla[x][y] != 0)     // Si el LED se ha de encender
        pintaPixel(x, y, pantalla[x][y]);
}

void Pantalla::pinta(Objeto obj) {
  // Comprueba las dimensiones
  if (obj.getPosX() < 0 || obj.getPosX() + obj.getSizeX() > ANCHO ||
      obj.getPosY() < 0 || obj.getPosY() + obj.getSizeY() > ALTO)
    return;  
  
  // Pinta el objeto
  for (int x = obj.getPosX(); x < obj.getPosX() + obj.getSizeX(); x++)
    for (int y = obj.getPosY(); y < obj.getPosY() + obj.getSizeY(); y++)
      pixel(x, y, 0, 0, 1);
}

/* Pinta un pixel en la pantalla */
void Pantalla::pintaPixel(int x, int y, int valor) {
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
  digitalWrite(_pinLatch, LOW);
  shiftOut(_pinData, _pinClock, MSBFIRST, b);
  // TODO: Implementar registros rojo y ver
  shiftOut(_pinData, _pinClock, LSBFIRST, y);
  digitalWrite(_pinLatch, HIGH);
}
