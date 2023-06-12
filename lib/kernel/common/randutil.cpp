#include "kernel/common/randutil.h"
#include <random>

namespace yrui {
namespace common {
    namespace {
        static std::default_random_engine en {
#ifdef NDEBUG
            std::random_device {}()
#endif // when debug, we expected reoccurred process
        };
    }

    int randint(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(en);
    }
}
}
