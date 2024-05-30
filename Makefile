#$Log: Makefile,v $
# Revision 2.2  1994/05/09  00:54:39  hojunya
# added EXEC_NAME variable for those who
# would wish to misappropriate the noble origins of
# bongfilt
#
# Revision 2.1  1994/05/09  00:52:27  hojunya
# added all the variables and stuff
#
# Revision 1.1  1994/05/03  18:20:36  hojunya
# Initial revision
#

#$Id: Makefile,v 2.2 1994/05/09 00:54:39 hojunya Exp hojunya $

CC=gcc
CFLAGS=-O4
INSTALL_DIR=/tmp
EXEC_NAME=bongfilt

$(EXEC_NAME):
	$(CC) $(CFLAGS) -o $(EXEC_NAME) bongfilt.c

install:
	make $(EXEC_NAME)
	strip $(EXEC_NAME)
	/bin/cp bongfilt $(INSTALL_DIR)
	chmod 555 $(INSTALL_DIR)/$(EXEC_NAME)

default:
	$(EXEC_NAME)

