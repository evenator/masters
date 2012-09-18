CC=g++
CXXFLAGS=  -Wall -Wextra -Wformat=2 -O -Wno-write-strings -g
LDFLAGS= -lpthread -Wall -Wextra -Wformat=2 -O -lrt

ROOT_DIR = ../..
BUILD_DIR = .
LIB_DIR = $(ROOT_DIR)/lib
SHARED_OBJS_DIR = $(ROOT_DIR)/obj

PROG_SUFFIX =
OBJ_SUFFIX =.o


INCLUDE_DIRS += $(ROOT_DIR)/include \


INCLUDE_FLAG = -I
# The include flag should probably never, ever, ever change.
# Oh wellz!

INCLUDES += $(foreach include, $(INCLUDE_DIRS), $(INCLUDE_FLAG)$(include))

CXXFLAGS += $(INCLUDES)

OBJS+= $(SHARED_OBJS_DIR)/Logger$(OBJ_SUFFIX) \
       $(SHARED_OBJS_DIR)/abb2$(OBJ_SUFFIX) \
       $(SHARED_OBJS_DIR)/surgery$(OBJ_SUFFIX) \
       $(SHARED_OBJS_DIR)/Timer$(OBJ_SUFFIX) \
       $(SHARED_OBJS_DIR)/snet$(OBJ_SUFFIX) \
       $(SHARED_OBJS_DIR)/Object1$(OBJ_SUFFIX) \

       # $(BUILD_DIR)/xml$(OBJ_SUFFIX) \


TARGS += $(BUILD_DIR)/abb-hp$(PROG_SUFFIX)

VPATH += $(LIB_DIR) \
		 $(SHARED_OBJS_DIR) \

#	 $(ROOT_DIR)/Objects \
#	 $(ROOT_DIR)/xml \
#	 $(ROOT_DIR)/Reporter \
#	 $(ROOT_DIR)/Timer \
#	 $(ROOT_DIR)/snet \

INCLUDE_DIRS += $(ROOT_DIR)

.PHONY: all clean bare

all: mkdir $(TARGS) $(OBJS)

mkdir: 
	mkdir -p $(SHARED_OBJS_DIR)

$(TARGS): $(BUILD_DIR)/%$(PROG_SUFFIX): $(BUILD_DIR)/%$(OBJ_SUFFIX) $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@


$(OBJS): $(SHARED_OBJS_DIR)/%$(OBJ_SUFFIX): $(LIB_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $^ -o $@


clean:
	-rm -f $(foreach target,$(TARGS),$(subst $(PRG_SUFFIX),$(OBJ_SUFFIX),$(target)))
	-rm -f $(TARGS)

bare: clean
	-rm -f $(OBJS)
	-rm -f $(TARGS) .*~ *~ *.log


