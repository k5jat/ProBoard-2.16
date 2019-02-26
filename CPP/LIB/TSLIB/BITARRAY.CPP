#ifdef PEX
  #include <pb_sdk.h>
#endif

#include <tslib.hpp>

BitArray::BitArray(long size,long s)
{
    array = new word[unsigned(size/16) + 1];

    for(int i = 0 ; i < unsigned(size/16) + 1 ; i++) array[i] = 0;

    start = s;
}

BitArray::~BitArray()
{
   delete [] array;
}

bool
BitArray::operator[](long i) const
{
    return !!(array[unsigned((i-start)/16)] & (1 << ((i-start) & 15)));
}

void
BitArray::set(long i)
{
    array[unsigned((i-start)/16)] |= (1 << ((i-start) & 15));
}

void
BitArray::clear(long i)
{
    array[unsigned((i-start)/16)] &= ~(1 << ((i-start) & 15));
}

