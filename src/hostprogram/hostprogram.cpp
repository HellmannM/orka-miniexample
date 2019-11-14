#include <cstdint>
#include <iostream>

extern "C" {
    #include "orka.h"
}

#include "accelerator.h"

int main()
{
    orka::fpga f;
    std::string json_filename   = JSON_NAME;
    std::string json_path       = JSON_DIR;
    std::string bit_path        = JSON_DIR;
    std::string orkagd_dir      = ORKAGD_INTERPRETER_DIR;
    f.json_filename = json_filename.c_str();
    f.json_path     = json_path.c_str();
    f.bit_path      = bit_path.c_str();
    f.orkagd_dir    = orkagd_dir.c_str();
    f.init();

    accelerator a(f);
    
    // set ports
    a.firstip.set_mem_offset(0x80010000);
    a.secondip.set_mem_offset(0x80020000);

    // write to fpga
    uint32_t blob1[4] = {1,2,3,4};
    uint32_t blob2[4] = {5,6,7,8};
    a.firstip_write_data((void*)blob1, sizeof(blob1), 0);
    a.secondip_write_data((void*)blob2, sizeof(blob2), 0);

    // start ips
    a.firstip.start();
    a.secondip.start();
    // wait until done
    a.firstip.wait();
    a.secondip.wait();

    // fetch results
    a.firstip_get_data((void*)blob1, sizeof(blob1), 0);
    a.secondip_get_data((void*)blob2, sizeof(blob2), 0);

    // print results
    std::cout << "blob1: ";
    for (int i=0; i<4; ++i)
        std::cout << blob1[i] << ", ";
    std::cout << std::endl;
    std::cout << "blob2: ";
    for (int i=0; i<4; ++i)
        std::cout << blob2[i] << ", ";
    std::cout << std::endl;

    f.deinit();
    return 0;
}
