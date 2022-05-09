PREFIX = /usr/local/bin
TARGET = trivium
GCCFLAGS = -O3

.PHONY: all clean install uninstall
all: $(TARGET)

$(TARGET): trivium.c
	$(CC) $(GCCFLAGS) $^ -o $(TARGET)
install:
	install $(TARGET) $(PREFIX)
clean:
	rm $(TARGET)

uninstall:
	rm $(PREFIX)/$(TARGET)
