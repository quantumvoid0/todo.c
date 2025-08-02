install:
	bash -c "gcc src/main.c -o todo $$(pkg-config --cflags --libs gtk4)"
	install -Dm755 todo ~/.local/bin/todo
	rm -f todo

uninstall:
	rm -f ~/.local/bin/todo
