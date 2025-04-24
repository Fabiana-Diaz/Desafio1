#ifndef NOMBRE_DE_FUNCIONES_H
#define NOMBRE_DE_FUNCIONES_H

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

unsigned char* aplicarXOR(unsigned char* imagen1, unsigned char* imagen2, int totalBytes);
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif // NOMBRE_DE_FUNCIONES_H
