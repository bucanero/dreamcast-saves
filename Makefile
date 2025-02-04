# Markdown Generator
# (c)2020 Damian Parrino

all: generator

generator:
	gcc -D _GNU_SOURCE -o markdown gen_markdown.c
	@echo "Generating markdown files..."
	./markdown > README.md
	@echo "Done!\n"

clean:
	-rm -f markdown
