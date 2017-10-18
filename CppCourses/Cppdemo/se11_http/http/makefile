EXECUTABLE = server 

# Now alter any implicit rules' variables if you like, e.g.: 
CFLAGS := -D_REENTRANT -I./include

ifdef DEBUG 
	CFLAGS += -g -DDEBUG
endif
vpath %.h=include
CXXFLAGS:= $(CFLAGS) 

CXX:=g++

LIBS=pthread 
# You shouldn't need to change anything below this point

SOURCE :=$(wildcard *.cpp) $(wildcard *.cc)
OBJS := $(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(SOURCE))) 
DEPS := $(patsubst %.o,%.d,$(OBJS)) 
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS)) 
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)) $(patsubst %.d,%.cc,$(MISSING_DEPS))) 
CPPFLAGS += -MD 

.PHONY : everything deps objs clean veryclean rebuild 

everything : $(EXECUTABLE) 
deps : $(DEPS) 
objs : $(OBJS) 
clean : 
	-rm *.o 
	-rm *.d 
veryclean: clean 
	-rm $(EXECUTABLE) 
rebuild: veryclean everything 
ifneq ($(MISSING_DEPS),)
	$(MISSING_DEPS) : 
		@rm $(patsubst %.d,%.o,$@) 
endif 
-include $(DEPS) 
$(EXECUTABLE) : $(OBJS) 
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -l,$(LIBS))
