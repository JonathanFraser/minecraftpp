TARGETS = chunk_reader NBT_test mapper
PACKAGES := zlib libpng
CXX = g++
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,build/%.o,$(SOURCES))
DEPENDS := $(patsubst src/%.cpp,build/%.d,$(SOURCES))
TARGET_OBJ := $(addprefix build/,$(addsuffix .o,$(TARGETS)))
OBJECTS := $(filter-out $(TARGET_OBJ),$(OBJECTS))

override CXXFLAGS := -Wall -Wextra --std=gnu++0x -g -O3 $(shell pkg-config --cflags $(PACKAGES)) $(CXXFLAGS) 
override LDFLAGS :=  --std=gnu++0x -g $(LDFLAGS) 
override LIBS := $(shell pkg-config --libs $(PACKAGES)) $(LIBS)

.PHONY: all
all: $(TARGETS)

$(TARGETS):%:$(OBJECTS) build/%.o
	$(CXX) $(LDFLAGS) -o $@ $+ $(LIBS)

.PHONY: doc
doc:
	doxygen

	
build/%.o: src/%.cpp build/%.d
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -MM $< -MT $(patsubst build/%.d,build/%.o,$@) || rm $@

.PHONY: clean clean_dep clean_obj clean_bin clean_doc
clean: clean_dep clean_obj clean_bin clean_doc

clean_dep:
	rm -f $(DEPENDS)

clean_obj:
	rm -f $(OBJECTS)
	rm -f $(TARGET_OBJ)
clean_bin:
	rm -f $(TARGETS)

clean_doc:
	rm -fr html
	rm -fr latex
include $(DEPENDS)
