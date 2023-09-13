#include "memoryManager.h"
using namespace std;

memoryManager::memoryManager() {


}

memoryManager::~memoryManager() {
    
}

void memoryManager::addEntry(std::string key, SSE_register entry) {
    // Póki co brak sprawdzania unikalności kluczy - zwracana będzie najnowsza deklaracja
    memoryDictKeys.push_back(key);
    memoryDict.push_back(entry);
    
}

SSE_register memoryManager::find(std::string key) {
    list<std::string>::reverse_iterator itKey = memoryDictKeys.rbegin();
    list<SSE_register>::reverse_iterator itEntry = memoryDict.rbegin();
    while(itKey != memoryDictKeys.rend()) {
        if(itKey->compare(key) == 0)
            return *(itEntry);
        itKey++;
        itKey++;

    }

}