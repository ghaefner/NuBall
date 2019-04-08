ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINCDIR   := $(DESTDIR)$(shell root-config --incdir)
ROOTLIBDIR   := $(DESTDIR)$(shell root-config --libdir)
ROOTVER      := $(shell root-config --version | cut -b1)

CXX          = g++

CXXFLAGS     += -g
CXXFLAGS     += -Wall
CXXFLAGS     += -I.
CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLIBS)

EXE = ReadEvents
SRCS += ReadEvents.cxx

DEPS += Lookup.hh
DEPS += MyHistograms.hh
DEPS += Raw.hh
DEPS += Unpacked.hh
DEPS += idmap_238u.hxx

$(EXE): $(SRCS) $(DEPS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	rm -f *~ $(EXE)
