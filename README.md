# bongfilter

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
