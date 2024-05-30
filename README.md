# bongfilter

This is bongfilter $Revision: 2.4 $.  Character substitutions can
 be made easily by substituting into the table caps[] and lowr[]
 below - a few substitutions have already been made as a
 demonstration.

  There is now an alternate character substitution list which will
 substitute multicharacter strings for certain characters, e.g.,
 /< for K.  Configure at your own peril.

## History

This was found embedded in an old [Brotherhood of Warez](http://www.textfiles.com/magazines/BOW/bow5.txt) e-zine as a uuencoded file, we took it out, cleaned it up, and are attempting to restore it to its glory.

The original author (Glasshead/hojunya@ecf.toronto.edu) is no where to be found.

## Compiling

```shell
make
```

## Usage

### Help

```shell
❯ ./bongfilt
goo's bongfilter $Revision: 2.3 $
Usage: bongfilt -none|-irc|-vt100 -ran|-alw [text]
```

### Random

```shell
❯ ./bongfilt -none -ran foobar
foob/\r
❯ ./bongfilt -none -ran this thing is really super k-rad
t|-|is T|-|i/\/g is r3ally super /<-r/\/>
```

### ALW mode

```shell
❯ ./bongfilt -none -alw this thing is really super k-rad
This Thing is r3a11y sup3r k-rad
❯ ./bongfilt -irc -alw this thing is really super k-rad
This Thing is r3a11y sup3r k-rad
```

### More

```
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
```
