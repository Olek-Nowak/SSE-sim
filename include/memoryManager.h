#ifndef MEM
#define MEM
#include <list>
#include <string>
#include <register.h>


/// @brief Klasa odpowiadająca za zapisywanie wartości wektorów dynamicznie deklarowanych. Działa na bazie słownika
class memoryManager {
private:
    std::list<std::string> memoryDictKeys;
    std::list<SSE_register> memoryDict;
public:
    memoryManager();
    ~memoryManager();
    void addEntry(std::string key, SSE_register entry);
    SSE_register find(std::string key);

};

#endif