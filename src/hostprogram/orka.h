#pragma once

#include <liborkagd.h>
#include <liborkagdint.h>
#include <types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <string>
#include <cstring>
#include <inttypes.h>

namespace orka
{

//typedef struct
//{
//    uint64_t start;
//    uint64_t size;
//} ORKA_DevMemEntry_t;
//
//ORKA_DevMemEntry_t g_ORKA_DevMemManagement[ 1 ] =
//{ 0 };
//
//void
//ORKA_DevMemInit(
//    uint64_t blockStartAddress,
//    uint64_t blockSize )
//{
//    printf("ORKA_DevMemInit: offs=0x%" PRIx64 ", range=0x%" PRIx64 "\n",
//        blockStartAddress,
//        blockSize );
//    g_ORKA_DevMemManagement[ 0 ].start = blockStartAddress;
//    g_ORKA_DevMemManagement[ 0 ].size = blockSize;
//}
//
//// hack to have a device-malloc. Assumption: We do nut run out of space and we do never free (or expect to really free)
//uint64_t
//ORKA_DevMalloc( uint64_t size )
//{
//    uint64_t res = 0ULL;
//    printf("ORKA_DevMalloc(0): size=0x%" PRIx64 "\n",
//        size );
//    printf("ORKA_DevMalloc(1): offs=0x%" PRIx64 ", range=0x%" PRIx64 "\n",
//        g_ORKA_DevMemManagement[ 0 ].start,
//        g_ORKA_DevMemManagement[ 0 ].size );
//    if ( g_ORKA_DevMemManagement[ 0 ].size >= size )
//    {
//        res = g_ORKA_DevMemManagement[ 0 ].start;
//        g_ORKA_DevMemManagement[ 0 ].start = g_ORKA_DevMemManagement[ 0 ].start + size;
//        g_ORKA_DevMemManagement[ 0 ].size  = g_ORKA_DevMemManagement[ 0 ].size - size;
//        printf("ORKA_DevMalloc(2): offs=0x%" PRIx64 ", range=0x%" PRIx64 "\n",
//            g_ORKA_DevMemManagement[ 0 ].start,
//            g_ORKA_DevMemManagement[ 0 ].size );
//        printf("ORKA_DevMalloc(3): res=0x%" PRIx64 "\n",
//            res );
//    }
//    return res;
//}

struct fpga
{
    ORKAGD_FPGAHandle_t* pTargetFPGA = nullptr;
    const char* json_filename;
    const char* json_path;
    const char* bit_path;
    const char* orkagd_dir;
    uint64_t numComponents;
    const ORKAGD_FPGAComponent_t** components = nullptr;

    void init()
    {
        ORKAGD_ConfigTarget_t targetConfig;
        ORKAGD_EC_t rc = ORKAGD_Init(orkagd_dir, json_path, json_path);
        if ( rc ) exit( 1 );

        targetConfig.m_InfrastructureFilename = (char*)json_filename;
        void *target = ORKAGD_BoardListOpen( &targetConfig );
        if ( !ORKAGD_BoardListRead( target )) exit( 2 );

        // Make sure there is only one FPGA in Host system
        if ( 1 != ORKAGD_BoardGetNumFPGAs( target )) exit( 3 );

        pTargetFPGA = (ORKAGD_FPGAHandle_t*) ORKAGD_FPGAHandleCreate( target, 0 );
        if ( !pTargetFPGA ) exit( 4 );
        ORKAGD_BoardListClose( target );

        ORKAGD_FPGAComponent_t *orkaIPHandle = nullptr;
        numComponents = ORKAGD_FPGAComponentsGetNumOf( pTargetFPGA );
        components = (const ORKAGD_FPGAComponent_t**) malloc(numComponents * sizeof(ORKAGD_FPGAComponent_t*));
        for ( uint64_t i = 0; i < numComponents; ++i )
        {
            components[i] = ORKAGD_FPGAComponentsGetEntry( pTargetFPGA, i );
            if ( !components[i] ) exit( 5 );
        }
        if ( !components[0] ) exit( 6 );

        if ( ORKAGD_FPGAOpen( pTargetFPGA )) exit( 7 );
    }

    int deinit()
    {
        ORKAGD_FPGAClose( pTargetFPGA );
        ORKAGD_FPGAHandleDestroy( pTargetFPGA );
        ORKAGD_Deinit();
        return 0;
    }

    void memcpy_d2h(void* dst, uint64_t src, uint64_t size)
    {
        ORKAGD_MemcpyD2H(pTargetFPGA, dst, src, size);
    }

    void memcpy_h2d(uint64_t dst, void* src, uint64_t size)
    {
        ORKAGD_MemcpyH2D(pTargetFPGA, dst, src, size);
    }
};

} // namespace orka
