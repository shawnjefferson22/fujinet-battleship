#include <lynx.h>
#include <stdint.h>

// Simple delay loop
void delay(unsigned int d) {
    unsigned int i;
    for (i = 0; i < d; ++i) { }
}

void initSound(void) {
    MIKEY.mstereo = 0;

   /*
    unsigned char volume;       // fd20
    unsigned char feedback;     // fd21
    unsigned char dac;          // fd22
    unsigned char shiftlo;      // fd23
    unsigned char reload;       // fd24
    unsigned char control;      // fd25
    unsigned char count;        // fd26
    unsigned char other;        // fd27
    */

    MIKEY.channel_a.volume = 0;
    MIKEY.channel_a.shiftlo = 0;
    MIKEY.channel_a.control = 0x18;
    MIKEY.channel_a.feedback = 1; 
    MIKEY.channel_a.other = 0;

    MIKEY.channel_b.volume = 0;
    MIKEY.channel_b.shiftlo = 0;
    MIKEY.channel_b.control = 0x18;
    MIKEY.channel_b.feedback = 1; 
    MIKEY.channel_b.other = 0;
}

void disableKeySounds()
{}

void enableKeySounds()
{}

void soundCursor()
{
    MIKEY.channel_a.control = 0x1E;
    MIKEY.channel_a.volume = 80;
    MIKEY.channel_a.reload = 200;
    delay(500);
    MIKEY.channel_a.volume = 0;
}

void soundSelect()
{
    MIKEY.channel_a.control = 0x1E;
    MIKEY.channel_a.volume = 0x127;
    MIKEY.channel_a.reload = 220;
    delay(800);
    MIKEY.channel_a.volume = 0;
}

void soundStop()
{
    MIKEY.channel_a.volume = 0;
    MIKEY.channel_b.volume = 0;
    MIKEY.mstereo = 0xFF;
}

void soundJoinGame()
{
    // Bright initial "ding"
    MIKEY.channel_a.volume = 127;
    MIKEY.channel_a.control = 0x18;

    MIKEY.channel_a.reload = 90;   // high pitch
    delay(2000);

    // Softer trailing tone
    MIKEY.channel_a.volume = 80;
    MIKEY.channel_a.reload = 140;  // slightly lower pitch
    delay(4000);

    MIKEY.channel_a.volume = 0;
}

void soundMyTurn()
{
	unsigned char i;

    MIKEY.channel_a.volume = 75;
    MIKEY.channel_a.control = 0x18;
    	
    for (i = 0; i < 2; i++) {
        MIKEY.channel_a.reload = 80;
        delay(5000);

        //MIKEY.channel_a.volume = 0;
        //delay(3000);

        MIKEY.channel_a.volume = 127;
    }

    MIKEY.channel_a.volume = 0;
}

void soundGameDone()
{
    MIKEY.channel_a.volume = 127;
    MIKEY.channel_a.control = 0x18;

    // Initial low tone
    MIKEY.channel_a.reload = 180;
    delay(3000);

    // Drop lower for “bong” feel
    MIKEY.channel_a.volume = 90;
    MIKEY.channel_a.reload = 220;
    delay(5000);

    MIKEY.channel_a.volume = 0;
}

void soundTick()
{
    MIKEY.channel_a.control = 0x1E;
    MIKEY.channel_a.volume = 0x1F;
    MIKEY.channel_a.reload = 200;
    delay(1000);
    MIKEY.channel_a.volume = 0;
}

void soundPlaceShip()
{
    MIKEY.channel_a.control = 0x1E;
    MIKEY.channel_a.volume = 0x127;
    MIKEY.channel_a.reload = 220;
    delay(800);
    MIKEY.channel_a.volume = 0;
}

void soundAttack(void) {
    unsigned char p;

    MIKEY.channel_a.control = 0x18;
    MIKEY.channel_a.feedback = 1;   // tone
    MIKEY.channel_a.volume = 127;

    // Start lower and descend further (heavier feel)
    for (p = 140; p < 220; p += 6) {
        MIKEY.channel_a.reload = p;
        delay(1200);   // longer step = more drawn out
    }

    // Brief pause before impact
    MIKEY.channel_a.volume = 90;
    delay(800);

    // Low rumble instead of hissy noise
    //MIKEY.channel_a.feedback = 0x3F;
    //MIKEY.channel_a.reload = 160;
    //MIKEY.channel_a.volume = 80;
    delay(2000);

    // Fade out
    //MIKEY.channel_a.volume = 40;
    //delay(1500);

    MIKEY.channel_a.volume = 0;
    MIKEY.channel_a.feedback = 1;
}

void soundInvalid()
{
 	unsigned char i;

    MIKEY.channel_a.control = 0x18;
    MIKEY.channel_a.volume = 0x1F;

    for (i = 0; i < 3; i++) {
        MIKEY.channel_a.reload = 100;
        delay(1500);

        MIKEY.channel_a.reload = 140;
        delay(1500);
    }

    MIKEY.channel_a.volume = 0;   
}

void soundHit(void) {

    MIKEY.channel_a.control = 0x18;
    MIKEY.channel_b.control = 0x18;

    // Noise burst
    MIKEY.channel_a.feedback = 0x3F;
    MIKEY.channel_a.volume = 127;
    MIKEY.channel_a.reload = 60;

    // Slight tonal layer
    MIKEY.channel_b.feedback = 1;
    MIKEY.channel_b.volume = 80;
    MIKEY.channel_b.reload = 90;

    delay(1000);

    // Quick decay
    MIKEY.channel_a.volume = 60;
    MIKEY.channel_b.volume = 40;
    delay(800);

    MIKEY.channel_a.volume = 0;
    MIKEY.channel_b.volume = 0;

    MIKEY.channel_a.feedback = 1;
}

void soundSink(void) {

    MIKEY.channel_a.control = 0x18;
    MIKEY.channel_b.control = 0x18;

    // --- Initial impact ---
    MIKEY.channel_a.feedback = 0x3F;
    MIKEY.channel_a.volume = 127;
    MIKEY.channel_a.reload = 50;

    delay(800);

    // --- Main explosion (both channels) ---
    MIKEY.channel_b.feedback = 0x3F;
    MIKEY.channel_b.volume = 100;
    MIKEY.channel_b.reload = 80;

    delay(1500);

    // --- Rumble decay ---
    MIKEY.channel_a.reload = 120;
    MIKEY.channel_a.volume = 80;

    MIKEY.channel_b.reload = 140;
    MIKEY.channel_b.volume = 60;

    delay(2000);

    // --- Final fade ---
    MIKEY.channel_a.volume = 40;
    MIKEY.channel_b.volume = 30;
    delay(1500);

    MIKEY.channel_a.volume = 0;
    MIKEY.channel_b.volume = 0;

    MIKEY.channel_a.feedback = 1;
    MIKEY.channel_b.feedback = 1;
}

void soundMiss(void) {

    MIKEY.channel_a.control = 0x18;

    // Use more chaotic noise (less tonal repetition)
    MIKEY.channel_a.feedback = 0x7F;

    // Very high reload = destroys pitch perception
    MIKEY.channel_a.reload = 255;

    // Short, dry burst
    MIKEY.channel_a.volume = 100;
    delay(400);

    MIKEY.channel_a.volume = 0;
    MIKEY.channel_a.feedback = 1;
}

//void pause(uint8_t frames)
//{}