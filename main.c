#include "max72xx.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2