#include <hard.h>
#include <ssd1306.h>
#include <ssd1306_font.h>
#include <max30100.h>
#include <stdbool.h>

bool start = false;

void app_init(void)
{
	hard_init();
	start = true;
}

void app_loop(void)
{
	hard_cpu_sleep();
}
