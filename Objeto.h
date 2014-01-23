/*
  Objeto.h - Representa un objeto en la pantalla.
  Created by Benito Palacios (pleonex), 23/01/2014.
  Licenciado con GPL v3
*/
#ifndef Objeto_h
#define Objeto_h

#include "Arduino.h"

class Objeto
{
  public:
    Objeto(int sizeX, int sizeY);
    
    // Propiedades
    void setPosX(int posX);
    int getPosX();
    void setPosY(int posY);
    int getPosY();
    void setVeloX(int veloX);
    int getVeloX();
    void setVeloY(int veloY);
    int getVeloY();
    int getSizeX();
    int getSizeY();
    
    void mueve();
    void rebota(int dimX, int dimY);
    void rebota(Objeto obj);
    void limita(int dimX, int dimY);
  
  private:  
    // Tamaño del objeto
    int _sizeX;
    int _sizeY;
    
    // Posicion del objeto
    int _posX;
    int _posY;
    
    // Velocidad del objeto
    int _veloX;
    int _veloY;
    
    void desmueve();
};
#endif
