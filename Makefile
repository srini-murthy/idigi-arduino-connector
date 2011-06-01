#
#  Copyright (C) 2011 by Digi International Inc.
#  All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License version 2  as published by
#  the Free Software Foundation.
#
#  Type 'make help' for more information.

include $(IDK_RULES)

LIB_DIR=./private
SAMPLE_DIR=./public/sample
OBJDIR=$(LIB_DIR)

ifeq ($(LITTLE_ENDIAN),true)
CFLAGS += -D_LITTLE_ENDIAN
endif

ifeq ($(DEBUG),true)
CFLAGS += -DDEBUG -g
else
CFLAGS += -DNDEBUG -O2
endif

CFLAGS += -I./public/include -I./private -Wall -Werror

vpath $(LIB_DIR)/%.c
vpath $(LIB_DIR)/%.h

LIB =$(LIBDIR)/libidk.so

all: $(LIB)

OBJS = $(OBJDIR)/idk_api.o

$(OBJS): $(LIB_DIR)/*.c $(LIB_DIR)/*.h


$(LIB): $(OBJS)
	$(LD) $(LDFLAGS) $@

MAKE= make IDK_RULES="../../../$(IDK_RULES)" DEBUG="$(DEBUG)" LITTLE_ENDIAN="$(LITTLE_ENDIAN)" LIB="../../../$(LIBDIR)"

linux:
	echo "building" $(SAMPLE_DIR)/linux;\
	TARGETDIR=`pwd` ; cd $(SAMPLE_DIR)/linux;\
	$(MAKE) -f Makefile all;\
	cd $${TARGETDIR};

.PHONY: clean
clean:
	-rm -f $(OBJS) $(LIB)
	TARGETDIR=`pwd` ; cd $(SAMPLE_DIR)/linux;\
	$(MAKE) -f Makefile clean;\
	cd $${TARGETDIR};\

.PHONY: help
help:
	@echo "This makefile only supports the GNU toolchain"
	@echo "Options:"
	@echo "    IDK_RULES     = Location and  name for toolset"
	@echo "    LIBDIR        = Location of library"
	@echo "    DEBUG         = true or false for debugging"
	@echo "    LITTLE_ENDIAN = true or false for little endian"
	@echo "Targets:"
	@echo "    all           - Build IDK library"
	@echo "    linux         - Build linux sample"
	@echo "    clean         - Delete all object files"
	@echo "IDK_RULES contain the following:"
	@echo "    CC            - Pointer to compiler"
	@echo "    LD            - Pointer to linker"
	@echo "    CFLAGS        - Options to the compiler"
	@echo "    LDFLAGS       - Options to the linker"
	@echo "NOTES:"
	@echo "    If you only wish to build a sample you can type make in the"
	@echo "    sample subdirectory once the IDK library is built."
