# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/Documents/GitHub/unidos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Documents/GitHub/unidos

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/d/Documents/GitHub/unidos/CMakeFiles /mnt/d/Documents/GitHub/unidos/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/d/Documents/GitHub/unidos/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named unidos

# Build rule for target.
unidos: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 unidos
.PHONY : unidos

# fast build rule for target.
unidos/fast:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/build
.PHONY : unidos/fast

dospath.o: dospath.c.o

.PHONY : dospath.o

# target to build an object file
dospath.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/dospath.c.o
.PHONY : dospath.c.o

dospath.i: dospath.c.i

.PHONY : dospath.i

# target to preprocess a source file
dospath.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/dospath.c.i
.PHONY : dospath.c.i

dospath.s: dospath.c.s

.PHONY : dospath.s

# target to generate assembly for a file
dospath.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/dospath.c.s
.PHONY : dospath.c.s

fdtable.o: fdtable.c.o

.PHONY : fdtable.o

# target to build an object file
fdtable.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/fdtable.c.o
.PHONY : fdtable.c.o

fdtable.i: fdtable.c.i

.PHONY : fdtable.i

# target to preprocess a source file
fdtable.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/fdtable.c.i
.PHONY : fdtable.c.i

fdtable.s: fdtable.c.s

.PHONY : fdtable.s

# target to generate assembly for a file
fdtable.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/fdtable.c.s
.PHONY : fdtable.c.s

global.o: global.c.o

.PHONY : global.o

# target to build an object file
global.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/global.c.o
.PHONY : global.c.o

global.i: global.c.i

.PHONY : global.i

# target to preprocess a source file
global.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/global.c.i
.PHONY : global.c.i

global.s: global.c.s

.PHONY : global.s

# target to generate assembly for a file
global.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/global.c.s
.PHONY : global.c.s

ints/int10.o: ints/int10.c.o

.PHONY : ints/int10.o

# target to build an object file
ints/int10.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int10.c.o
.PHONY : ints/int10.c.o

ints/int10.i: ints/int10.c.i

.PHONY : ints/int10.i

# target to preprocess a source file
ints/int10.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int10.c.i
.PHONY : ints/int10.c.i

ints/int10.s: ints/int10.c.s

.PHONY : ints/int10.s

# target to generate assembly for a file
ints/int10.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int10.c.s
.PHONY : ints/int10.c.s

ints/int15.o: ints/int15.c.o

.PHONY : ints/int15.o

# target to build an object file
ints/int15.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int15.c.o
.PHONY : ints/int15.c.o

ints/int15.i: ints/int15.c.i

.PHONY : ints/int15.i

# target to preprocess a source file
ints/int15.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int15.c.i
.PHONY : ints/int15.c.i

ints/int15.s: ints/int15.c.s

.PHONY : ints/int15.s

# target to generate assembly for a file
ints/int15.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int15.c.s
.PHONY : ints/int15.c.s

ints/int20.o: ints/int20.c.o

.PHONY : ints/int20.o

# target to build an object file
ints/int20.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int20.c.o
.PHONY : ints/int20.c.o

ints/int20.i: ints/int20.c.i

.PHONY : ints/int20.i

# target to preprocess a source file
ints/int20.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int20.c.i
.PHONY : ints/int20.c.i

ints/int20.s: ints/int20.c.s

.PHONY : ints/int20.s

# target to generate assembly for a file
ints/int20.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int20.c.s
.PHONY : ints/int20.c.s

ints/int21.o: ints/int21.c.o

.PHONY : ints/int21.o

# target to build an object file
ints/int21.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int21.c.o
.PHONY : ints/int21.c.o

ints/int21.i: ints/int21.c.i

.PHONY : ints/int21.i

# target to preprocess a source file
ints/int21.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int21.c.i
.PHONY : ints/int21.c.i

ints/int21.s: ints/int21.c.s

.PHONY : ints/int21.s

# target to generate assembly for a file
ints/int21.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/ints/int21.c.s
.PHONY : ints/int21.c.s

main.o: main.c.o

.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i

.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s

.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/main.c.s
.PHONY : main.c.s

mem.o: mem.c.o

.PHONY : mem.o

# target to build an object file
mem.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mem.c.o
.PHONY : mem.c.o

mem.i: mem.c.i

.PHONY : mem.i

# target to preprocess a source file
mem.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mem.c.i
.PHONY : mem.c.i

mem.s: mem.c.s

.PHONY : mem.s

# target to generate assembly for a file
mem.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mem.c.s
.PHONY : mem.c.s

mount.o: mount.c.o

.PHONY : mount.o

# target to build an object file
mount.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mount.c.o
.PHONY : mount.c.o

mount.i: mount.c.i

.PHONY : mount.i

# target to preprocess a source file
mount.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mount.c.i
.PHONY : mount.c.i

mount.s: mount.c.s

.PHONY : mount.s

# target to generate assembly for a file
mount.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/mount.c.s
.PHONY : mount.c.s

psp.o: psp.c.o

.PHONY : psp.o

# target to build an object file
psp.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/psp.c.o
.PHONY : psp.c.o

psp.i: psp.c.i

.PHONY : psp.i

# target to preprocess a source file
psp.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/psp.c.i
.PHONY : psp.c.i

psp.s: psp.c.s

.PHONY : psp.s

# target to generate assembly for a file
psp.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/psp.c.s
.PHONY : psp.c.s

util/uc.o: util/uc.c.o

.PHONY : util/uc.o

# target to build an object file
util/uc.c.o:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/util/uc.c.o
.PHONY : util/uc.c.o

util/uc.i: util/uc.c.i

.PHONY : util/uc.i

# target to preprocess a source file
util/uc.c.i:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/util/uc.c.i
.PHONY : util/uc.c.i

util/uc.s: util/uc.c.s

.PHONY : util/uc.s

# target to generate assembly for a file
util/uc.c.s:
	$(MAKE) -f CMakeFiles/unidos.dir/build.make CMakeFiles/unidos.dir/util/uc.c.s
.PHONY : util/uc.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... unidos"
	@echo "... edit_cache"
	@echo "... dospath.o"
	@echo "... dospath.i"
	@echo "... dospath.s"
	@echo "... fdtable.o"
	@echo "... fdtable.i"
	@echo "... fdtable.s"
	@echo "... global.o"
	@echo "... global.i"
	@echo "... global.s"
	@echo "... ints/int10.o"
	@echo "... ints/int10.i"
	@echo "... ints/int10.s"
	@echo "... ints/int15.o"
	@echo "... ints/int15.i"
	@echo "... ints/int15.s"
	@echo "... ints/int20.o"
	@echo "... ints/int20.i"
	@echo "... ints/int20.s"
	@echo "... ints/int21.o"
	@echo "... ints/int21.i"
	@echo "... ints/int21.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... mem.o"
	@echo "... mem.i"
	@echo "... mem.s"
	@echo "... mount.o"
	@echo "... mount.i"
	@echo "... mount.s"
	@echo "... psp.o"
	@echo "... psp.i"
	@echo "... psp.s"
	@echo "... util/uc.o"
	@echo "... util/uc.i"
	@echo "... util/uc.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

