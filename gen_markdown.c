#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SAVE_TYPE   "SEGA Dreamcast"
#define SAVE_FOLDER "."

char * readFile(const char * path, char* string)
{
	FILE *f = fopen(path, "rb");

	if (!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (fsize <= 0)
	{
		fclose(f);
		return NULL;
	}

	fread(string, fsize, 1, f);
	fclose(f);

	return string;
}

void save_main(char* path, char* title)
{
    FILE * fp;
    FILE * out;
    char * line = NULL;
    char * pos;
    size_t len = 0;
    ssize_t read;
    char vmi[200];

    chdir(path);
    fp = fopen("SAVELIST.TXT", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    out = fopen("README.md", "w");

    fprintf(out, "---\nlayout: default\ntitle: \"%s\"\nparent: " SAVE_TYPE " Saves\npermalink: " SAVE_FOLDER "/%s/\n---\n", title, path);
    fprintf(out, "# %s\n\n## VMU Saves\n\n| Icon | Filename | VMI | VMS | Description |\n|------|----------|-----|-----|-------------|\n", title);

    while ((read = getline(&line, &len, fp)) != -1) {
		line[read-1] = 0;
		line[read-2] = 0;
		pos = strchr(line, '=');
		*pos = 0;
		
		readFile(line, vmi);
		char *tmp = strchr(vmi+88, 0x20);
		if (tmp) *tmp = 0;
        fprintf(out, "| ![%s](../icons/%s.GIF) | `%s` | [%s](%s) ", title, vmi+88, vmi+88, line, line);
        *strrchr(line, '.') = 0;
        fprintf(out, "| [%s.VMS](%s.VMS) | %s |\n", line, line, ++pos);
    }

    fclose(out);
    fclose(fp);
    if (line)
        free(line);

    chdir("..");
}

int main(int argc, char** argv)
{
    FILE * fp;
    char * line = NULL;
    char * pos;
    size_t len = 0;
    ssize_t read;

    fp = fopen("GAMELIST.TXT", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    printf("---\nlayout: default\ntitle: " SAVE_TYPE " Saves\npermalink: " SAVE_FOLDER "/\nhas_children: true\nhas_toc: false\n---\n");
    printf("# Dreamcast Games\n\n");

    while ((read = getline(&line, &len, fp)) != -1) {
		line[read-1] = 0;
		pos = strchr(line, '=');
		*pos = 0;
        printf("- [%s](%s/)\n", ++pos, line);
        save_main(line, pos);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}
