// Copyright(c) KIPR,  2010
// printf, cbc_printf, cbc_display_clear configured to print in kissSim window
// History:
//    produced 1/28/2010 - cnw
//    ks_printf and ks_cbc_printf use the same printf strategy as cbc_printf for the CBC2
//
// stdarg.h provides macros for accessing a function's argument list ... see K&R
#include <stdarg.h>
#include <string.h>

              //***12345678901234567890123456789012345678901234567890123456789012345678901234567890
#define BLANKLINE "                                                                                "
// text print area rows & columns
#ifndef PROW_CNT
// also used in kiss-sim.c for glfw window specifications in __prtArea (extern below)
  #define PROW_CNT 9
#endif
#define PCOL_CNT 80

char __prtTEXT[PROW_CNT][PCOL_CNT+1]={
	BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE,BLANKLINE
	};

int __maxW = PCOL_CNT;

extern struct __prtSpec  __prtArea[];  // PROW_CNT entries in kiss-sim.c

int  __pROW=0, __pCOL=0;    // current print y=row, x=col in simulator display
void scroll(int cnt);
void refresh();
void ks_printf(char *t, ...)  // Note: for the elipsis any named arguments
                              // must be typed in line and precede the elipsis; there
                              // must be at least one named argument.
  {
    va_list argp;   // argp is typed for traversing the variable part of the arg list

	int i; char *c; double d;  // working variables to receive arg values
	char *cp, *fmte, sc;  // cp traverses format string t, fmte marks end of each format phrase, sc is switch control

	int y;           // row index
	char *dp;        // pointer into display
	int maxw;        // available room on line
	char fws[PCOL_CNT]; // formatted phrase work area
	char fmt[PCOL_CNT]; int fl; // fmt is a working string for each format extracted

	va_start (argp,t);  // t is last named argument in cbc_printf's function header;
	// this initializes argp to point to first variable arg

	dp = &__prtTEXT[__pROW][__pCOL];   // starting point for printf
	maxw=PCOL_CNT - __pCOL;            // space remaining on line
	for (cp = t; *cp; cp++)  // process printf string; stop when *cp = '\0'
	{
		if (*cp != '%')           // if not a format phrase
		{
			if(strspn(cp,"\n\t\v\b\r\f")>0) {  // is it a spec char? if so treat as if \n
				for (i=0;i<maxw;i++) { // clear balance of line
					*dp=' '; dp++;
				}
				if (__pROW < PROW_CNT-1) {
					__pROW++; __pCOL=0;
					dp = &__prtTEXT[__pROW][__pCOL]; maxw=PCOL_CNT;  // set up for new line
				}
				else break; // no more rows so go to display refresh and scroll
			}
			else { // nothing special about this one so insert it
				*dp=*cp; dp++; __pCOL++;
				maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			}
			continue;             // return to top
		}
		// OK, if we're here we may have hit a format phrase
		fmte = strpbrk(cp+1, "dioxXucsfeEgG%"); // look for format end
		// strpbrk returns the location of 1st character of its argument that is in the scan string
		if (fmte == NULL)        // what's left is not a format phrase so insert % and return to top
		{
			*dp='%'; dp++; __pCOL++;
			maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			continue;            // return to top
		}
		// OK, there looks to be a format phrase
		sc = *fmte;              // set switch control for the case
		fl = 1+fmte-cp;          // pick off phrase (pointed to by cp)
		strncpy(fmt,cp,fl);      // capture the format phrase
		fmt[fl] = '\0';          // make it a string
		switch (sc)              // process the kind of format specified
		{
			case 'd': case 'i': case 'o': case 'x': case 'X': case 'u': case 'c':
			i = va_arg(argp, int);    // return next parm as type int and step argp
			sprintf(fws,fmt,i);       // use sprintf to do the formatting to fws
			break;
			case 's':
			c = va_arg(argp, char *); // return next parm as type char * and step argp
			sprintf(fws,fmt,c);       // use sprintf to do the formatting to fws
			break;
			case 'f': case 'e': case 'E': case 'g': case 'G':
			d = va_arg(argp, double); // return next parm as type double and step argp
			sprintf(fws,fmt,d);       // use sprintf to do the formatting to fws
			break;
			case '%':                 // no format specified between %'s
			sprintf(fws,fmt);
			break;
		}
		for(i=0; i<strlen(fws); i++) {// insert formatted phrase in display map
			if (maxw==0) break;       // if no more room get out of this
		    *dp = fws[i];             // insert next character from formatted phrase
			dp++; __pCOL++; maxw--;
		}
		if (maxw==0) break;           // if no more room proceed to display refresh
		cp = fmte;                    // set cp for next pass
	}
	va_end(argp);                    //clean up
    // OK the print string is fully processed
    refresh();
    if(*cp != '\0') { // didn't finish string (so set for new new line)
        if (__pROW == PROW_CNT-1)
		   scroll(1); // state is now window scrolling
		__pCOL=0;
		if (__pROW < PROW_CNT-1) __pROW++;
		dp = &__prtTEXT[__pROW][__pCOL]; maxw=PCOL_CNT; // set up for new line
	}
  }

void ks_cbc_printf(int col, int row, char *t, ...)  // Note: for the elipsis any named arguments
                              // must be typed in line and precede the elipsis; there
                              // must be at least one named argument.
  {
    va_list argp;   // argp is typed for traversing the variable part of the arg list

	int i; char *c; double d;  // working variables to receive arg values
	char *cp, *fmte, sc;  // cp traverses format string t, fmte marks end of each format phrase, sc is switch control

	int y;           // row index
	char *dp;        // pointer into display
	int maxw;        // available room on line
	char fws[PCOL_CNT]; // formatted phrase work area
	char fmt[PCOL_CNT]; int fl; // fmt is a working string for each format extracted

	va_start (argp,t);  // t is last named argument in cbc_printf's function header;
	// this initializes argp to point to first variable arg
    if ((col>=PCOL_CNT)||(row>=PROW_CNT)) return;
    __pROW = row; __pCOL=col;    // keep track of next printf location if user switches to it
	dp = &__prtTEXT[row][col];   // starting point for printf
	maxw=PCOL_CNT - col;            // space remaining on line
	for (cp = t; *cp; cp++)  // process printf string; stop when *cp = '\0'
	{
		if (*cp != '%')           // if not a format phrase
		{
			if(strspn(cp,"\n\t\v\b\r\f")>0) {  // is it a spec char? if so treat as if \n
				for (i=0;i<maxw;i++) { // clear balance of line
					*dp=' '; dp++; __pCOL++;
				}
				if (row == PROW_CNT-1) break;  // out of rows
				row++; __pROW++;
				dp = &__prtTEXT[row][0]; maxw=PCOL_CNT;  // set up for new line
			}
			else { // nothing special about this one so insert it
				*dp=*cp; dp++; __pCOL++;
				maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			}
			continue;             // return to top
		}
		// OK, if we're here we may have hit a format phrase
		fmte = strpbrk(cp+1, "dioxXucsfeEgG%"); // look for format end
		// strpbrk returns the location of 1st character of its argument that is in the scan string
		if (fmte == NULL)        // what's left is not a format phrase so insert % and return to top
		{
			*dp='%'; dp++; __pCOL++;
			maxw--; if (maxw==0) break; // no more room on line so proceed to display refresh
			continue;            // return to top
		}
		// OK, there looks to be a format phrase
		sc = *fmte;              // set switch control for the case
		fl = 1+fmte-cp;          // pick off phrase (pointed to by cp)
		strncpy(fmt,cp,fl);      // capture the format phrase
		fmt[fl] = '\0';          // make it a string
		switch (sc)              // process the kind of format specified
		{
			case 'd': case 'i': case 'o': case 'x': case 'X': case 'u': case 'c':
			i = va_arg(argp, int);    // return next parm as type int and step argp
			sprintf(fws,fmt,i);       // use sprintf to do the formatting to fws
			break;
			case 's':
			c = va_arg(argp, char *); // return next parm as type char * and step argp
			sprintf(fws,fmt,c);       // use sprintf to do the formatting to fws
			break;
			case 'f': case 'e': case 'E': case 'g': case 'G':
			d = va_arg(argp, double); // return next parm as type double and step argp
			sprintf(fws,fmt,d);       // use sprintf to do the formatting to fws
			break;
			case '%':                 // no format specified between %'s
			sprintf(fws,fmt);
			break;
		}
		for(i=0; i<strlen(fws); i++) {// insert formatted phrase in display map
			if (maxw==0) break;       // if no more room get out of this
		    *dp = fws[i];             // insert next character from formatted phrase
			dp++; __pCOL++; maxw--;
		}
		if (maxw==0) break;           // if no more room proceed to display refresh
		cp = fmte;                    // set cp for next pass
	}
	va_end(argp);                    //clean up
    // OK the print string is fully processed
    refresh();
  }

void ks_cbc_display_clear() {
	int i;
	graphics_rectangle_fill(0,ksWindow.height-ksWindow.tHeight+1,ksWindow.width,ksWindow.height-1,WHITE);
	for (i=0;i<PROW_CNT;i++)
	   strcpy(__prtTEXT[i],BLANKLINE);
}


void scroll(int cnt) {
	int i,j=PROW_CNT-1;
	for (i=cnt;i<PROW_CNT;i++)
	   strcpy(__prtTEXT[i-cnt],__prtTEXT[i]);
	for (i=0;i<cnt;i++)
	   strcpy(__prtTEXT[j-i],BLANKLINE);
}
void refresh() {
    int i;
    graphics_rectangle_fill(0,ksWindow.height-ksWindow.tHeight+1,ksWindow.width,ksWindow.height-1,WHITE);
    for(i=0;i<PROW_CNT;i++) {
       NOBOLD g_printString(__prtTEXT[i], __prtArea[i].x, __prtArea[i].y,
                            __prtArea[i].r, __prtArea[i].g, __prtArea[i].b, __prtArea[i].size);
       sleep(.001); // glfw apparently needs a slight pause between calls to g_printString
	}
}
