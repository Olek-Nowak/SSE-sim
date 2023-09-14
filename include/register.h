#ifndef REG
#define REG
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>

/// @brief Klasa reprezentująca pojedynczy, 128-bitowy rejestr SSE
/// @remark Klasa std::register już istnieje, wymagana inna nazwa
class SSE_register {
private:
    /// @brief Wektor przechowujący 128 bitów rejestru w postaci 128 zmiennych bool
    std::vector<bool> binary;
    /// @brief Wektor przechowujący 128 bitów rejestru w postaci 4 zmiennych single-precision floating point
    std::vector<float> data;
public:
    SSE_register();
    ~SSE_register();
    /// @brief Getter pojedynczych bitów rejestru
    /// @param start Początek sekwencji / numer pierwszego bitu do pobrania (0-127)
    /// @param amount Liczba bitów do pobrania (0-127)
    /// @return Bity w postaci ciągu zer i jedynek
    std::string getBits(int start, int amount);
    /// @brief Setter pojedynczych bitów rejestru
    /// @param start Początek sekwencji / numer pierwszego bitu do zmiany (0-127)
    /// @param bits Bity w postaci ciągu zer i jedynek
    void setBits(int start, std::string bits);
    /// @brief Setter 32 bitów rejestru przy pomocy jednej zmiennej single-precision floating point
    /// @param start Początek sekwencji (0-3)
    /// @param newData Zmienna single-precision floating point do zmiany
    void setFloats(int start, float newData);
    /// @brief Overload operatora funkcji, służący za getter 32 bitów rejestru
    /// @param num Numer sekwencji do pobrania (0-3)
    /// @return Zmienna single-precision floating point
    float operator()(int num);

};

#endif