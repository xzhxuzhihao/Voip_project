TARGET = register
OBJECTS = ./register_define_judge.o ./register.o 
VPATH = ./
CC = gcc
LDFLAGS = -L/usr/local/lib -leXosip2 -losip2 -losipparser2
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
%.o : %.c
	$(CC) $(LDFLAGS) -c -o $@ $<

clean:
	rm ./*.o register
