#include "register.h"
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

sf::RenderWindow* mainWindow;
sf::Font font;
std::vector<SSE_register> xmm;
std::vector<SSE_register> backup;
fstream instructions;

// Main holds all registers, draws to screen, polls inputs and updates
// Registers hold all values and perform basic operations

void setup() {
    mainWindow = new sf::RenderWindow(sf::VideoMode(900, 600), "SEE-sim");
    mainWindow->setKeyRepeatEnabled(false);
    mainWindow->setFramerateLimit(60);
    mainWindow->setVerticalSyncEnabled(1);
    mainWindow->clear();
    font.loadFromFile("../resource/arial.ttf");
    instructions.open("../resource/main.asm");
    xmm.resize(8);
    backup.resize(8);
    backup = xmm;

}

void redraw() {
    for(int i = 0; i < xmm.size(); i++) {
        sf::Text name;
        name.setFont(font);
        string t = "XMM ";
        t += char(i + 48);
        name.setString(t);
        name.setCharacterSize(20);
        name.setFillColor(sf::Color::White);
        name.setPosition(65 + (99 * i), 90);
        mainWindow->draw(name);
        for(int j = 0; j < 4; j++) {
            sf::Text temp;
            temp.setFont(font);
            string t = to_string(xmm[i](j));
            temp.setString(t);
            temp.setCharacterSize(20);
            temp.setFillColor(sf::Color::White);
            temp.setPosition(50 + (100 * i), 200 + (100 * j));
            mainWindow->draw(temp);

        }

    }
    mainWindow->display();


}

void update() {
    mainWindow->clear();
    sf::Event event;
    while(mainWindow->pollEvent(event)) {
        switch (event.type)
        {
        case sf::Event::Closed:
            mainWindow->close();
            break;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                // previous step
                break;
            case sf::Keyboard::Right:
                // next step
                break;
            case sf::Keyboard::Space:
                // debug
                break;
            }
            break;

        }

    }
    
    redraw();


}

int main() {
    setup();
    xmm[0].setBits(0, "00000000000000000000000000000010");
    while(mainWindow->isOpen())
        update();
}