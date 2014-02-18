CXX=g++
C_FLAGS=-c -Wall -std=c++11 -DPLATFORM_LINUX -pthread -fpic -I/opt/local/include
L_FLAGS=-L/opt/local/lib -lX11 -lGL -lpthread

TARGET=KillTheWorld
SOURCES=engine.cpp platformLinux.cpp

OBJECTS=$(SOURCES:.cpp=.o)


game: $(SOURCES) $(TARGET) Makefile

$(TARGET): $(OBJECTS) Makefile
	$(CXX) $(L_FLAGS) $(OBJECTS) -o $@
	
.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(C_FLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)