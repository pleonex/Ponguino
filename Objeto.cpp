/*
  Objeto.cpp - Representa un objeto en la pantalla.
  Created by Benito Palacios (pleonex), 23/01/2014..
  Licenciado con GPL v3
*/
#include "Arduino.h"
#include "Objeto.h"

Objeto::Objeto(int sizeX, int sizeY) {
  _sizeX = sizeX;
  _sizeY = sizeY;
}

// Propiedades
void Objeto::setPosX(int posX) { _posX = posX; }
int Objeto::getPosX() { return _posX; }
void Objeto::setPosY(int posY) { _posY = posY; }
int Objeto::getPosY() { return _posY; }
void Objeto::setVeloX(int veloX) { _veloX = veloX; }
int Objeto::getVeloX() { return _veloX; }
void Objeto::setVeloY(int veloY) { _veloY = veloY; }
int Objeto::getVeloY() { return _veloY; }
int Objeto::getSizeX() { return _sizeX; }
int Objeto::getSizeY() { return _sizeY; }

void Objeto::mueve() {
  _posX += _veloX;
  _posY += _veloY; 
}

void Objeto::desmueve() {
  _posX -= _veloX;
  _posY -= _veloY; 
}

void Objeto::limita(int dimX, int dimY) {  
  if (_posX < 0 || _posX + _sizeX > dimX ||
      _posY < 0 || _posY + _sizeY > dimY)
    desmueve();
}

void Objeto::rebota(int dimX, int dimY) {  
  // Limite eje X (deshace movimiento)
  if (_posX < 0 || _posX + _sizeX > dimX) {
    desmueve();
    _veloX *= -1;
    mueve(); 
  }
 
  // Limite eje Y (deshace movimiento)
  if (_posY < 0 || _posY + _sizeY > dimY) {
    desmueve();
    _veloY *= -1; 
    mueve();
  }
}

void Objeto::rebota(Objeto obj) {
  //if (_posX >= obj.getPosX() && _posX < obj.getPosX() + obj.getSizeX())
  //  return;
  if (_posX == obj.getPosX() && _posY >= obj.getPosY() && _posY < obj.getPosY() + obj.getSizeY()) {
    desmueve();
    _veloX *= -1;
    mueve(); 
  }
}
