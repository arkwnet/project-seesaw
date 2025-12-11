CXX = g++
CXXFLAGS =
LDFLAGS = -lfltk
TARGET = main
SRCS = main.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
