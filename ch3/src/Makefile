.PHONY: clean stop
all:
	gcc send.c encapsulate.h encapsulate.c -o send 

clean: 
	rm -rf *.o send 
	
stop:
	pkill send
