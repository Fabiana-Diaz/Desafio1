#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "nombre_de_funciones.h"

using namespace std;

int main()
{
    // Definición de rutas de archivo de entrada (imagen original) y salida (imagen modificada)
    QString archivoEntrada = "I_O.bmp";
    QString archivoSalida = "I_D.bmp";
    QString imagen_comp = "I_M.bmp";

    // Variables para almacenar las dimensiones de la imagen
    int height = 0; //altura en pixeles
    int width = 0;  //ancho en pixeles

    // Carga la imagen BMP en memoria dinámica y obtiene ancho y alto
    unsigned char *pixelData = loadPixels(archivoEntrada, width, height);
    unsigned char *pixelData2 = loadPixels(imagen_comp, width, height);


    // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
    int seed = 0;
    int n_pixels = 0;

    // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
    unsigned int *maskingData = loadSeedMasking("M1.txt", seed, n_pixels);

    // Muestra en consola los primeros valores RGB leídos desde el archivo de enmascaramiento
    for (int i = 0; i < n_pixels * 3; i += 3) {
        cout << "Pixel " << i / 3 << ": ("
             << maskingData[i] << ", "
             << maskingData[i + 1] << ", "
             << maskingData[i + 2] << ")" << endl;
    }

    int total_bits = (height * width * 3);
    unsigned char* resultado = aplicarXOR(pixelData, pixelData2, total_bits);

    // Exporta la imagen modificada a un nuevo archivo BMP
    bool exportI = exportImage(resultado, width, height, archivoSalida);

    // Muestra si la exportación fue exitosa (true o false)
    cout << exportI << endl;

    // Libera la memoria usada para los píxeles y los datos de enmascaramiento
    delete[] pixelData;
    pixelData = nullptr;

    if (maskingData != nullptr) {
        delete[] maskingData;
        maskingData = nullptr;
    }

    return 0; // Fin del programa
}

