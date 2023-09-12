#ifndef WINDOW
#define WINDOW
#include <SFML/Graphics.hpp>
#include "register.h"

/// @brief Klasa zarządzająca wyświetlaniem gui oraz pobieraniem stanu wejść
class windowManager {
private:
    /// @brief Flaga oznaczająca włączone okno
    bool enabled = true;
    /// @brief Główne okno (część biblioteki SFML)
    sf::RenderWindow* mainWindow;
    /// @brief Czcionka (część biblioteki SFML)
    sf::Font font;
public:
    windowManager();
    ~windowManager();
    /// @brief Getter stanu włączenia okna
    bool getEnabled();
    /// @brief Metoda pobierająca wydarzenia inputu użytkownika z kolejki (część biblioteki SFML)
    /// @return Najnowszy input
    sf::Event pollEvents();
    /// @brief Czyści okno
    void clear();
    /// @brief Rysuje elementy stałe / nagłówki
    void addHeaders();
    /// @brief Uaktualnia wyświetlany stan danego rejestru
    /// @param number Numer rejestru do uaktualnienia (0-7)
    /// @param prev Poprzedni stan rejestru
    /// @param next Nowy stan rejestru
    void updateRegister(int number, SSE_register prev, SSE_register next);
    /// @brief 
    void updateInstruction();
    /// @brief Zamyka główne okno
    void exit();
    /// @brief 
    void show();
    
};

#endif