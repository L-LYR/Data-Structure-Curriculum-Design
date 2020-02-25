objects = main.o memoryPool.o tokenizer.o lexer.o parser.o ast.o display.o rearrange.o

main: $(objects)
	cc -m32 -o main $(objects)

main.o: main.c
	cc -m32 -c main.c

memoryPool.o: memoryPool.c
	cc -m32 -c memoryPool.c

tokenizer.o: tokenizer.c
	cc -m32 -c tokenizer.c

lexer.o: lexer.c
	cc -m32 -c lexer.c

parser.o: parser.c
	cc -m32 -c parser.c

ast.o: ast.c
	cc -m32 -c ast.c

display.o: display.c
	cc -m32 -c display.c

rearrange.o: rearrange.c
	cc -m32 -c rearrange.c

clean: 
	-rm main $(objects)