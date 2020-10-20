test:main.o HttpHandler.o
	cc main.o HttpHandler.o -o ./bin/test
main.o:./src/main.c 
	cc ./src/main.c -c
HttpHandler.o:./src/HttpHandler.c
	cc ./src/HttpHandler.c -c
clean:
	rm -f main.o HttpHandler.o