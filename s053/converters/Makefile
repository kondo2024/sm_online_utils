TARGETS = smts_converter nebula_converter 

SRCS  = $(wildcard *.cc)
OBJS  = $(notdir $(SRCS:.cc=.o))
SMCONVLIB = ../../smconverterlib/libsmconverter.a

CXXFLAGS = -Wall -O2 -fPIC $(shell root-config --cflags) -I$(TARTSYS)/include -I./../../smconverterlib
LDFLAGS += $(shell root-config --glibs) -lXMLParser -L$(TARTSYS)/lib -lanacore -lanabrips -lanasamurai

all: $(OBJS) $(TARGETS)

smts_converter: smts_converter.o $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ $? 

nebula_converter: nebula_converter.o $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ $? 

clean:
	rm -f *.o *~ $(TARGETS)
