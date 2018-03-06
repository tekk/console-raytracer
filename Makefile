###################################################
#                                                 #
# PROJEKT DO PGR - REAL TIME ASCII RAY TRACER     #
#                                                 #
###################################################

# nastaveni
CFLAGS = -Wall -lm -lncurses
CC=gcc               # pro systemy divne nastavene, mozno vyhodit podle libosti

##################################################
# obecne cile
.PHONY: all clean
all: raytracer

clean:
	$(RM) raytracer raytracer.exe
	$(RM) *~

##################################################
# pravidla

%.exe: %.c
	$(CC) -o $@ $^ $(CFLAGS)

##################################################
# zavislosti

	raytracer.exe: raytracer.c

