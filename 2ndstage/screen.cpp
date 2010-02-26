#include "system.h"

unsigned char * vmem;
int curserX = 0;
int curserY = 0;
unsigned char foreColor = 14;
unsigned char backColor = 7;
int screenWidth = 80;
int screenHeight = 25;
int attrib = 0x0F;

void WriteCharacter(unsigned char c, int x, int y)
{
     //unsigned short attrib = (backcolour << 4) | (forecolour & 0x0F);
	 int location = (y * screenWidth + x);
	 vmem[location*2+0] = c;
	 vmem[location*2+1] = attrib;
     //vmem = c | (attrib << 8);
}




/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row 25 is the end, this means we need to scroll up */
    if(curserY >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = curserY - 25 + 1;
        memcpy (vmem, vmem + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (((unsigned short*)vmem + (25 - temp) * 80), blank, 80);
        curserY = 25 - 1;
    }
}

void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
	temp = curserY * screenWidth + curserX;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where2 the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}
/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < 25; i++)
        memsetw (((unsigned short*)vmem + i * 80), blank, 80);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    curserX = 0;
    curserY = 0;
    move_csr();
}

void WriteString(char *str, unsigned int x, unsigned int y)
{
//	int i;
	for(int i=0; i < strlen(str); i++)
	{
		WriteCharacter(str[i], x+i, y);
	}
}

void Printf(char *str)
{
	WriteString(str, curserX, curserY);
	curserX += strlen(str);
	if(curserX > screenWidth)
	{
		curserY++;
		curserX -= screenWidth;
	}
	move_csr();
}

void setupScreen(unsigned char * vmemptr)
{
	vmem = vmemptr;
}

void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

