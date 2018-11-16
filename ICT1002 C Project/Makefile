##############################################################################
#
# ICT1002 Assignment 2, 2018-19 Trimester 1.
#
# You can use this file to build the ICT1002 Spreadsheet using nmake (on Windows)
# or make (on Unix and MacOS X).
#
# Use the top settings for Microsoft compilers (i.e. Visual Studio) and the
# lower settings for GNU compilers (MacOS X, MinGW and Linux). Use the #
# character to comment out whichever settings you don't want.
#
###############################################################################

#
# Use the following settings for Microsoft compilers (Visual Studio).
#
CC       = cl
CFLAGS   = /nologo /c /Fo:
LD       = link
LDFLAGS  = /nologo /out:
LDLIBS   = user32.lib
RM       = del
O        = .obj
X        = .exe

#
# Use the following settings for MacOS, MinGW and Linux.
#
#CC       = gcc
#CFLAGS   = -c -o
#LD       = gcc
#LDFLAGS  = -o 
#LDLIBS   =
#RM       = rm -f
#O        = .o
#X        =


###############################################################################
#
# Do not change anything below this line
#
###############################################################################

sheet1002$(X): commands$(O) main$(O) viewport$(O) worksheet$(O)
	$(LD) $(LDFLAGS)sheet1002$(X) commands$(O) main$(O) viewport$(O) worksheet$(O) $(LDLIBS)

commands$(O): commands.c sheet1002.h
	$(CC) $(CFLAGS) commands$(O) commands.c
	
main$(O): main.c sheet1002.h
	$(CC) $(CFLAGS) main$(O) main.c

viewport$(O): viewport.c sheet1002.h
	$(CC) $(CFLAGS) viewport$(O) viewport.c

worksheet$(O): worksheet.c sheet1002.h
	$(CC) $(CFLAGS) worksheet$(O) worksheet.c
	
clean:
	$(RM) *$(O)
