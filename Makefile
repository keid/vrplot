TARGET = vrplot
test_TARGET = test_vrplot

target_SRC=vrplot.cpp

main_SRC= \
ParameterTable.cpp \
Parameters.cpp \
VolumeRenderer.cpp \
Controller.cpp \
CommandList.cpp \
commands/CommandPlot.cpp \
commands/CommandReplot.cpp \
commands/CommandSet.cpp \
commands/CommandUnset.cpp \
commands/CommandShow.cpp \
commands/CommandParamsAutoscale.cpp \
expr/ExprGenerator.cpp \
expr/ExprConstant.cpp \
Color.cpp \
ColorMap.cpp \
FileLoader.cpp \
Components.cpp \
FieldSelector.cpp \
CoordinateAdjuster.cpp \
VolumeData.cpp \
volumeGenerator/SimpleVolumeGenerator.cpp

#volumeGenerator/Demo0.cpp 

SRC=$(main_SRC) $(target_SRC)

test_SRC= tests/vrplotTest.cpp \
tests/ExprConstantTest.cpp \
tests/ParametersTest.cpp \
$(main_SRC)

OBJS=$(patsubst %.cpp,%.o,$(SRC))

DEPENDS=$(patsubst %.cpp,%.d,$(SRC))

PACKAGES=gl glu libedit

CXXFLAGS=-Wall -Wextra -O3 -I./
CXXFLAGS+=$(shell pkg-config --cflags ${PACKAGES} )

LIBS = -lpthread -lm -lglut
LIBS+=$(shell pkg-config --libs ${PACKAGES} )

##

test_OBJS=$(patsubst %.cpp,%.o,$(test_SRC))

test_DEPENDS=$(patsubst %.cpp,%.d,$(tetst_SRC))

test_LIBS = -lpthread -lm -lglut
test_LIBS+=$(shell pkg-config --libs ${PACKAGES} ) -lcppunit

##

CXX=g++

.PHONY: all test clean distclean

all: $(TARGET)

test: $(test_TARGET)
	./$(test_TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

$(test_TARGET): $(test_OBJS)
	$(CXX) -o $@ $^ $(test_LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	@set -e; $(CXX) -MM $(CXXFLAGS) $< \
	| sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@; \
	[ -s $@ ] || rm -f $@
-include $(DEPENDS)
-include $(test_DEPENDS)

clean:
	rm -f $(DEPENDS) $(test_DEPENDS)
	find . -name '*.o' -delete
	find . -name '*~' -delete
	find . -name '*\#*' -delete

distclean: clean
	rm -rf $(TARGET) $(test_TARGET)

