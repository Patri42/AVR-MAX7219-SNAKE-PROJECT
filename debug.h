#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 0 // TURN ON (1) OR OFF (0) DEBUG MESSAGES

#if DEBUG
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif


#endif 