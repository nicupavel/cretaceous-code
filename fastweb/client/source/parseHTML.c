//parseHTML.c

#include <stdio.h>
#include <string.h>


#define WHITECHAR(x) ((x) == 9 || (x) == 10 || (x) == 12 || (x) == 13 || (x) == ' ')

#define U(x) ((x) == '"' || (x) == '\'')

// is an attribute ?
static inline int atchr(unsigned char c)
{
        return isA(c) || (c > ' ' && c != '=' && c != '<' && c != '>');
}
	

void parseHTML (const char *tag)
{

    (void)fputs (tag,stdout);

}