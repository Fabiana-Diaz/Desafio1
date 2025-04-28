#ifndef NOMBRE_DE_FUNCIONES_H
#define NOMBRE_DE_FUNCIONES_H

#include <QString>

// Funciones originales (NO cambiar nombres)
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

// Nuevas funciones mejoradas
bool compararValores(int valorImagen, int valorTxt);

bool verificarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int tamMascara, int semilla);
bool aplicarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden);

bool verificarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda);
bool aplicarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden);

bool verificarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda);
bool aplicarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden);

void aplicarTransformaciones(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden);

void procesarArchivos(int nArchivos, unsigned char* img, unsigned char* mascara, unsigned char* imgXor, int totalBytes);

#endif // NOMBRE_DE_FUNCIONES_H
