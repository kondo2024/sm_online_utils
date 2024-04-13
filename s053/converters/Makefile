TARGETS = smts_converter plastics_converter dc_converter nebula_converter

SRCS  = $(wildcard *.cc)
OBJS  = $(notdir $(SRCS:.cc=.o))
SMCONVLIB = ../../lib/smconverter/libsmconverter.a

CXXFLAGS = -Wall -O2 -fPIC $(shell root-config --cflags) -I$(TARTSYS)/include -I./../../lib/smconverter
LDFLAGS += $(shell root-config --glibs) -lXMLParser -L$(TARTSYS)/lib -lanacore -lanabrips -lanasamurai

all: $(OBJS) $(TARGETS)

smts_converter: smts_converter.cc $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ smts_converter.o $(SMCONVLIB)

plastics_converter: plastics_converter.cc $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ plastics_converter.o $(SMCONVLIB)

dc_converter: dc_converter.cc $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ dc_converter.o $(SMCONVLIB)

nebula_converter: nebula_converter.cc $(SMCONVLIB)
	$(CXX) $(LDFLAGS) -o $@ nebula_converter.o $(SMCONVLIB)

install:
	cp $(TARGETS) ../bin
clean:
	rm -f *.o *~ $(TARGETS) 
	cd ../bin;rm -f $(TARGETS)
