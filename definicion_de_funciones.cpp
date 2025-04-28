
#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;
//funciones dadas por el profe.
unsigned char* loadPixels(QString input, int &width, int &height) {
    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
    height = imagen.height();
    int dataSize = width * height * 3;
    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);
        unsigned char* dstLine = pixelData + y * width * 3;
        memcpy(dstLine, srcLine, width * 3);
    }
    return pixelData;
}

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida) {
    if (pixelData == nullptr) return false;
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }
    if (!outputImage.save(archivoSalida, "BMP")) {
        cout << "Error: No se pudo guardar la imagen BMP." << endl;
        return false;
    }
    cout << "Imagen BMP reconstruida fue guardada como " << archivoSalida.toStdString() << endl;
    return true;
}

unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }
    archivo >> seed;
    int r, g, b;
    while (archivo >> r >> g >> b) {
        n_pixels++;
    }
    archivo.close();
    archivo.open(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }
    unsigned int* RGB = new unsigned int[n_pixels * 3];
    archivo >> seed;
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }
    archivo.close();
    return RGB;
}

bool compararValores(int valorImagen, int valorTxt) {
    return valorImagen == valorTxt; // compara si la imagen y el txt son iguales
}
// comprueba si al aplicar xor y sumarlo a la mascara da los resultados del txt.
bool verificarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int tamMascara, int semilla) {
    for (int i = 0; i < tamMascara; i++) { // recorre desde cero hasta el tamaño de la mascara
        unsigned char dato = img[semilla + i] ^ imgXor[semilla + i]; // calcula el XOR
        int suma = static_cast<int>(dato) + static_cast<int>(mascara[i]);// convierte los datos en enteros y los suma
        if (!compararValores(suma, datosTxt[i])) return false;
    }
    return true;
}

//función Operación xor
bool aplicarXOR(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden) {
    if (verificarXOR(img, mascara, datosTxt, imgXor, tamMascara, semilla)) { //llama a verificarXOR para comprobar que este correcto
        //aplica XOR a toda la imagen
        for (int i = 0; i < totalBytes; ++i) {
            img[i] ^= imgXor[i];
        }
        cout << orden << ") La operacion aplicada fue XOR." << endl;
        return true;
    }
    return false; //retorna true si aplico la operacion y false si falló
}


