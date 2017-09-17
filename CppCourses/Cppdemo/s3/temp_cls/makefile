EXECUTABLE =stacktest 

# Now alter any implicit rules' variables if you like, e.g.: 

CFLAGS := -g 
CXXFLAGS := $(CFLAGS) 
CXX:=g++

# You shouldn't need to change anything below this point

SOURCE := strng2.cc stacktem.cc
OBJS := $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCE))) 
DEPS := $(patsubst %.o,%.d,$(OBJS)) 
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS)) 
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)) $(patsubst %.d,%.cc,$(MISSING_DEPS))) 
CPPFLAGS += -MD 

.PHONY : everything deps objs clean veryclean rebuild 

everything : $(EXECUTABLE) 
deps : $(DEPS) 
objs : $(OBJS) 
clean : 
	@rm -f *.o 
	@rm -f *.d 
veryclean: clean 
	@rm -f $(EXECUTABLE) 
rebuild: veryclean everything 
ifneq ($(MISSING_DEPS),) 
	$(MISSING_DEPS) : 
		rm $(patsubst %.d,%.o,$@) 
endif 
-include $(DEPS) 
$(EXECUTABLE) : $(OBJS) 
	$(CXX) -o $(EXECUTABLE) $(OBJS) 

