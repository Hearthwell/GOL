LIB_SRC_FILE := $(wildcard lib/src/*.c)
LIB_SRC_OBJ  := $(LIB_SRC_FILE:lib/src/%.c=lib/out/%.c.o)
LIB_INC_FLAG := -Ilib/include

lib/out/%.c.o:lib/src/%.c
	gcc -Wall -Wextra $(LIB_INC_FLAG) -c $< -o $@

gol: $(LIB_SRC_OBJ)
# TODO, BUILD LIB
	echo "BUILDING LIB NOT IMPLEMENTED"

SDL_VIEW_SRC_FILE := $(wildcard sdlView/src/*.c)
SDL_VIEW_SRC_OBJ  := $(SDL_VIEW_SRC_FILE:sdlView/src/%.c=sdlView/out/%.c.o)
SDL_VIEW_INC_FLAGS:= $(LIB_INC_FLAG) -IsdlView/include
SDL_VIEW_LIBS_FLAG:= $(shell pkg-config --libs sdl2) $(shell pkg-config --libs glew)
SDL_VIEW_LIBS_CL_FLAG:= $(SDL_VIEW_LIBS_FLAG) $(shell pkg-config --libs OpenCL)
sdlView/out/%.c.o:sdlView/src/%.c
	gcc -Wall -Wextra $(SDL_VIEW_INC_FLAGS) -c $< -o $@
nativeGOLSerial: sdlView/out/main.c.o sdlView/out/sdl_common.c.o $(LIB_SRC_OBJ)
	gcc $^ -o $@ $(SDL_VIEW_LIBS_FLAG)
nativeGOLOpenCL: sdlView/out/main_cl.c.o sdlView/out/sdl_common.c.o $(LIB_SRC_OBJ)
	gcc $^ -o $@ $(SDL_VIEW_LIBS_CL_FLAG)

TERMINAL_VIEW_SRC_FILES := $(wildcard terminalView/src/*.c)
TERMINAL_VIEW_SRC_OBJ   := $(TERMINAL_VIEW_SRC_FILES:terminalView/src/%.c=terminalView/out/%.c.o)
terminalView/out/%.c.o:terminalView/src/%.c
	gcc -Wall -Wextra $(LIB_INC_FLAG) -c $< -o $@
terminalGOLSerial: $(TERMINAL_VIEW_SRC_OBJ) $(LIB_SRC_OBJ)
	gcc $^ -o $@

clean:
	rm -rf lib/out/*
	rm -rf sdlView/out/*
	rm -f nativeGOLSerial nativeGOLOpenCL terminalGOLSerial