.PHONY: all clean

CC = $(if $(COMPILER),$(COMPILER),g++)
CFLAGS = -fdiagnostics-color=always -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef			 \
		 -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs 		 \
		 -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual	 \
		 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers \
		 -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual 			 \
		 -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel 		 \
		 -Wtype-limits -Wwrite-strings -Werror=vla

CFLAGS_SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,$\
				   float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,$\
				   object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
				   undefined,unreachable,vla-bound,vptr

OPTIMISATION = $(if $(NDEBUG),-O2 -DNDEBUG, -O0) -march=native $(OPTION_FLAGS) -masm=intel -g

EXTERNAL_DIR =
LIB_DIR =
IFLAGS = $(LIB_DIR:%=-I%/ )
LIB_FILES = #$(if $(NDEBUG),List/List.a,ListDebug/ListDebug.a)
LIBS = $(LIB_FILES:%=$(LIB_DIR)/%)

SRC_DIR = src
BUILD_DIR = build
NON_CODE_DIRS = $(BUILD_DIR) .vscode .git
TARGET = $(if $(OPTION_NAME),$(OPTION_NAME),main)

CD = $(shell pwd)


NESTED_CODE_DIRS_CD = $(shell find ./$(SRC_DIR) -maxdepth 5 -type d $(NON_CODE_DIRS:%=! -path "*%*"))
NESTED_CODE_DIRS = $(NESTED_CODE_DIRS_CD:.%=%)



FILES_FULL = $(shell find ./$(SRC_DIR) -name "*.cpp")
ASM_FILES_FULL = $(shell find ./$(SRC_DIR) -name "*.nasm")

FILES = $(FILES_FULL:.%=%)
ASM_FILES = $(ASM_FILES_FULL:.%=%)

MAKE_DIRS = $(NESTED_CODE_DIRS:%=$(BUILD_DIR)%)
OBJ = $(FILES:%=$(BUILD_DIR)%)
ASM_OBJ = $(ASM_FILES:%=$(BUILD_DIR)%)
DEPENDS = $(OBJ:%.cpp=%.d)
OBJECTS = $(OBJ:%.cpp=%.o) $(ASM_OBJ:%.nasm=%.o)

target: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(CC) $(OPTIMISATION) $(IFLAGS) $(CFLAGS) $(if $(sanitizer), $(CFLAGS_SANITIZER)) $^ -o $@ $(LIBS)

$(BUILD_DIR):
	@mkdir ./$@

$(MAKE_DIRS): | $(BUILD_DIR)
	@mkdir ./$@

-include $(DEPENDS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR) $(MAKE_DIRS)
	@$(CC) $(OPTIMISATION) $(IFLAGS) $(CFLAGS) $(if $(sanitizer), $(CFLAGS_SANITIZER)) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.nasm | $(BUILD_DIR) $(MAKE_DIRS)
	@nasm -g -f elf64 $< -o $@

.PHONY: callgrind perf perf_build callgrind_test perf_test

callgrind:
	valgrind --dump-instr=yes --collect-jumps=yes --tool=callgrind --callgrind-out-file=test/callgrind/$(prog)/callgrind.out.main_$(prog)$(prog_n) ./main_$(prog)

perf:
	sudo perf record -o test/perf/$(prog)/main_$(prog)$(prog_n).data --call-graph dwarf ./main_$(prog)
	chmod a=rwx test/perf/$(prog)/main_$(prog)$(prog_n).data

perf_build:
	@make clean
	@make OPTION_FLAGS="" OPTION_NAME=main_debug
	@make clean
	@make NDEBUG=1 OPTION_FLAGS="" OPTION_NAME=main_base
	@make clean
	@make NDEBUG=1 OPTION_FLAGS="-DCRC_OPTIMISATION" OPTION_NAME=main_crc_only
	@make clean
	@make NDEBUG=1 OPTION_FLAGS="-DCRC_OPTIMISATION -DSTRCMP_OPTIMISATION" OPTION_NAME=main_crc_cmp
	@make clean

callgrind_test:
	@make callgrind prog=debug prog_n=1
	@make callgrind prog=debug prog_n=2
	@make callgrind prog=debug prog_n=3

	@make callgrind prog=base prog_n=1
	@make callgrind prog=base prog_n=2
	@make callgrind prog=base prog_n=3

	@make callgrind prog=crc_only prog_n=1
	@make callgrind prog=crc_only prog_n=2
	@make callgrind prog=crc_only prog_n=3

	@make callgrind prog=crc_cmp prog_n=1
	@make callgrind prog=crc_cmp prog_n=2
	@make callgrind prog=crc_cmp prog_n=3

perf_test:
	@make perf prog=debug prog_n=1
	@make perf prog=debug prog_n=2
	@make perf prog=debug prog_n=3

	@make perf prog=base prog_n=1
	@make perf prog=base prog_n=2
	@make perf prog=base prog_n=3

	@make perf prog=crc_only prog_n=1
	@make perf prog=crc_only prog_n=2
	@make perf prog=crc_only prog_n=3

	@make perf prog=crc_cmp prog_n=1
	@make perf prog=crc_cmp prog_n=2
	@make perf prog=crc_cmp prog_n=3

clean:
	@rm -rf ./$(BUILD_DIR)/*
	@rm -f ./$(TARGET)

