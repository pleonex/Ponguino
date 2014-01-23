/*
  Pantalla.h - Libreria para controlar una matriz RGB de LED.
  Created by Benito Palacios (pleonex), 23/01/2014..
  Licenciado con GPL v3
*/
#ifndef Pantalla_h
#define Pantalla_h

#include "Arduino.h"

class Pantalla
{
  public:
    Pantalla(int pinData, int pinLatch, int pinClock);
    void limpia();
    void pinta();
    void pixel(int x, int y, int r, int g, int b);
  
  private:
    static const int ANCHO = 8;
    static const int ALTO  = 8;
  
    // Pines de los registro de desplazamiento
    int _pinData;
    int _pinLatch;
    int _pinClock;
    
    // Variable con los colores RGB de la pantalla (1 bit depth)
    int pantalla[ANCHO][ALTO];
    
    void pintaPixel(int x, int y, int valor);
};
#endif
