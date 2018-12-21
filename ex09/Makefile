##############################################################################
# File:         Makefile
# Version:      1.1
# Date:         2016-03-31
# Author:       M. van der Sluys, J. Onokiewicz
# Description:  OPS exercise 5: Queues
##############################################################################


CXX = gcc
CXXFLAGS=-g -O0 -Wall -Wextra

EXECUTABLE = queue
SOURCES = ${wildcard *.c}
HEADERS = ${wildcard *.h}
OBJECTS = ${SOURCES:.c=.o}

# Rule to create .o from .c:
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: all
all: printversion ${EXECUTABLE}

$(EXECUTABLE): $(OBJECTS) buildnumber.num
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDXXFLAGS) -o $@
	@echo "-- Build: " $$(cat buildnumber.num)

# Create dependency file compiler option -MM
depend: $(SOURCES)
	$(CXX) $(CXXFLAGS) -MM  $^ > $@

-include depend

# Buildnumber
buildnumber.num: $(OBJECTS)
	@if ! test -f buildnumber.num; then echo 0 > buildnumber.num; fi
	@echo $$(($$(cat buildnumber.num)+1)) > buildnumber.num

# Create a clean environment
.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)

# Clean up dependency file
.PHONY: clean-depend
clean-depend: clean
	$(RM) depend

printversion:
	gcc --version |grep gcc

