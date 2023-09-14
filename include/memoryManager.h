#ifndef MEM
#define MEM
#include <list>
#include <string>
#include <register.h>


/// @brief Klasa odpowiadająca za zapisywanie wartości wektorów dynamicznie deklarowanych. Działa na bazie słownika
class memoryManager {
private:
    /// @brief Lista kluczy (nazw zapisanych zmiennych)
    std::list<std::string> memoryDictKeys;
    /// @brief Lista wartości zapisanych zmiennych
    std::list<SSE_register> memoryDict;
public:
    memoryManager();
    ~memoryManager();
    /// @brief Dodaje nową zmienną do obu list
    /// @param key Nazwa nowej zmiennej
    /// @param entry Wartość nowej zmiennej
    void addEntry(std::string key, SSE_register entry);
    /// @brief Odnajduje daną zmienną
    /// @param key Nazwa szukanej zmiennej
    /// @return Ostatnia przypisana wartość zmiennej
    SSE_register find(std::string key);

};

#endif