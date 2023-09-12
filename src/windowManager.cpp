#include "windowManager.h"
#include "string"
using namespace std;

// TODO: responsive window resizing

windowManager::windowManager() {
    mainWindow = new sf::RenderWindow(sf::VideoMode(900, 600), "SEE-sim");
    mainWindow->setKeyRepeatEnabled(false);
    mainWindow->setFramerateLimit(60);
    mainWindow->setVerticalSyncEnabled(1);
    mainWindow->clear();
    font.loadFromFile("../resource/arial.ttf");
    enabled = true;

}

windowManager::~windowManager() {
    enabled = false;
    mainWindow->close();

}

bool windowManager::getEnabled() {
    return enabled;

}

sf::Event windowManager::pollEvents() {
    sf::Event newEvent;
    mainWindow->waitEvent(newEvent);
    return newEvent;

}

void windowManager::clear() {
    mainWindow->clear();

}

void windowManager::addHeaders() {
    for(int i = 0; i < 8; i++) {
        sf::Text name;
        name.setFont(font);
        string t = "XMM ";
        t += char(i + 48);
        name.setString(t);
        name.setCharacterSize(20);
        name.setFillColor(sf::Color::White);
        name.setPosition(65 + (99 * i), 90);
        mainWindow->draw(name);
    
    }

}

void windowManager::updateRegister(int number, SSE_register prev, SSE_register next) {
    if(number > 7)
        return;
    for(int j = 0; j < 4; j++) {
        sf::Text temp;
        string t;
        temp.setFont(font);
        temp.setCharacterSize(20);
        temp.setFillColor(sf::Color::White);
        // Jeżeli nastepiła zmiana, koloruj na zielono
        if(next(j) != prev(j))
            temp.setFillColor(sf::Color::Green);
        t = to_string(next(j));
        temp.setString(t);
        temp.setPosition(50 + (100 * number), 200 + (100 * j));
        mainWindow->draw(temp);

    }

}

void windowManager::exit() {
    enabled = false;
    mainWindow->close();

}

void windowManager::show() {
    mainWindow->display();

}