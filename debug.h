#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 1 // TURN ON OR OFF DEBUG MESSAGES

#if DEBUG
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif

#endif 