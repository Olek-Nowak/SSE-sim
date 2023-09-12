#ifndef REG
#define REG
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

/// @remark Klasa std::register już istnieje, wymagana inna nazwa
class SSE_register {
private:
    std::vector<bool> data;
public:
    SSE_register();
    ~SSE_register();
    std::string getBits(int start, int amount);
    void setBits(int start, std::string bits);
    float operator()(int num);

};

#endif