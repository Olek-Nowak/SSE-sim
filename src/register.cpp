#include "register.h"

SSE_register::SSE_register() {
    binary.resize(128);
    data.resize(4);
    for(int i = 0; i < 128; i++) {
        binary[i] = 0;

    }
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 0.0f;

}

SSE_register::~SSE_register() {

}

std::string SSE_register::getBits(int start, int amount) {
    if(start + amount > 128)
        return "";
    std::string out = "";
    for(int i = start; i < start + amount; i++) {
        out += std::to_string(binary[i]);

    }
    return out;

}

void SSE_register::setBits(int start, std::string bits) {
    if(start + bits.length() > 128)
        return;
    for(int i = start; i < start + bits.length(); i++) {
        binary[i] = int(bits[i - start]) - 48;
        if(binary[i] > 1 || binary[i] < 0)
            return;

    }
    //Uaktualnij wszystkie floaty
    for(int i = 0; i < 4; i++) {
        std::string temp = "";
        temp = getBits(i*32,32);
        int n = 0;
        for(int j = 0; j < temp.length(); ++j) {
            n |= (temp[j] - 48) << j;

        }
        float f = *(float *)&n;
        data[i] = f;

    }

}

void SSE_register::setFloats(int start, float newData) {
    // Po aktualizacji floatów nie aktualizujemy bitów - bardzo skomplikowany algorytm, a w 99% przypadków używamy tylko float
    if(start > 3)
        return;
    data[start] = newData;

}

float SSE_register::operator()(int num) {
    // Nietypowa wartość dla debugowania
    if(num > 3)
        return -1.9f;
    return data[num];

}