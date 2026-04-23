#include <lynx.h>
#include <stdlib.h>
#include <time.h>

#include "../misc.h"
#include "../fujinet-fuji.h"
#include "../platform-specific/graphics.h"

#include "network.h"

uint32_t _start_time;

/* gamelogic.c grabs the "time" in jiffies, and divides it by clocks_per_sec to
 * compute seconds elapsed. We will record the start time each time resetTimer
 * is called, and then subtract that from now, sending that back as an int
 * gamelogic will then divide that by CLOCKS_PER_SEC to get seconds elapsed
 */

void resetTimer()
{
	_start_time = clock();
}

uint16_t getTime()
{
	uint32_t now;

    now = clock();
	return ((uint16_t) (now - _start_time));
}

void quit()
{
    resetScreen(false);
    resetGraphics();
    while(1);				// maybe we want to reboot the lynx? -SJ
    //exit(0);
}

void housekeeping()
{
}

uint8_t getJiffiesPerSecond()
{
	return(CLOCKS_PER_SEC);
}

uint8_t getRandomNumber(uint8_t maxExclusive)
{
    return (rand() % maxExclusive);
}


uint16_t custom_read_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, char *destination)
{
    return 0;
}

void custom_write_appkey(uint16_t creator_id, uint8_t app_id, uint8_t key_id, uint16_t count, char *data)
{
}


uint16_t custom_network_call(char *url, uint8_t *buffer, uint16_t max_len)
{
    uint8_t r;

    r = getResponse(url, buffer, max_len);
    return(r);
}