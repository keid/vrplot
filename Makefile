LDLIBS = -lpthread -lm -framework OpenGL -framework GLUT 
TARGETS = glsl1
OBJECTS = main.o trackball.o glsl.o VolumeRenderer.o

CXXFLAGS=-Wall -Wextra

CXX=/usr/bin/g++

$(TARGETS): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<


main.o: trackball.h glsl.h
trackball.o: trackball.h
glsl.o: glsl.h

clean:
	-rm -f $(TARGETS) *.o *~
