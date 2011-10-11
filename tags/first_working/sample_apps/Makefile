.SECONDEXPANSION:
#
# Build a full CXXFLAGS, LDFLAGS, and LIBS.
#
TARGETS = NBT_test mapper chunk_reader IteratorTest

override PACKAGES := zlib libpng

override CXXFLAGS := -std=gnu++0x -Wall -Wextra -Wconversion -Wundef -Wold-style-cast -march=native -O3 -g -pthread $(CXXFLAGS)
override CXXFLAGS := -I../minecraft++/src/ $(shell pkg-config --cflags $(PACKAGES) | sed 's/-I/-isystem /g') $(CXXFLAGS)
override LDFLAGS := $(shell pkg-config --libs-only-other --libs-only-L $(PACKAGES)) $(LDFLAGS)
override LIBS := $(shell pkg-config --libs-only-l $(PACKAGES)) $(LIBS)

MCRAFT = ../minecraft++/bin/minecraftpp.a

#
# The target to build everything.
#
.PHONY : world
world: $(TARGETS)

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

.PHONY: $(MCRAFT)
$(MCRAFT):
		$(MAKE) -C ../minecraft++

#
# Rule to link a final executable.
#
# $$(addprefix build/,$$(subst .cpp,.o,$$(shell find $$(SOURCES_%) -name .svn -prune -o -name '*.cpp' -print) ))
#
$(TARGETS) :%: build/src/%.o $(MCRAFT)
	echo "  LD	$@"
	$(CXX) $(LDFLAGS) -o$@ $+  $(LIBS)

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
	$(RM) -rf $(TARGETS)

.PHONY : cleandep
cleandep :
	find build -name '*.d' -delete

.PHONY : cleandoc
cleandoc :
	$(RM) -rf html
	$(RM) -rf latex

.PHONY : cleanobj
cleanobj :
	$(RM) -rf build
