#include <iostream>
#include "HelloWorld.h"
#include "RngTests.h"

int main() {
    while(true){
    std::cout << "Dies sind die Projekte zur Vorlesung Numerische Methoden der statistischen Physik von Chris Nill 2021"<<std::endl
    <<"Welches Projekt möchtst du starten? Wähle:" << std::endl
    <<"Programm beenden (0)"<<std::endl
    <<"Hello World (1)"<<std::endl
    <<"rngTests (2)"<<std::endl
    <<"Projektnummer: ";

    uint chooseProgram;
    std::cin>>chooseProgram;
        switch (chooseProgram) {
            case 0:
                return 0;
            case 1:
                runHelloWorld();
                break;
            case 2:
                runRngTests();
                break;
            default:
                break;
        }
        std::cout<<std::endl<<std::endl;
    }
}
