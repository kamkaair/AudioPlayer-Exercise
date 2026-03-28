#include <iostream>
#include <windows.h>
#include "miniaudio.h"

std::string getAsset(std::string assetName) {
    return ASSET_DIR + std::string("/" + assetName);
}

void isKeyPressed(int key, bool& buttonGate) {
    if (GetAsyncKeyState(key) < 0 && buttonGate == false)
    {
        std::cout << "ALT key is down." << std::endl;
        buttonGate = true;
    }
    if (GetAsyncKeyState(key) == 0 && buttonGate == true)
    {
        std::cout << "ALT key is RELEASED!" << std::endl;
        buttonGate = false;
    }
}

int main() {
    std::cout << "Test: " << getAsset("Testings") << std::endl;

    bool exitable = false, buttonGate = false;

    while (!exitable) {
        /*if (isKeyPressed(VK_SHIFT, gate)) {
            std::cout << "ALT key is down." << std::endl;
        }
        else {
            std::cout << "ALT key is RELEASED!" << std::endl;
        }*/

        isKeyPressed(VK_SHIFT, buttonGate);

        /*if (GetAsyncKeyState(VK_SHIFT) < 0 && gate == false)
        {
            std::cout << "ALT key is down." << std::endl;
            gate = true;
        }
        if (GetAsyncKeyState(VK_SHIFT) == 0 && gate == true)
        {
            std::cout << "ALT key is RELEASED!" << std::endl;
            gate = false;
        }*/

    }
    return 0;
}
