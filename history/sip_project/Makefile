TARGET = register
OBJECTS = ./register_define_judge.o ./register.o 
VPATH = ./
CC = gcc
LDFLAGS = -L/usr/local/lib  -I/usr/local/include -leXosip2 -losip2 -losipparser2 -lpthread
$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS) $(LDFLAGS)
%.o : %.c
	$(CC) -g -c -o $@ $< $(LDFLAGS)

clean:
	rm ./*.o register 
