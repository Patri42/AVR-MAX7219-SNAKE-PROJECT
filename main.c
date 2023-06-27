#include "max72xx.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

// Hej

//// https://wokwi.com/projects/296234816685212169


int main()
{
	init_serial();
	max7219_init();
	int x = 0;
	int y = 0;
	while (1) {
		for(int i = 0; i < 16;i++){
			printf("%d\n", i);
			max7219b_set(i, y);
			max7219b_out();
				_delay_ms(1000);
		}
	}
	return 0;
}