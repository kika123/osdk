/* 
	2ndstage.cpp

	http://www.freewebs.com/kmcguire/hod/
	http://www.osdever.net/forums/

	josh@eezznet.com
	Leonard Kevin McGuire Jr. 2004
*/
struct HOD_HEADER{
	unsigned int		signature;
	unsigned int		lpexport;
	unsigned int		lpreloc;
	unsigned int		lpcode;
	unsigned int		exportcnt;
	unsigned int		relcnt;
	unsigned int		codecnt;
	unsigned int		characteristic;
	unsigned int		lpextra;
	unsigned int		extracnt;
	unsigned int		lpcreloc;
	unsigned int		crelcnt;
};

#define dd(x)                            \
        __asm _emit (x)       & 0xff     \
        __asm _emit     (x) >> 8  & 0xff \
        __asm _emit     (x) >> 16 & 0xff \
        __asm _emit     (x) >> 24 & 0xff
#define KERNEL_STACK               0x00103fff
#define KERNEL_START               0x00101000
#define KERNEL_LENGTH               0x0000200F

#define EXTERN		extern "C"
#define EXPORT		__declspec(dllexport)    // exported from DLL
#define IMPORT		__declspec(dllimport)    // imported from DLL
#define NAKED		__declspec(naked)		// no prolog or epilog code added
#define NORETURN	__declspec(noreturn)



/*void main(void);

void WriteCharacter(unsigned char * vmem, unsigned int color, unsigned char c, unsigned int x, unsigned int y);
void WriteString(unsigned char * vmem, unsigned int color, unsigned int x, unsigned int y, char *str);
int strlen(char *str);*/
#include "system.h"
//#include "screen.cpp"
//void WriteCharacter(unsigned char c, unsigned char forecolour, unsigned char backcolour, int x, int y);

/*
	Microsoft Visual C++ Linker, looks for this function as the default entry point.
	It isnt a bad name either.
*/

NAKED EXPORT void __multiboot_entry__(void)
{
          _asm {
          
          multiboot_header:
                    dd(0x1BADB002)              // ; magic
                    dd(1 << 16)                   // ; flags
                    dd(-(0x1BADB002 + (1 << 16)))     //; checksum
                    dd(0x00101000)               //; header_addr
                    dd(0x00101000)               //; load_addr
                    dd(0x00102FFF)               //; load_end_addr
                    dd(0x00102FFF)              // ; bss_end_addr
                    dd(0x00101030)               //; entry_addr
                    dd(0x00000000)               //; mode_type
                    dd(0x00000000)               //; width
                    dd(0x00000000)               //; height
                    dd(0x00000000)               //; depth

          kernel_entry:
                    mov     esp,     KERNEL_STACK

                    xor     ecx,     ecx
                    push     ecx
                    popf

                    push     eax
                    push     ebx
                    
		  }
		  main();
          _asm          jmp     $
          
}


__declspec (naked) __declspec(dllexport) void mainCRTStartup(void)
{
	/* 
		We want to fix up the segments, before the compiler inserts any prolog
		code for setting up the stack frame. I disabled the function prolog and
		epilog by using a naked function.
	*/

	_asm cli
	_asm mov ax, 10h
	_asm mov ds, ax
	_asm mov es, ax
	_asm mov fs, ax
	_asm mov gs, ax
	_asm mov ax, 18h
	_asm mov ss, ax
	_asm mov esp, 0x90000
	/*
		Setup stack frame pointer.
	*/
	_asm mov ebp, esp
	_asm push ebp
	/*
		All code is placed in or called from this function.
	*/
	main();
	_asm hlt
}


/*void hod_Relocate(unsigned int imageaddr, unsigned int pageaddr)
{
	HOD_HEADER * hdr;
	unsigned int * reloc;
	hdr = (HOD_HEADER*)imageaddr;
	/* data relocations *//*
	reloc = (unsigned int*)(imageaddr + hdr->lpreloc);
    for(unsigned int ri = 0; ri < hdr->relcnt; ri++)
		*(unsigned int*)(imageaddr + hdr->lpcode + reloc[ri]) =	*(unsigned int*)(imageaddr + hdr->lpcode + reloc[ri]) + pageaddr;
	/* code relocations *//*
	reloc = (unsigned int*)(imageaddr + hdr->lpcreloc);
	for(unsigned int ri = 0; ri < hdr->crelcnt; ri++)
		*(unsigned int*)(imageaddr + hdr->lpcode + reloc[ri]) =	*(unsigned int*)(imageaddr + hdr->lpcode + reloc[ri]) + imageaddr + hdr->lpcode;
	return;
}*/

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}


unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    //_asm  ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	__asm { 
		mov dx, _port
		in al, dx
		mov rv, al
	}
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
    __asm  mov al, _data
    __asm  mov dx, _port
    __asm  out dx, al

}

int strlen(char *str)
{
    int retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}
/*
	To debug quickly. Press CTRL+F5 (this disables MSVC's debugger witch will attach to BOCHS NOT your kernel.)
	Global or Static initialized varibles are NOT allowed in the HOD. They must be initialized during
	run-time. The vmem varibles in the function main is pre-initialized in code however when compiled
	it is truely initialized via a instruction moving 0xB8000 into a temp slot of the stack so those
	kind of initializations are okay.
*/
void main(void)
{
	/*
		Any global or static varibles now have a address using this offset(0xFA000) in memory.
		The 0x900 tells the location of the HOD header or commonly the first byte of the HOD
		image.
	*/
	//hod_Relocate(0x900, 0xFA000);
	
	/* TODO: PLACE YOUR CODE HERE */
	unsigned char * vmem = (unsigned char*)0xB8000;
	//WriteCharacter(vmem, 14, 'A', 0,0);
	//WriteCharacter(vmem, 14, 'B', 1,1);
	setupScreen(vmem);
	cls();
	settextcolor(14, 0);
	//WriteCharacter('A', 0, 0);
	Printf("Hello World");
	//WriteString(vmem, 13, 12, 12, "Hello World");
	/*char *str = "Hello, world 1 2 3 4 5 6 7 8", *ch;
	unsigned short *vidmem = (unsigned short*) 0xb8000;
	unsigned i;
	
	for (ch = str, i = 0; *ch; ch++, i++)
		vidmem[i] = (unsigned char) *ch | 0x0700;*/
		
	for (;;)
		;
	/*for(unsigned int x = 0; x < (80*20); x++)
	{
		WriteCharacter(vmem, 'A', x);
		//vmem[x*2+0] = 'D';
		//vmem[x*2+1] = 14;
	}*/
	return;
}



