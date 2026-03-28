#include <iostream>
#include <windows.h>
#include "miniaudio.h"

std::string getAsset(std::string assetName) {
    return ASSET_DIR + std::string("/" + assetName);
}

int main() {
    std::cout << "Test: " << getAsset("Testings") << std::endl;
    return 0;
}
