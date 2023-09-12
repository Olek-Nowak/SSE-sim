#include "register.h"
#include "windowManager.h"
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

windowManager* wm;
std::vector<SSE_register> xmm;
std::vector<SSE_register> backup;
fstream instructions;

// Main przechowuje rejestry, odczytuje z pliku, wykonuje instrukcje oraz wywołuje windowManager
// WindowManager rysuje okno oraz pobiera inputy użytkownika
// Rejestry przechowują wartości i wykonują podstawowe operacje / konwersje

void setup() {
    wm = new windowManager();
    instructions.open("../resource/main.asm");
    xmm.resize(8);
    backup.resize(8);
    backup = xmm;

}

void redraw() {
    wm->clear();
    wm->addHeaders();
    for(int i = 0; i < xmm.size(); i++)
        wm->updateRegister(i, backup[i], xmm[i]);
    wm->show();


}

void handleInput() {
    sf::Event e = wm->pollEvents();
    switch(e.type)
    {
    case sf::Event::Closed:
        wm->exit();
        break;
    case sf::Event::KeyPressed:
        switch (e.key.code)
        {
        case sf::Keyboard::Left:
            // previous step
            break;
        case sf::Keyboard::Right:
            // next step
            break;
        case sf::Keyboard::Space:
            // debug
            redraw();
            break;
        }
        break;
    }

}

int main() {
    setup();
    redraw();
    xmm[0].setBits(0, "00000000000000000000000000000010");
    while(wm->getEnabled()) {
        handleInput();

    }

}