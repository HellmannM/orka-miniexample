#include <cstdint>
#include <iostream>
#include <hls_stream.h>

void firstip(hls::stream<uint32_t>& in1, hls::stream<uint32_t>& in2)
{
#pragma HLS INTERFACE s_axilite     port=reutrn
#pragma HLS INTERFACE axis port=in1 bundle=in1
#pragma HLS INTERFACE axis port=in2 bundle=in2
    while(in1.empty()){}
    uint32_t a = in1.read();
    while(in2.empty()){}
    uint32_t b = in2.read();
    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
}
