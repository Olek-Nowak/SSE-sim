#include "register.h"

SSE_register::SSE_register() {
    data.resize(128);
    for(int i = 0; i < 128; i++) {
        data[i] = 0;

    }

}

SSE_register::~SSE_register() {

}

std::string SSE_register::getBits(int start, int amount) {
    if(start + amount > 128)
        return "";
    std::string out = "";
    for(int i = start; i < start + amount; i++) {
        out += std::to_string(data[i]);

    }
    return out;

}

void SSE_register::setBits(int start, std::string bits) {
    if(start + bits.length() > 128)
        return;
    for(int i = start; i < start + bits.length(); i++) {
        data[i] = int(bits[i - start]) - 48;
        if(data[i] > 1 || data[i] < 0)
            return;

    }

}

float SSE_register::operator()(int num) {
    // Nietypowa wartość dla debugowania
    if(num > 3)
        return -1.9f;
    std::string temp = "";
    temp = getBits(num*32,32);
    int n = 0;
    for(int i = 0; i < temp.length(); ++i) {
        n |= (temp[i] - 48) << i;

    }
    float f = *(float *)&n;
    return f;
}