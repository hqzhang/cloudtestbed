CC=gcc
CPPFLAGS += -MD
SRC := $(wildcard *.c)
OBJS :=$(patsubst %.c,%.o,$(SRC))
DEPS:= $(patsubst %.o,%.d,$(OBJS))
LIBS:= 
EXEC:=exec
all: $(EXEC) run

$(EXEC): $(OBJS)
	gcc -o$@ $^ $(addprefix -l,$(LIBS))

-include $(DEPS)

.PHONY : clean distclean run
run:
	./$(EXEC)
clean:
	@rm -f *.o
	@rm -f *.d

distclean: clean
	@rm exec

