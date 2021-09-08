CC=mpicc
OBJ =tool.o  
APPS=app

.PHONY:library
library:libtool.so

libtool.so: $(OBJ)
	$(CC) -fPIC -rdynamic -shared -o $@ $^

tool.o: tool.c 
	$(CC) -c -fPIC -rdynamic $< -o $@ 

.PHONY: app
#app:$(APPS)
	
app: app.c
	$(CC) $< -o app

.PHONY: clean
clean:
	@rm -f a.out
	@rm -f $(APPS)
	@rm -f ./*.o
