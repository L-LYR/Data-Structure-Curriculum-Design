objects = main.c memoryPool.c tokenizer.c lexer.c parser.c ast.c display.c rearrange.c

debug: $(objects)
	cc -g -O0 $(objects) -o lpc

main: $(objects)
	cc -o lpc $(objects)

clean: 
	-rm lpc *.o