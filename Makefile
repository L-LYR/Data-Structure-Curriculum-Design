objects = main.o memoryPool.o tokenizer.o lexer.o parser.o 

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

clean: 
	-rm main $(objects)