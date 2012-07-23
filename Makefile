TARGET = vrplot

SRC=vrplot.cpp \
VolumeRenderer.cpp \
Controller.cpp \
commands/CommandPlot.cpp \
commands/CommandReplot.cpp \
Color.cpp \
ColorMap.cpp \
FileLoader.cpp \
Components.cpp \
FieldSelector.cpp \
CoordinateAdjuster.cpp \
VolumeData.cpp \
volumeGenerator/SimpleVolumeGenerator.cpp

#volumeGenerator/Demo0.cpp 

OBJS=$(patsubst %.cpp,%.o,$(SRC))
DEPENDS=$(patsubst %.cpp,%.d,$(SRC))

PACKAGES=gl glu libedit

CXXFLAGS=-Wall -Wextra -O3 -I./
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
	rm -f $(DEPENDS)
	find . -name '*.o' -delete
	find . -name '*~' -delete
	find . -name '*\#*' -delete

distclean: clean
	rm -rf $(TARGET)
