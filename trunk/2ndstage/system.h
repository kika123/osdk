#ifndef __SYSTEM_H
#define __SYSTEM_H


extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void cls();

extern void main(void);
extern void WriteCharacter(unsigned char c, int x, int y);
extern void WriteString(char *str, unsigned int x, unsigned int y);
extern int strlen(char *str);
extern void setupScreen(unsigned char *vmemptr);
extern void Printf(char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);

#endif