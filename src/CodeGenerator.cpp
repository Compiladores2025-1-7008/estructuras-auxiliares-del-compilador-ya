#include "CodeGenerator.hpp"
#include <string>

// Se inicializan los contadores
CodeGenerator::CodeGenerator()
    : nextTemp(0), nextLabel(0) {}

// Genera temporales
std::string CodeGenerator::newTemp() {
    return "t" + std::to_string(nextTemp++);
}

// Genera etiquetas
std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(nextLabel++);
}

// Reinicia ambos contadores
void CodeGenerator::reset() {
    nextTemp = 0;
    nextLabel = 0;
}
