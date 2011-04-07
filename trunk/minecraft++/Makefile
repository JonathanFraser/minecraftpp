# ========== STOP ========== STOP ========== STOP ==========
#
# The contents of this file should not generally be modified.
# The rules in this file are generic and compatible with building multiple executables.
# To change the set of executables compiled, the set sources for each executable, or the compilation or linking flags, edit project.mk instead.
#
# ========== STOP ========== STOP ========== STOP ==========

#
# Some rules will need second expansions as they are static pattern rules which do variable dereferences based on target name.
#
.SECONDEXPANSION :

#
# Pull in the project rules.
#
include project.mk

#
# Build a full CXXFLAGS, LDFLAGS, and LIBS.
#
override CXXFLAGS := $(PROJECT_CXXFLAGS) $(shell pkg-config --cflags $(PACKAGES) | sed 's/-I/-isystem /g') $(CXXFLAGS)
override LDFLAGS := $(PROJECT_LDFLAGS) $(shell pkg-config --libs-only-other --libs-only-L $(PACKAGES)) $(LDFLAGS)
override LIBS := $(PROJECT_LIBS) $(shell pkg-config --libs-only-l $(PACKAGES)) $(LIBS)

#
# The target to build everything.
#
.PHONY : world
world : $(addprefix bin/,$(EXECUTABLES))

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
$(addprefix bin/,$(EXECUTABLES)) : bin/% : $$(addprefix obj/,$$(subst .cc,.o,$$(subst .cpp,.o,$$(shell find $$(SOURCES_%) -name .svn -prune -o -name '*.cpp' -print) $$(if $$(filter proto,$$(SOURCES_%)),$(PROTOBUF_CCS)))))
	echo "  LD	$@"
	mkdir -p bin
	$(CXX) $(LDFLAGS) -o$@ $+ $(LIBS)

#
# Rule to make a .d file from a .cpp file.
#
obj/%.d : %.cpp
	echo "  DEP	$@"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MT '$(@:.d=.o) $@' -MM -MF $@ $< || { $(RM) $@; false; }

#
# Rule to make a .o file from a .cpp file.
#
obj/%.o : %.cpp
	echo "  CXX	$@"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o$@ -c $<

#
# Include all the dependency files.
#
ALL_D_FILES=$(addprefix obj/,$(subst ./,,$(subst .cpp,.d,$(shell find . -name .svn -prune -o -name '*.cpp' -print))))

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
clean : cleanbin cleandoc cleanobj cleanpb cleantags

.PHONY : cleanbin
cleanbin :
	$(RM) -rf bin

.PHONY : cleandep
cleandep :
	find obj -name '*.d' -delete

.PHONY : cleandoc
cleandoc :
	$(RM) -rf html

.PHONY : cleanobj
cleanobj :
	$(RM) -rf obj
