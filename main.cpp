#include <fstream>
#include <iostream>
#include <QString>
#include <QCoreApplication>
#include <QImage>
#include "nombre_de_funciones.h"

using namespace std;

int main()
{
    int numEnmascaramientos;
    cout << "Ingrese el número de enmascaramientos hechos: ";
    cin >> numEnmascaramientos;

    QString archivoEntrada = "P" + QString::number(numEnmascaramientos) + ".bmp";
    QString archivoSalida = "I_D.bmp";
    QString archivoXOR = "I_M.bmp";
    QString archivoMascara = "M.bmp";

    int width = 0, height = 0;

    // Cargar imágenes
    unsigned char* pixelData = loadPixels(archivoEntrada, width, height);
    unsigned char* pixelMascara = loadPixels(archivoMascara, width, height);
    unsigned char* pixelXor = loadPixels(archivoXOR, width, height);

    if (!pixelData || !pixelMascara || !pixelXor) {
        cout << "Error al cargar una de las imágenes." << endl;
        return -1;
    }

    int totalBytes = width * height * 3;

    // Procesar archivos
    procesarArchivos(numEnmascaramientos, pixelData, pixelMascara, pixelXor, totalBytes); // función que invoca a las demás para que corra el codigo

    // Exportar la imagen reconstruida
    if (!exportImage(pixelData, width, height, archivoSalida)) {
        cout << "Error al exportar la imagen." << endl;
    }

    // Liberar memoria
    delete[] pixelData;
    delete[] pixelMascara;
    delete[] pixelXor;

    return 0;
}



