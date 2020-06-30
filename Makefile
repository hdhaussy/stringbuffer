CFLAGS += -g
EXE=sb_test

all: $(EXE)

clean:
	rm $(EXE)

$(EXE): stringbuffer.c

test: $(EXE)
	./$(EXE)
