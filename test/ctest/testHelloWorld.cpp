//
// Created by chris on 26.04.21.
//

#include "assert_macro.h"
#include "../../Projects/HelloWorld/HelloWorld.h"

int test_addvalues_negpos() {
    int err_code = 0;
    assertEqual ((-5)+10 == 5);
    assertEqual ((-10)+5 == -5);
    //assertEqual (5+(10) == 5); //test to fail
    return err_code;
}

int main() {
    int err_code=0;
    assertEqual (runHelloWorld()==0);
    return err_code;
}