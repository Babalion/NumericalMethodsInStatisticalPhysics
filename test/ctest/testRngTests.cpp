//
// Created by chris on 26.04.21.
//
#include "assert_macro.h"
#include "../../Projects/RngTests//RngTests.h"


int test_addvalues_zero() {
    int err_code = 0;
    assertEqual (0+0 == 0);
    assertEqual ((-5)+5 == 0);
    return err_code;
}

int isPiTestValid(){
    int err_code=0;
    assertEqual (piTest({0.234655, 0.51976},{0.156, 0.8436})==pi-2);//one pair lays in the circle
    return err_code;
}

int main() {

    return test_addvalues_zero();
}
