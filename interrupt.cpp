
#include "io.h"
#include "types.h"
#include "screen.h"
#include "kbd.h"

extern "C" {

void isr_default_int(void)
{
	screen::print((char*) "interrupt\n");
}

void isr_clock_int(void)
{
/*	static int tic = 0;
	static int sec = 0;
	tic++;
	if (tic % 500 == 0) {
		sec++;
		tic = 0;
		screen::print((char*) ".");
	}
*/
}

void isr_kbd_int(void)
{
        uchar i;
        static int lshift_enable;
        static int rshift_enable;
        static int alt_enable;
        static int ctrl_enable;

        do {
                i = inb(0x64);
        } while ((i & 0x01) == 0);

        i = inb(0x60);
        i--;

        if (i < 0x80) {         /* touche enfoncee */
                switch (i) {
                case 0x29:
                        lshift_enable = 1;
                        break;
                case 0x35:
                        rshift_enable = 1;
                        break;
                case 0x1C:
                        ctrl_enable = 1;
                        break;
                case 0x37:
                        alt_enable = 1;
                        break;
                default:
                        screen::putcar(kbdmap
                               [i * 4 + (lshift_enable || rshift_enable)]);
                }
        } else {                /* touche relachee */
                i -= 0x80;
                switch (i) {
                case 0x29:
                        lshift_enable = 0;
                        break;
                case 0x35:
                        rshift_enable = 0;
                        break;
                case 0x1C:
                        ctrl_enable = 0;
                        break;
                case 0x37:
                        alt_enable = 0;
                        break;
                }
        }

        screen::show_cursor();
}

}

