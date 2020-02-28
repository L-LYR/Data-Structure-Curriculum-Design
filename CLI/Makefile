objects = main.c memoryPool.c tokenizer.c lexer.c parser.c ast.c display.c rearrange.c

debug: $(objects)
	gcc -g -O0 $(objects) -o ./bin/mlpc

main: $(objects)
	gcc $(objects) -o ./bin/mlpc 

clean: 
	-rm ./bin/mlpc