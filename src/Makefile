# Compiling variables.
CC	:= ../common/cc5x.exe
CCFLAGS	:= -a -I"..\common\lib" -L -V -Q -B

SRC  	:= main.c
HEX	:= out.hex

all:$(HEX)

$(HEX): $(SRC)
	wine $(CC) -o$(HEX) $(CCFLAGS) $(SRC)

clean:
	@echo "delete file"
	rm -f $(HEX)
	rm -f *.cpr *.fcs *.lst *.occ *.var *.asm
