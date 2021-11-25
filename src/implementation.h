#ifndef IMPL_H
#define IMPL_H

#include "vector.h"

class implementation
{
public:
    void add_to_vec(int x);

private:
    int m_x;
    bud::vector<int> m_vec;      
};

#endif // IMPL_H
