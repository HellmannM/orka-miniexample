#include <cstdint>

void secondip(volatile uint32_t* masterport)
{
#pragma HLS INTERFACE s_axilite     port=reutrn
#pragma HLS INTERFACE m_axi offset=slave depth=16 port=masterport
    for(int i=0; i<4; ++i)
        ++masterport[i];
}
