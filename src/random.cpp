#include <algorithm>
#include <random>
#include <string>

using namespace std;

string randomAlphaNumericString(int length)
{
    constexpr string_view characters = "0123456789abcdef";
    random_device randomDevice;
    mt19937 rg(randomDevice());
    uniform_int_distribution<int> distribution(0, characters.length() - 1);
    string string(length, '-');
    generate(begin(string), end(string),
             [&]() { return characters[distribution(rg)]; });
    return string;
}
