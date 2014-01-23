/*
  Boton.cpp - Libreria para controlar botones.
  Created by Benito Palacios (pleonex), 23/01/2014..
  Licenciado con GPL v3
*/
#include "Arduino.h"
#include "Boton.h"

Boton::Boton(int pin, bool pullUp)
{
  _pin = pin;
  _pullUp = pullUp;
  _estadoAnterior = pullUp;
  _tiempoActual = 0;
  
  pinMode(pin, INPUT);
}

/* Comprueba si se ha pulsado el botón. */
bool Boton::estaPulsado()
{
  bool pulsado = false;
  int lectura = digitalRead(_pin);
  if (lectura != _estadoAnterior)
    _tiempoActual = millis();
  
  if (millis() - _tiempoActual > TIEMPO_LIMITE && lectura == !_pullUp)
    pulsado = true;
  
  _estadoAnterior = lectura;
  return pulsado;
}
