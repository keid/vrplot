TARGET = volumerenderer

SRC=main.cpp trackball.cpp \
VolumeRenderer.cpp Controller.cpp \
FileLoader.cpp \
SimpleVolumeGenerator.cpp

OBJS=$(patsubst %.cpp,%.o,$(SRC))
DEPENDS=$(patsubst %.cpp,%.d,$(SRC))

PACKAGES=gl glu libedit

CXXFLAGS=-Wall -Wextra -O3
CXXFLAGS+=$(shell pkg-config --cflags ${PACKAGES} )

LIBS = -lpthread -lm -lglut
LIBS+=$(shell pkg-config --libs ${PACKAGES} )

CXX=g++

.PHONY: all clean distclean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	@set -e; $(CXX) -MM $(CXXFLAGS) $< \
	| sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@; \
	[ -s $@ ] || rm -f $@
-include $(DEPENDS)

clean:
	rm -f *.o *~ $(DEPENDS)

distclean: clean
	rm -rf $(TARGET)
