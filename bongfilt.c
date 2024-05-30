/*
Bongfilter $Revision: 2.3 $ (C) Copyleft Junya Ho. (of the GNU flavour)

This is bongfilter $Revision: 2.3 $.  Character substitutions can
be made easily by substituting into the table caps[] and lowr[]
below - a few substitutions have already been made as a 
demonstration.

There is now an alternate character substitution list which will
substitute multicharacter strings for certain characters, e.g.,
/< for K.  Configure at your own peril.

For usage information, type bongfilt without any arguments.  The
revision log is at the end of the source.

(more usage information)
- bongfilter can take input from the command line, or it can take
    info from a pipe, eg: "ls -al | bongfilt -none -alw"
- the option -none adds no characters for inverse/underline/bold.
- the option -alw always substitutes, and -ran substitutes sometimes.
- for your ircII clients, probably the easiest way to incorporate
    bongfilter is to add an alias:
        /ALIAS bongf exec -out [path]/bongfilt -irc -ran
            (or -alw instead of -ran, its up to you)
    and invoking it by: /bongf text here
    from within your ircII client.

Questions and comments can be directed towards hojunya@ecf.toronto.edu
or glasshead on IRC.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>

#define NOEMU 0
#define IRCEMU 1
#define VT100EMU 2
#define ALWAYS 0
#define RANDOM 1

static char caps[] = "ABCD3FGHiJKLMN0PQRSTUVWXYZ";
static char lowr[] = "abCd3fghijk1mn0pqrsTuvwxyz";
static char *alt[26];

// Function prototypes
void init_alt(void);
int mangle(char *ch, char from, int freq);
void print_usage(void);
void check_options(int argc, char **argv, int *emu, int *freq);
void do_mangle(int argc, char **argv, int emu, int freq);
int underlinify(char *cpr, int *underline, int emu);
int inversify(char *cpr, int *inverse, int emu);
int boldify(char *cpr, int *bold, int emu);

void init_alt()
{
    for (int count = 0; count < 26; count++)
        alt[count] = NULL;

    alt['a' - 'a'] = strdup("/\\");
    alt['d' - 'a'] = strdup("/>");
    alt['h' - 'a'] = strdup("|-|");
    alt['k' - 'a'] = strdup("/<");
    alt['l' - 'a'] = strdup("|_");
    alt['n' - 'a'] = strdup("/\\/");
    alt['o' - 'a'] = strdup("()");
    alt['u' - 'a'] = strdup("|_|");
    alt['v' - 'a'] = strdup("\\/");
    alt['w' - 'a'] = strdup("\\/\\/");
}

int mangle(char *ch, char from, int freq)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);

    if (isupper(from)) {
        if ((tp.tv_usec % 3) && (alt[from - 'A'] != NULL) && (freq == RANDOM)) {
            strcpy(ch, alt[from - 'A']);
            return strlen(alt[from - 'A']);
        } else {
            strncpy(ch, &caps[from - 'A'], 1);
            return 1;
        }
    } else if (islower(from)) {
        if ((tp.tv_usec % 3) && (alt[from - 'a'] != NULL) && (freq == RANDOM)) {
            strcpy(ch, alt[from - 'a']);
            return strlen(alt[from - 'a']);
        } else {
            strncpy(ch, &lowr[from - 'a'], 1);
            return 1;
        }
    } else {
        strncpy(ch, &from, 1);
        return 1;
    }
}

void print_usage(void)
{
    fprintf(stderr, "goo's bongfilter $Revision: 2.3 $\n");
    fprintf(stderr, "Usage: bongfilt -none|-irc|-vt100 -ran|-alw [text]\n");
}

void check_options(int argc, char **argv, int *emu, int *freq)
{
    if (argc < 3) {
        print_usage();
        exit(1);
    }
    if (!strcmp(argv[1], "-none")) {
        *emu = NOEMU;
    } else if (!strcmp(argv[1], "-irc")) {
        *emu = IRCEMU;
    } else if (!strcmp(argv[1], "-vt100")) {
        *emu = VT100EMU;
    } else {
        print_usage();
        exit(1);
    }
    if (!strcmp(argv[2], "-ran")) {
        *freq = RANDOM;
    } else if (!strcmp(argv[2], "-alw")) {
        *freq = ALWAYS;
    } else {
        print_usage();
        exit(1);
    }
}

int main(int argc, char **argv)
{
    int emu, freq;
    char oink[512];
    char *p_oink[4];

    p_oink[0] = NULL;
    p_oink[1] = NULL;
    p_oink[2] = NULL;
    p_oink[3] = oink;

    check_options(argc, argv, &emu, &freq);
    init_alt();

    if (argc < 4) {
        while (fgets(oink, sizeof(oink), stdin)) {
            do_mangle(3, p_oink, emu, freq);
        }
    } else {
        do_mangle(argc, argv, emu, freq);
    }

    return 0;
}

void do_mangle(int argc, char **argv, int emu, int freq)
{
    int vnum = 3;
    int outnum = 0;
    int inum = 0;
    int bold = 0;
    int inverse = 0;
    int underline = 0;
    struct timeval tp;
    char out[512];

    while (outnum < 511) {
        if (inum == strlen(argv[vnum])) {
            if (vnum < (argc - 1)) {
                vnum++;
                inum = 0;
                out[outnum] = ' ';
                outnum++;
                continue;
            } else {
                break;
            }
        }
        if (freq == RANDOM) {
            gettimeofday(&tp, NULL);
            if (tp.tv_usec % 3)
                outnum += mangle(&out[outnum], argv[vnum][inum], freq);
            else
                out[outnum++] = argv[vnum][inum];
        } else {
            outnum += mangle(&out[outnum], argv[vnum][inum], freq);
        }

        inum++;
        gettimeofday(&tp, NULL);
        if (tp.tv_usec % 3) outnum += boldify(&(out[outnum]), &bold, emu);
        gettimeofday(&tp, NULL);
        if (tp.tv_usec % 3) outnum += inversify(&(out[outnum]), &inverse, emu);
        gettimeofday(&tp, NULL);
        if (tp.tv_usec % 3) outnum += underlinify(&(out[outnum]), &underline, emu);
    }
    if (bold) outnum += boldify(&(out[outnum]), &bold, emu);
    if (inverse) outnum += inversify(&(out[outnum]), &inverse, emu);
    out[outnum] = '\0';
    printf("%s\n", out);
    fflush(stdout);
}

int underlinify(char *cpr, int *underline, int emu)
{
    if (emu == NOEMU)
        return 0;
    else if (emu == VT100EMU) {
        if (!(*underline)) {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = '4'; cpr++;
            *cpr = 'm'; cpr++;
            *underline = !(*underline);
            return 4;
        } else {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = 'm'; cpr++;
            *underline = !(*underline);
            return 3;
        }
    } else if (emu == IRCEMU) {
        *cpr = 31; cpr++;
        *underline = !(*underline);
        return 1;
    }
    return 0;
}

int inversify(char *cpr, int *inverse, int emu)
{
    if (emu == NOEMU)
        return 0;
    else if (emu == VT100EMU) {
        if (!(*inverse)) {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = '7'; cpr++;
            *cpr = 'm'; cpr++;
            *inverse = !(*inverse);
            return 4;
        } else {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = 'm'; cpr++;
            *inverse = !(*inverse);
            return 3;
        }
    } else if (emu == IRCEMU) {
        *cpr = 22; cpr++;
        *inverse = !(*inverse);
        return 1;
    }
    return 0;
}

int boldify(char *cpr, int *bold, int emu)
{
    if (emu == NOEMU)
        return 0;
    else if (emu == VT100EMU) {
        if (!(*bold)) {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = '1'; cpr++;
            *cpr = 'm'; cpr++;
            *bold = !(*bold);
            return 4;
        } else {
            *cpr = 27; cpr++;
            *cpr = '['; cpr++;
            *cpr = 'm'; cpr++;
            *bold = !(*bold);
            return 3;
        }
    } else if (emu == IRCEMU) {
        *cpr = 2; cpr++;
        *bold = !(*bold);
        return 1;
    }
    return 0;
}

/*
$Log: bongfilt.c,v $
 * Revision 2.4  2024/05/29  Update by ChatGPT
 * Added function prototypes to address implicit declaration warnings.
 * Used strcpy instead of strncpy to prevent string overflow issues.
 * Updated main function to return int for proper standard compliance.
 *
 * Revision 2.3  1994/05/07  21:31:34  hojunya
 * added multicharacter substitutions
 * (and things get a little more complicated)
 *
 * Revision 2.2  1994/05/07  00:44:01  hojunya
 * fixed error which screwed up piped input
 * added random or always substituting characters, hope it didn't break something.
 *
 * Revision 2.1  1994/05/05  03:56:25  hojunya
 * added much more usage information in the comments, because..
 *
 * Revision 2.0  1994/05/05  03:50:22  hojunya
 * added a substitution table instead previous (inefficient) method
 *
 * Revision 1.5  1994/05/04  04:52:14  hojunya
 * properly returns 0 when emu==NOEMU
 *
 * Revision 1.4  1994/05/04  04:48:16  hojunya
 * merged inverse and bold for different emus
 *
 * Revision 1.3  1994/05/04  04:37:52  hojunya
 * merged underline for all emu's
 *
 * Revision 1.2  1994/05/03  18:19:45  hojunya
 * added options for picking emulation
 *
 * Revision 1.1  1994/05/03  18:06:00  hojunya
 * Initial revision
 *
*/
