# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/danis/retele/PROIECT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/danis/retele/PROIECT/build1

# Include any dependencies generated for this target.
include CMakeFiles/ClientSecundar.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ClientSecundar.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ClientSecundar.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ClientSecundar.dir/flags.make

CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o: CMakeFiles/ClientSecundar.dir/flags.make
CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o: ../ClientSecundar.c
CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o: CMakeFiles/ClientSecundar.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/danis/retele/PROIECT/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o -MF CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o.d -o CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o -c /home/danis/retele/PROIECT/ClientSecundar.c

CMakeFiles/ClientSecundar.dir/ClientSecundar.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ClientSecundar.dir/ClientSecundar.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/danis/retele/PROIECT/ClientSecundar.c > CMakeFiles/ClientSecundar.dir/ClientSecundar.c.i

CMakeFiles/ClientSecundar.dir/ClientSecundar.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ClientSecundar.dir/ClientSecundar.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/danis/retele/PROIECT/ClientSecundar.c -o CMakeFiles/ClientSecundar.dir/ClientSecundar.c.s

# Object files for target ClientSecundar
ClientSecundar_OBJECTS = \
"CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o"

# External object files for target ClientSecundar
ClientSecundar_EXTERNAL_OBJECTS =

ClientSecundar: CMakeFiles/ClientSecundar.dir/ClientSecundar.c.o
ClientSecundar: CMakeFiles/ClientSecundar.dir/build.make
ClientSecundar: CMakeFiles/ClientSecundar.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/danis/retele/PROIECT/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ClientSecundar"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ClientSecundar.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ClientSecundar.dir/build: ClientSecundar
.PHONY : CMakeFiles/ClientSecundar.dir/build

CMakeFiles/ClientSecundar.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ClientSecundar.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ClientSecundar.dir/clean

CMakeFiles/ClientSecundar.dir/depend:
	cd /home/danis/retele/PROIECT/build1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/danis/retele/PROIECT /home/danis/retele/PROIECT /home/danis/retele/PROIECT/build1 /home/danis/retele/PROIECT/build1 /home/danis/retele/PROIECT/build1/CMakeFiles/ClientSecundar.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ClientSecundar.dir/depend

