# Compiling variables.
CC	:= ../common/cc5x.exe
CCFLAGS	:= -a -I"..\common\lib" -L -V -Q -B

SRC := main.c
HEX	:= out.hex

all: table-create $(HEX)

$(HEX): $(SRC) max7219-1chip.c out.h fsm.c
	wine $(CC) -o$(HEX) $(CCFLAGS) $(SRC)

table-create: table-create.c number.h
	rm -f table-create out.h
	gcc table-create.c -o table-create && ./table-create out.h

clean:
	@echo "delete file"
	rm -f $(HEX)
	rm -f *.cpr *.fcs *.lst *.occ *.var *.asm
	rm -f table-create out.h
