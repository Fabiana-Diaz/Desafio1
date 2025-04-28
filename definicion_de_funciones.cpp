
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

//comprueba si al aplicar la rotación y sumar los datos con la mascara estos coinciden con el txt
bool verificarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda) {
for (int i = 0; i < tamMascara; i++) { //recorre los bytes de la imagen
    unsigned char dato = izquierda ? (img[semilla + i] << nBits) | (img[semilla + i] >> (8 - nBits)) : // si es true rota a la izquierda, si es false a la izquierda
                             (img[semilla + i] >> nBits) | (img[semilla + i] << (8 - nBits));
    int suma = static_cast<int>(dato) + static_cast<int>(mascara[i]); //sumamos el dato rotado más la mascara, estos valores los convertimos en enetos
    if (!compararValores(suma, datosTxt[i])) return false; // verificamos si los datos coinciden o no
}
return true; //retornamos true si todo esta bien
}

//función para rotar toda la imagen
bool aplicarRotacion(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden) {
    if (verificarRotacion(img, mascara, datosTxt, tamMascara, semilla, nBits, izquierda)) { //se verifica que sean los datos esperados
        for (int i = 0; i < totalBytes; ++i) {
            img[i] = izquierda ? (img[i] << nBits) | (img[i] >> (8 - nBits)) : //se aplica rotación a cada bit de la imagen, ya sea izquierda si es true o derecha si es false
                         (img[i] >> nBits) | (img[i] << (8 - nBits));
        }
        string dir = izquierda ? "derecha" : "izquierda"; // muestra en consola la rotacion que se hico
        cout << orden << ") La operación aplicada fue una rotación de " << nBits << " bits a la " << dir << "." << endl;
        return true;
    }
    return false; //si todo estuvo bien se retorna true y si todo estubo mal, false
}

// Función que verifica si al desplazar los bits de la imagen (a izquierda o derecha) y sumar la máscara,
// se obtiene el mismo resultado que los datos esperados (del archivo txt)

bool verificarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int tamMascara, int semilla, int nBits, bool izquierda) {
    for (int i = 0; i < tamMascara; i++) {
        // Aplica desplazamiento: si 'izquierda' es true, desplaza hacia la izquierda; si no, hacia la derecha
        unsigned char dato = izquierda ? img[semilla + i] << nBits : img[semilla + i] >> nBits;
        // Suma el dato desplazado con la máscara
        int suma = static_cast<int>(dato) + static_cast<int>(mascara[i]);
        // Compara la suma con el dato esperado del archivo txt
        if (!compararValores(suma, datosTxt[i])) return false; // Retorna false si algún dato no coincide
    }
    return true; //Retorna true si todos los datos coinciden
}

// Función que aplica un desplazamiento a todos los bytes de la imagen si la verificación es exitosa
bool aplicarDesplazamiento(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, int totalBytes, int tamMascara, int semilla, int nBits, bool izquierda, int orden) {
    // Verifica si el desplazamiento produciría los resultados esperados
    if (verificarDesplazamiento(img, mascara, datosTxt, tamMascara, semilla, nBits, izquierda)) {
        // Si la verificación es correcta, desplaza todos los bytes de la imagen
        for (int i = 0; i < totalBytes; ++i) {
            img[i] = izquierda ? img[i] << nBits : img[i] >> nBits;
        }
        // Imprimir en consola el tipo de desplazamiento realizado
        string dir = izquierda ? "derecha" : "izquierda";
        cout << orden << ") La operación aplicada fue un desplazamiento de " << nBits << " bits a la " << dir << "." << endl;
        return true; // Operación aplicada correctamente
    }
    return false; // No se aplicó la operación porque la verificación falló
}

// Función que intenta aplicar diferentes tipos de transformaciones sobre la imagen
// hasta encontrar la operación correcta que coincide con los datos del archivo txt
void aplicarTransformaciones(unsigned char* img, unsigned char* mascara, unsigned int* datosTxt, unsigned char* imgXor, int totalBytes, int tamMascara, int semilla, int orden) {
    // Iterar sobre los métodos posibles: 1 (XOR), 2 (Rotaciones), 3 (Desplazamientos)
    for (int metodo = 1; metodo <= 3; metodo++) {
        bool exito = false;
        switch (metodo) {
        case 1:
            // Intentar aplicar la operacion XOR
            exito = aplicarXOR(img, mascara, datosTxt, imgXor, totalBytes, tamMascara, semilla, orden);
            if (exito) return; // Si tiene éxito, salir de la función
            break;
        case 2:
            // Intentar aplicar rotaciones de 1 a 7 bits, primero a la izquierda y luego a la derecha
            for (int rot = 1; rot < 8; rot++) {
                if (aplicarRotacion(img, mascara, datosTxt, totalBytes, tamMascara, semilla, rot, true, orden)) return; // Rotación izquierda
                if (aplicarRotacion(img, mascara, datosTxt, totalBytes, tamMascara, semilla, rot, false, orden)) return; // Rotación derecha
            }
            break;
        case 3:
             // Intentar aplicar desplazamientos de 1 a 7 bits, primero a la izquierda y luego a la derecha
            for (int desp = 1; desp < 8; desp++) {
                if (aplicarDesplazamiento(img, mascara, datosTxt, totalBytes, tamMascara, semilla, desp, true, orden)) return; // Desplazamiento izquierda
                if (aplicarDesplazamiento(img, mascara, datosTxt, totalBytes, tamMascara, semilla, desp, false, orden)) return; // Desplazamiento derecha
            }
            break;
        }
    }
}
