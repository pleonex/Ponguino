/*
  Boton.h - Libreria para controlar botones.
  Created by Benito Palacios (pleonex), 23/01/2014..
  Licenciado con GPL v3
*/
#ifndef Boton_h
#define Boton_h

#include "Arduino.h"

class Boton
{
  public:
    Boton(int pin, bool pullUp);
    bool estaPulsado(); 
  
  private:
    static const long TIEMPO_LIMITE = 50;
  
    int _pin;
    int _pullUp;
    int _estadoAnterior;
    long _tiempoActual;
};
#endif
