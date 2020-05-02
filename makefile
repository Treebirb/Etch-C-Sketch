src = $(wildcard src/*.c)
dep = $(wildcard src/*.h)
obj = $(patsubst %.c,bin/%.o,$(notdir $(src)))

all: $(obj) $(dep)
	gcc $(obj) -o bin/etch -lmingw32 -lSDL2main -lSDL2 -mwindows

bin/%.o : src/%.c $(dep)
	gcc -c $< -o $@ -lmingw32 -lSDL2main -lSDL2

var:
	@echo '$(src)'
	@echo '$(dep)'
	@echo '$(obj)'

clean:
	powershell rm bin/*.exe, bin/*.o
