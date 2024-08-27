# Define compiler
CXX = g++ 
# Compiler flags
CXXFLAGS = -std=gnu++14 -g 

# Define the target executable
TARGET = main

# Define source files
SOURCES = main.cpp Population.cpp Individual.cpp

# Define object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# To generate object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)
