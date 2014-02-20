CXX=g++
C_FLAGS=-c -Wall -std=c++11 -DPLATFORM_LINUX -DGL_GLEXT_PROTOTYPES -pthread -fpic -I/opt/local/include
L_FLAGS=-L/opt/local/lib -lX11 -lGL -lpthread

TARGET=KillTheWorld
SOURCES=main.cpp engineLinux.cpp aux.cpp render.cpp
HEADERS=minimath.h utils.h engine.h

OBJECTS=$(SOURCES:.cpp=.o)
.SUFFIXES: .cpp .o


game: $(SOURCES) $(HEADERS) $(TARGET) Makefile

$(TARGET): $(OBJECTS) Makefile
	$(CXX) $(L_FLAGS) $(OBJECTS) -o $@
	
.cpp.o: $(SOURCES) $(HEADERS) Makefile
	$(CXX) $(C_FLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)