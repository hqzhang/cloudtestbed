SRCLIBS :=$(wildcard *.c)
OBJLIBS := $(SRCLIBS:.c=.o)
DEPLIBS:=$(OBJLIBS:.o=.d) 
CFLAGS +=-I../
libunp.a:$(OBJLIBS)
	$(AR) rcs $@ $(OBJLIBS)

-include $(DEPLIBS)

.PHONY : clean distclean

clean:
	@rm -f *.o
	@rm -f *.d
	@rm -f libunp.a

