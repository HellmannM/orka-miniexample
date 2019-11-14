#include <cassert>
#include <cstdint>
#include <hls_stream.h>

extern void firstip(hls::stream<uint32_t>& in1, hls::stream<uint32_t>& in2);

int main()
{
    hls::stream<uint32_t> s1;
    hls::stream<uint32_t> s2;
    uint32_t v1 = 1;
    uint32_t v2 = 23;
    s1.write(v1);
    s2.write(v2);
    firstip(s1, s2);
    return 0;
}
