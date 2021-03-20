CC := gcc
CFL := -ggdb -fPIC
TOOLCHAIN :=
OUT_EXECUTABLE_NAME := fifa98_pc_models_parser
ARCH :=
LIBS := -lstdc++
SOURCE_FILES := main.cpp

all:
	$(TOOLCHAIN)$(CC) $(CFL) $(ARCH) $(SOURCE_FILES) $(LIBS) -o $(OUT_EXECUTABLE_NAME)

clear:
	rm -f *.o
	rm -f $(OUT_EXECUTABLE_NAME)