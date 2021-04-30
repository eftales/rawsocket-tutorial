.PHONY: clean stop
all:
	gcc client.c -o client 
	gcc server.c -o server

clean:
	rm -rf *.o client server
	
stop:
	pkill server

run:
	./server &
	./client