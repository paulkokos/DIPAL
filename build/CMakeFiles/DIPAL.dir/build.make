# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/paulkokos/MyProjects/DIPAL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paulkokos/MyProjects/DIPAL/build

# Include any dependencies generated for this target.
include CMakeFiles/DIPAL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DIPAL.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DIPAL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DIPAL.dir/flags.make

CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o: /home/paulkokos/MyProjects/DIPAL/src/Image/Image.cpp
CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o -MF CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o.d -o CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o -c /home/paulkokos/MyProjects/DIPAL/src/Image/Image.cpp

CMakeFiles/DIPAL.dir/src/Image/Image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/src/Image/Image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/src/Image/Image.cpp > CMakeFiles/DIPAL.dir/src/Image/Image.cpp.i

CMakeFiles/DIPAL.dir/src/Image/Image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/src/Image/Image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/src/Image/Image.cpp -o CMakeFiles/DIPAL.dir/src/Image/Image.cpp.s

CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o: /home/paulkokos/MyProjects/DIPAL/src/Image/BinaryImage.cpp
CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o -MF CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o.d -o CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o -c /home/paulkokos/MyProjects/DIPAL/src/Image/BinaryImage.cpp

CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/src/Image/BinaryImage.cpp > CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.i

CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/src/Image/BinaryImage.cpp -o CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.s

CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o: /home/paulkokos/MyProjects/DIPAL/src/Image/GrayscaleImage.cpp
CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o -MF CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o.d -o CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o -c /home/paulkokos/MyProjects/DIPAL/src/Image/GrayscaleImage.cpp

CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/src/Image/GrayscaleImage.cpp > CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.i

CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/src/Image/GrayscaleImage.cpp -o CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.s

CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o: /home/paulkokos/MyProjects/DIPAL/src/Image/ColorImage.cpp
CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o -MF CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o.d -o CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o -c /home/paulkokos/MyProjects/DIPAL/src/Image/ColorImage.cpp

CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/src/Image/ColorImage.cpp > CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.i

CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/src/Image/ColorImage.cpp -o CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.s

CMakeFiles/DIPAL.dir/main.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/main.cpp.o: /home/paulkokos/MyProjects/DIPAL/main.cpp
CMakeFiles/DIPAL.dir/main.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/DIPAL.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/main.cpp.o -MF CMakeFiles/DIPAL.dir/main.cpp.o.d -o CMakeFiles/DIPAL.dir/main.cpp.o -c /home/paulkokos/MyProjects/DIPAL/main.cpp

CMakeFiles/DIPAL.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/main.cpp > CMakeFiles/DIPAL.dir/main.cpp.i

CMakeFiles/DIPAL.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/main.cpp -o CMakeFiles/DIPAL.dir/main.cpp.s

CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o: CMakeFiles/DIPAL.dir/flags.make
CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o: /home/paulkokos/MyProjects/DIPAL/src/Image/ImageFactory.cpp
CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o: CMakeFiles/DIPAL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o -MF CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o.d -o CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o -c /home/paulkokos/MyProjects/DIPAL/src/Image/ImageFactory.cpp

CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paulkokos/MyProjects/DIPAL/src/Image/ImageFactory.cpp > CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.i

CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paulkokos/MyProjects/DIPAL/src/Image/ImageFactory.cpp -o CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.s

# Object files for target DIPAL
DIPAL_OBJECTS = \
"CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o" \
"CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o" \
"CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o" \
"CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o" \
"CMakeFiles/DIPAL.dir/main.cpp.o" \
"CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o"

# External object files for target DIPAL
DIPAL_EXTERNAL_OBJECTS =

DIPAL: CMakeFiles/DIPAL.dir/src/Image/Image.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/src/Image/BinaryImage.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/src/Image/GrayscaleImage.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/src/Image/ColorImage.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/main.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/src/Image/ImageFactory.cpp.o
DIPAL: CMakeFiles/DIPAL.dir/build.make
DIPAL: CMakeFiles/DIPAL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/paulkokos/MyProjects/DIPAL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable DIPAL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DIPAL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DIPAL.dir/build: DIPAL
.PHONY : CMakeFiles/DIPAL.dir/build

CMakeFiles/DIPAL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DIPAL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DIPAL.dir/clean

CMakeFiles/DIPAL.dir/depend:
	cd /home/paulkokos/MyProjects/DIPAL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paulkokos/MyProjects/DIPAL /home/paulkokos/MyProjects/DIPAL /home/paulkokos/MyProjects/DIPAL/build /home/paulkokos/MyProjects/DIPAL/build /home/paulkokos/MyProjects/DIPAL/build/CMakeFiles/DIPAL.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/DIPAL.dir/depend

