install:
	bash -c "gcc main.c -o todo $$(pkg-config --cflags --libs gtk4)"
	install -Dm755 todo ~/.local/bin/todo
	install -Dm644 style.css ~/.local/bin/style.css
	rm -f todo

uninstall:
	rm -f ~/.local/bin/todo
	rm -f ~/.local/bin/style.css
