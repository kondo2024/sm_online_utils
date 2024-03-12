TARGET1 = libsmconverter.so
TARGET2 = libsmconverter.a

SRCS  = $(wildcard *.cc)
HDRS  = $(wildcard *.hh)
OBJS  = $(notdir $(SRCS:.cc=.o))

CXXFLAGS = -Wall -O2 -fPIC $(shell root-config --cflags) -I$(TARTSYS)/include
LDFLAGS += $(shell root-config --glibs) -L$(TARTSYS)/lib -lanacore -lanabrips -lanasamurai

all: lib

lib: $(OBJS) 
	$(CXX) -shared $(LDFLAGS) -o $(TARGET1) $^
	ar rs $(TARGET2) $^

clean:
	rm -f *.o *~ $(TARGET1) $(TARGET2)
