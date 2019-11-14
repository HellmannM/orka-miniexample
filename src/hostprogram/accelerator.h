#pragma once

#include "orka.h"

#include <cstdint>
#include <iostream>
#include <string>

struct ip
{
    const ORKAGD_FPGAComponent_t* handle = nullptr;
    
    void start()
    {
        ORKAGD_Axi4LiteBlockStart(handle);
    }

    void wait()
    {
        ORKAGD_Axi4LiteBlockWait(handle);
    }
};

struct mmip : ip
{
    uint64_t mem_offset;
    uint32_t mem_addr_offset;

    void set_mem_offset(uint64_t offset)
    {
        mem_offset = offset;
        ORKAGD_RegisterU32Write(handle, mem_addr_offset, offset);
    }
};

struct accelerator
{
    orka::fpga& _f;
    mmip firstip;
    mmip secondip;

    accelerator(orka::fpga& f)
    : _f(f)
    {
        for (int i = 0; i < f.numComponents; ++i)
        {
            if (f.components[i])
            {
                if (f.components[i]->ipType)
                {
                    if (! strcmp("orkaip", f.components[i]->ipType))
                    {
                        if ( 0 == strcmp("register", f.components[i]->ipAccess))
                        {
                            //check for name and set handle for corresponding ip
                            if (std::string(f.components[i]->ipDesignComponentName).find("firstip") != std::string::npos)
                            {
                                firstip.handle = f.components[i];
                            }
                            else if (std::string(f.components[i]->ipDesignComponentName).find("secondip") != std::string::npos)
                            {
                                secondip.handle = f.components[i];
                            }
                        }
                    }
                }
            }
        }
        //TODO
        firstip.mem_addr_offset = 0x10;
        secondip.mem_addr_offset = 0x10;
    }

    // size in bytes
    void firstip_write_data(void* src, uint64_t size, uint64_t offset)
    {
        _f.memcpy_h2d(firstip.mem_offset + offset, src, size);
    }

    void firstip_get_data(void* dst, uint64_t size, uint64_t offset)
    {
        _f.memcpy_d2h(dst, firstip.mem_offset + offset, size);
    }

    // size in bytes
    void secondip_write_data(void* src, uint64_t size, uint64_t offset)
    {
        _f.memcpy_h2d(secondip.mem_offset + offset, src, size);
    }

    void secondip_get_data(void* dst, uint64_t size, uint64_t offset)
    {
        _f.memcpy_d2h(dst, secondip.mem_offset + offset, size);
    }
};

