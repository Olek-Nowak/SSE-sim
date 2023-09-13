#ifndef REG
#define REG
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

/// @remark Klasa std::register już istnieje, wymagana inna nazwa
class SSE_register {
private:
    std::vector<bool> binary;
    std::vector<float> data;
public:
    SSE_register();
    ~SSE_register();
    std::string getBits(int start, int amount);
    void setBits(int start, std::string bits);
    void setFloats(int start, float newData);
    float operator()(int num);

};

#endif