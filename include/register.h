#ifndef REG
#define REG
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

/// @remark Turns out std::register already exists, have to use different name 
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