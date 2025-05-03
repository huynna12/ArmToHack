#include "ArmToHack.h"
#include "token_io.h"
using namespace std;

// clang++ -std=c++14 -arch arm64 -g main.cpp ArmToHack.cpp token_io.cpp -o ArmToHack 
// ./ArmToHack
int main() {
    ArmToHack translator;

    translator.translate("test/program1.arm", "test/program1.hack");
    translator.translate("test/program2.arm", "test/program2.hack");
    translator.translate("test/program3.arm", "test/program3.hack");

    translator.translate("test/program4.arm", "test/program4.hack");
    translator.translate("test/program5.arm", "test/program5.hack");
    translator.translate("test/program6.arm", "test/program6.hack");
    
    translator.translate("test/program7.arm", "test/program7.hack");
    translator.translate("test/program8.arm", "test/program8.hack");
    translator.translate("test/program9.arm", "test/program9.hack");

    translator.translate("test/program10.arm", "test/program10.hack");
    return 0; 
}