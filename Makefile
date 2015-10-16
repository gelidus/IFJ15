# Compiler
CXX = gcc
# Preprocessor flags
CPPFLAGS =
# Compiler flags
CXXFLAGS = -std=c99 -Wall -pedantic -Wextra -g -o0
# Linker flags
LDFLAGS =

# Read (space-separated) sources list
SOURCES := $(wildcard *.c)
# Generate objects list (PATtern SUBSTitute .c -> .o)
OBJS := $(patsubst %.c, %.o, $(SOURCES))

# A list of all needed special libraries (eg pthread for threads)
LIBS :=
# Automatically append '-l' in front of all above libs (eg. 'm' -> '-lm' for math lib)
$(addprefix -l,$(LIBS))

# The first target is the default one, 'all' is the most expected one; relay to 'release' target
all: release

# Rule for compiling .c source files to .o object files
.c.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ -lm

# Cleanup
clean:
	rm -fv *.o
	rm -fv *.gch
	rm -fv Makefile.deps

# Generate dependencies for all source files using -MM switch (-M lists all deps, including a lot of the system headers)
deps:
	$(CXX) -MM $(SOURCES) > Makefile.deps

# Effectively, the default build target ('all' redirects here)
# Append compiler code optimization
# release: CXXFLAGS += -O2
# Building depends on generated dependencies and all .o (object) files
release: deps $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@ -lm

# Debugging target, append max possible level (3rd) of adding debugging symbols to the output program
debug: CXXFLAGS += -g3
# As 'release', depends on generated dependencies and all .o files
debug: deps $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

# Just a testing target for echo-ing all the variables used ('@' sign prevents outputing the executed command string)
echo:
    @echo CXX: $(CXX)
    @echo CPPFLAGS: $(CPPFLAGS)
    @echo CXXFLAGS: $(CXXFLAGS)
    @echo LDFLAGS: $(LDFLAGS)
    @echo SOURCES: $(SOURCES)
    @echo OBJS: $(OBJS)
    @echo LIBS: $(LIBS)

#GGGGG  G  GGGGG
#G      G    G
#G  GG  G    G
#G   G  G    G
#GGGGG  G    G

#commit do lokalniho rezpozitare
commit: clean
	git add --all
	git commit -m "$(m)"

#push do centralniho repozitare
push:
	git push

#pullnuti z repozitare
pull:
	git pull

#odebere soubor z gitu, ale nevymaze ho z disku
remove:
	git rm --cached $(f)

#vymaze soubor z gitu a z disku
remove2: remove
	rm $(f)

# Force all targets (even if file with the same name exists in the directory)
.PHONY: all clean deps release debug commit push pull remove remove2 echo

# Include (ie. insert here literally) all the generated dependencies
# '-' prevents make from showing errors if the file doesn't exist (or is not readable)
-include Makefile.deps
