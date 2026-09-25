#pragma once

#include <3ds.h>
#include <cstdio>

#ifndef NDEBUG
    #define ASSERT(condition, message)                                      \
        do {                                                                \
            if (!(condition)) {                                             \
                printf("\n");                                               \
                printf("========== ASSERT FAILED ==========\n");            \
                printf("Condition: %s\n", #condition);                      \
                printf("Message:   %s\n", message);                         \
                printf("File:      %s\n", __FILE__);                        \
                printf("Line:      %d\n", __LINE__);                        \
                printf("====================================\n");            \
                gfxFlushBuffers();                                          \
                gspWaitForVBlank();                                         \
                svcBreak(USERBREAK_PANIC);                                  \
            }                                                               \
        } while (0)
#else
    #define ASSERT(condition, message) ((void)0)
#endif