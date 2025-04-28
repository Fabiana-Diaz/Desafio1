
#ifndef NOMBRE_DE_FUNCIONES_H
#define NOMBRE_DE_FUNCIONES_H

#include <QString>

// Funciones dadas por el profe.
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

//funcion para comparar valores
bool compararValores(int valorImagen, int valorTxt);
//funciones para verificar y aplicar xor
bool verificarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int tamMascara, int semilla);
bool aplicarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden);

bool verificarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda);
bool aplicarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden);

bool verificarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda);
bool aplicarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden);

void aplicarTransformaciones(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden);

#endif // NOMBRE_DE_FUNCIONES_H
