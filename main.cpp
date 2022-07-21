#include "Words.h"

int main()
{
    Words words({{"spears"}, {"planet"}, {"easily"}, {"animal"}, {"relate"}, {"styles"}});
    for(const auto& iter : words.getMaxRectangle())
        std::cout << iter << '\n';
    return 0;
}
