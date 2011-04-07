#
# Some rules will need second expansions as they are static pattern rules which do variable dereferences based on target name.
#
.SECONDEXPANSION:

#
# Build a full CXXFLAGS, LDFLAGS, and LIBS.
#
override PACKAGES := zlib
SOURCES = src
override CXXFLAGS := $(shell pkg-config --cflags $(PACKAGES) | sed 's/-I/-isystem /g') -std=gnu++0x $(CXXFLAGS)
override LDFLAGS := $(shell pkg-config --libs-only-other --libs-only-L $(PACKAGES)) $(LDFLAGS)
override LIBS := $(shell pkg-config --libs-only-l $(PACKAGES)) $(LIBS)

#
# The target to build everything.
#
.PHONY : world
world:bin/minecraftpp.a

#
# Prevent echoing of compilation commands.
#
.SILENT :

#
# Rule to build the documentation.
#
.PHONY : doc
doc :
	doxygen

#
# Rule to link a final executable.
#
bin/minecraftpp.a : $$(addprefix build/,$$(subst .cpp,.o,$$(shell find $(SOURCES) -name .svn -prune -o -name '*.cpp' -print) ))
	echo "  LD	$@"
	mkdir -p bin
	$(AR)	rcs $@ $+

#
# Rule to make a .d file from a .cpp file.
#
build/%.d : %.cpp
	echo "  DEP	$@"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MT '$(@:.d=.o) $@' -MM -MF $@ $< || { $(RM) $@; false; }

#
# Rule to make a .o file from a .cpp file.
#
build/%.o : %.cpp
	echo "  CXX	$@"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o$@ -c $<

#
# Include all the dependency files.
#
ALL_D_FILES=$(addprefix build/,$(subst ./,,$(subst .cpp,.d,$(shell find . -name .svn -prune -o -name '*.cpp' -print))))

include $(ALL_D_FILES)

#
# Rule to make only the dependency files.
#
.PHONY : dep
dep : $(ALL_D_FILES)

#
# Rules to clean.
#
.PHONY : clean
clean : cleanbin cleandoc cleanobj

.PHONY : cleanbin
cleanbin :
	$(RM) -rf bin

.PHONY : cleandep
cleandep :
	find build -name '*.d' -delete

.PHONY : cleandoc
cleandoc :
	$(RM) -rf html

.PHONY : cleanobj
cleanobj :
	$(RM) -rf build
