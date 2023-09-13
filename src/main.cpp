#include "register.h"
#include "windowManager.h"
#include "memoryManager.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <SFML/Graphics.hpp>
using namespace std;

windowManager* wm;
memoryManager* mm;
std::vector<SSE_register> xmm;
std::vector<SSE_register> backup;
fstream instructions;
string prevInstruction;
string nextInstruction;

// Main przechowuje rejestry, odczytuje z pliku, wykonuje instrukcje oraz wywołuje windowManager
// WindowManager rysuje okno oraz pobiera inputy użytkownika
// Rejestry przechowują wartości i wykonują podstawowe operacje / konwersje

void setup() {
    wm = new windowManager();
    mm = new memoryManager();
    instructions.open("../resource/main.asm");
    xmm.resize(8);
    backup.resize(8);
    backup = xmm;
    prevInstruction = "";
    nextInstruction = "";

}

std::vector<std::string> splitString(const std::string &s, char delimiter) {
    // Niestety splitString wymaga pełnej dokładności (brak nadmiarowych spacji, przesuniętych przecinków)
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;
    while(getline(ss, item, delimiter)) {
        result.push_back (item);

    }

    return result;
}

void parseInstruction() {
    if(instructions.eof())
        return;
    prevInstruction = nextInstruction;
    backup = xmm;
    getline(instructions, nextInstruction);
    std::vector<std::string> parsed = splitString(nextInstruction, ' ');
    // Rozpatrz każdą obsługiwaną instrukcję
    if(parsed[0] == "_declspec(align(16))" && parsed[1] == "float" && parsed[3] == "=") {
        parsed[4].erase(parsed[4].begin());
        SSE_register newreg = SSE_register();
        for(int i = 0; i < 4; i++) {
            parsed[4 + i].pop_back();
            newreg.setFloats(i, stof(parsed[4 + i]));

        }
        parsed[2].pop_back();
        parsed[2].pop_back();
        mm->addEntry(parsed[2], newreg);
        //nextInstruction = parsed[4] + "/" + parsed[5] + "/" + parsed[6] + "/" + parsed[7];

    }
    else if(parsed[0] == "movaps") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            if(parsed[2] == "xmm") {
                xmm[numa - 48] = xmm[numb - 48];

            }
            else {
                xmm[numa - 48] = mm->find(parsed[2] + numb);

            }

        }

    }

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
            wm->clear();
            wm->addHeaders();
            wm->updateInstruction(prevInstruction);
            for (int i = 0; i < backup.size(); i++)
                wm->updateRegister(i, backup[i], backup[i]);
            wm->show();
            break;
        case sf::Keyboard::Right:
            // next step
            parseInstruction();
            wm->clear();
            wm->addHeaders();
            wm->updateInstruction(nextInstruction);
            for (int i = 0; i < xmm.size(); i++)
                wm->updateRegister(i, backup[i], xmm[i]);
            wm->show();
            break;
        case sf::Keyboard::Space:
            // debug
            break;
        }
        break;
    }

}

int main() {
    setup();
    wm->clear();
    wm->addHeaders();
    wm->updateInstruction(nextInstruction);
    for (int i = 0; i < xmm.size(); i++)
        wm->updateRegister(i, backup[i], xmm[i]);
    wm->show();
    // główna pętla
    while(wm->getEnabled()) {
        handleInput();

    }

}