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
bool browseFlag;

// Main przechowuje rejestry, odczytuje z pliku, wykonuje instrukcje oraz wywołuje windowManager
// WindowManager rysuje okno oraz pobiera inputy użytkownika
// MemoryManager przechowuje wartości odkładane na stosie
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
    browseFlag = 0;

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
    // Deklaracja zmiennej
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
    // Ładowanie wektorowe
    // Wyrónianie obszaru pamięci do 16 nie jest sprawdzane - movaps równoważne movups
    else if(parsed[0] == "movaps" || parsed[0] == "movups" || parsed[0] == "MOVAPS" || parsed[0] == "MOVUPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48] = xmm[numb - 48];

            }
            else {
                xmm[numa - 48] = mm->find(parsed[2] + numb);

            }

        }

    }
    // Ładowanie skalarne
    else if(parsed[0] == "movss" || parsed[0] == "MOVSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, xmm[numb - 48](0));

            }
            else {
                xmm[numa - 48].setFloats(0, mm->find(parsed[2] + numb)(0));

            }

        }

    }
    // Dodawanie wektorowe
    else if(parsed[0] == "addps" || parsed[0] == "ADDPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) + xmm[numb - 48](i));

            }
            else {
                for (int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) + mm->find(parsed[2] + numb)(i));

            }

        }

    }
    // Dodawanie skalarne
    else if(parsed[0] == "addss" || parsed[0] == "ADDSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) + xmm[numb - 48](0));

            }
            else {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) + mm->find(parsed[2] + numb)(0));

            }

        }

    }
    // Odejmowanie wektorowe
    else if(parsed[0] == "subps" || parsed[0] == "SUBPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) - xmm[numb - 48](i));

            }
            else {
                for (int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) - mm->find(parsed[2] + numb)(i));

            }

        }

    }
    // Odejmowanie skalarne
    else if(parsed[0] == "subss" || parsed[0] == "SUBSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) - xmm[numb - 48](0));

            }
            else {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) - mm->find(parsed[2] + numb)(0));

            }

        }

    }
    // Mnożenie wektorowe
    else if(parsed[0] == "mulps" || parsed[0] == "MULPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) * xmm[numb - 48](i));

            }
            else {
                for (int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) * mm->find(parsed[2] + numb)(i));

            }

        }

    }
    // Mnożenie skalarne
    else if(parsed[0] == "mulss" || parsed[0] == "MULSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) * xmm[numb - 48](0));

            }
            else {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) * mm->find(parsed[2] + numb)(0));

            }

        }

    }
    // Dzielenie wektorowe
    else if(parsed[0] == "divps" || parsed[0] == "DIVPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) / xmm[numb - 48](i));

            }
            else {
                for (int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, xmm[numa - 48](i) / mm->find(parsed[2] + numb)(i));

            }

        }

    }
    // Dzielenie skalarne
    else if(parsed[0] == "divss" || parsed[0] == "DIVSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) / xmm[numb - 48](0));

            }
            else {
                xmm[numa - 48].setFloats(0, xmm[numa - 48](0) / mm->find(parsed[2] + numb)(0));

            }

        }

    }
    // Pierwiastek kwadratowy wektorowy
    else if(parsed[0] == "sqrtps" || parsed[0] == "SQRTPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, sqrt(xmm[numb - 48](i)));

            }
            else {
                for (int i = 0; i < 4; i++)
                    xmm[numa - 48].setFloats(i, sqrt(mm->find(parsed[2] + numb)(i)));

            }

        }

    }
    // Pierwiastek kwadratowy skalarny
    else if(parsed[0] == "sqrtss" || parsed[0] == "SQRTSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem może być rejestr lub zmienna
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                xmm[numa - 48].setFloats(0, sqrt(xmm[numb - 48](0)));

            }
            else {
                xmm[numa - 48].setFloats(0, sqrt(mm->find(parsed[2] + numb)(0)));

            }

        }

    }
    // Porównianie == wektorowe
    else if(parsed[0] == "cmpeqps" || parsed[0] == "CMPEQPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++) {
                    if(xmm[numa - 48](i) == xmm[numb - 48](i))
                        xmm[numa - 48].setBits(i*32, "11111111111111111111111111111111");
                    else xmm[numa - 48].setBits(i*32, "00000000000000000000000000000000");

                }

            }

        }

    }
    // Porównianie == skalarne
    else if(parsed[0] == "cmpeqss" || parsed[0] == "CMPEQSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                if(xmm[numa - 48](0) == xmm[numb - 48](0))
                    xmm[numa - 48].setBits(0, "11111111111111111111111111111111");
                else xmm[numa - 48].setBits(0, "00000000000000000000000000000000");

            }

        }

    }
    // Porównianie < wektorowe
    else if(parsed[0] == "cmpltps" || parsed[0] == "CMPLTPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++) {
                    if(xmm[numa - 48](i) < xmm[numb - 48](i))
                        xmm[numa - 48].setBits(i*32, "11111111111111111111111111111111");
                    else xmm[numa - 48].setBits(i*32, "00000000000000000000000000000000");

                }

            }

        }

    }
    // Porównianie < skalarne
    else if(parsed[0] == "cmpltss" || parsed[0] == "CMPLTSS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                if(xmm[numa - 48](0) < xmm[numb - 48](0))
                    xmm[numa - 48].setBits(0, "11111111111111111111111111111111");
                else xmm[numa - 48].setBits(0, "00000000000000000000000000000000");

            }

        }

    }
    // Porównianie !<= wektorowe
    else if(parsed[0] == "cmpnleps" || parsed[0] == "CMPNLEPS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                for(int i = 0; i < 4; i++) {
                    if(xmm[numa - 48](i) > xmm[numb - 48](i))
                        xmm[numa - 48].setBits(i*32, "11111111111111111111111111111111");
                    else xmm[numa - 48].setBits(i*32, "00000000000000000000000000000000");

                }

            }

        }

    }
    // Porównianie !<= skalarne
    else if(parsed[0] == "cmpnless" || parsed[0] == "CMPNLESS") {
        parsed[1].pop_back();
        char numa = parsed[1].back();
        parsed[1].pop_back();
        // Celem musi być rejestr
        if(parsed[1] == "xmm" || parsed[1] == "XMM") {
            char numb = parsed[2].back();
            parsed[2].pop_back();
            // Żródłem musi być rejestr
            if(parsed[2] == "xmm" || parsed[2] == "XMM") {
                if(xmm[numa - 48](0) > xmm[numb - 48](0))
                    xmm[numa - 48].setBits(0, "11111111111111111111111111111111");
                else xmm[numa - 48].setBits(0, "00000000000000000000000000000000");

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
            browseFlag = 1;
            break;
        case sf::Keyboard::Right:
            // next step
            if(browseFlag == 0) {
                parseInstruction();

            }
            wm->clear();
            wm->addHeaders();
            wm->updateInstruction(nextInstruction);
            for (int i = 0; i < xmm.size(); i++)
                wm->updateRegister(i, backup[i], xmm[i]);
            wm->show();
            browseFlag = 0;
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