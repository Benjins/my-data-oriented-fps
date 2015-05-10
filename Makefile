build: src/*.cpp
	$(CXX) -std=c++11 -o my-fps src/*.cpp lib/*.a -lGL -lGLU -lGLEW -lfreeglut 
