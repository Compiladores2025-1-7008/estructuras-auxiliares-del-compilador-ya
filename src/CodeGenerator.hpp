#pragma once
#include <string>

// Clase encargada de generar temporales y etiquetas para la construcción de código intermedio de tres direcciones
class CodeGenerator {
private:
    int nextTemp = 0; // Garantiza que el primer temporal es t0
    int nextLabel = 0; // Garantiza que el primer label sea L0

public:
    // Inicializa los contadores de temporales y etiquetas.
    CodeGenerator();           

    std::string newTemp(); // Devuelve t0, t1, t2...

    std::string newLabel() ; // Devuelve L0, L1, L2...

    void reset() ; // Reinicia ambos contadores
};
