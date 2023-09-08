#include "Common.hpp"

template<>
std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat)
{
    os << "mat4[" << std::endl;
    char buf[128];
    for (int i=0; i < 4; i++)
    {
        std::sprintf(buf, "%.6f, %.6f, %.6f, %.6f", mat[0][i], mat[1][i], mat[2][i], mat[3][i]);
        os << "    " << buf << std::endl;
    }
    os << "]";
    return os;
}
