# All Targets
all: clean fs

# Tool invocations
# Executable "fs" depends on the files fs.o and main.o.
fs: bin/Commands.o bin/FileSystem.o bin/Environment.o bin/Files.o  bin/main.o
	@echo 'Building target: fs'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/fs bin/Commands.o bin/FileSystem.o bin/Environment.o bin/Files.o bin/main.o
	@echo 'Finished building target: fs'
	@echo ' '

# Depends on the source and header files
bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

# Depends on the source and header files
bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp

# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

#Clean the build directory
clean:
	rm -f bin/*
