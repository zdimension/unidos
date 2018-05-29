#ifndef UNIDOS_PSP_H
#define UNIDOS_PSP_H

#include <stdint.h>

#pragma pack(push, 1)
struct PSP
{
    uint8_t CPMExit[2];    // 0
    uint16_t FirstFreeSegment;  // 2
    uint8_t Reserved1; // 4
    uint8_t CPMCall5Compat[5]; // 5
    uint32_t OldTSRAddress; // 12
    uint32_t OldBreakAddress;
    uint32_t CriticalErrorHandlerAddress;
    uint16_t CallerPSPSegment;
    uint8_t JobFileTable[20];
    uint16_t EnvironmentSegment;
    uint32_t INT21SSSP;
    uint16_t JobFileTableSize;
    uint32_t JobFileTablePointer;
    uint32_t PreviousPSP;
    uint32_t Reserved2;
    uint16_t DOSVersion;
    uint8_t Reserved3[14];
    uint8_t DOSFarCall[3];
    uint16_t Reserved4;
    uint8_t ExtendedFCB1[7];
    uint8_t FCB1[16];
    uint8_t FCB2[20];
    uint8_t CommandLineLength;
    char CommandLine[127];
};
#pragma pack(pop)

void psp_setup(int16_t seg, uint8_t* fcontent, int argc, char** argv);

uint8_t psp_create(int16_t seg);

uint8_t psp_copy(uint16_t source, uint16_t target);

#endif