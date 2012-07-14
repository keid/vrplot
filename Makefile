LDLIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lpthread -lm
TARGETS = glsl1
OBJECTS = main.o trackball.o glsl.o

CXX=/opt/local/bin/g++-mp-4.5

$(TARGETS): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: trackball.h glsl.h
trackball.o: trackball.h
glsl.o: glsl.h

clean:
	-rm -f $(TARGETS) *.o *~
