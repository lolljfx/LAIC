#include <fstream>
#include "preprocessor.h"

int main(int argc, char** argv) {
    preprocessCode("main.cpp", "processed_main.cpp");
    return 0;
}
