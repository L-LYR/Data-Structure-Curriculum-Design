objects = main.c memoryPool.c tokenizer.c lexer.c parser.c ast.c display.c rearrange.c

debug: $(objects)
	gcc -g -O0 $(objects) -o mlpc

main: $(objects)
	gcc -o mlpc $(objects)

clean: 
	-rm mlpc