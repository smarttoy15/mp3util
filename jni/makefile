objects = main.o utils.o types.o frame.o header.o id3v2lib.o
CCX = cc
CFLAGS = -g

all : $(objects)
	$(CCX) $(CFLAGS) -o mp3lib $(objects)
	
$(objects) : %.o : %.c
	$(CCX) $(CFLAGS) -c $< -o $@

clean:
	-del $(objects) mp3lib.exe
