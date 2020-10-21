/*
В этих задачах требуется освоить ввод данных в программу 
с помощью командной строки и познакомиться с операциями 
получения информации о файлах и каталогах. Для обхода 
каталогов наиболее удобно использовать функции opendir, 
readdir, и т. д., кроме того можно использовать функцию ftw (см. man 3 opendir или
man 3 ftw ).
Реализовать программу обработки множества файлов, заданных шаблоном 
или относящихся к указанному
каталогу. Входные параметры должны задаваться в командной строке.

Для заданного каталога и его подкаталогов определить все файлы, содержащие заданное слово.
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

long find_average(FILE* file)
{
	int i = 0;
    long avg = 0;
    long x;

    for(i = 1; fscanf(file, "%ld", &x) == 1; i++) 
        avg += (x - avg) / i;

	return avg;
}

int main()
{
	struct dirent* dp;
	DIR* dir;
	FILE* file;

	if((dir = opendir(".")) == NULL) {
		printf("Cannot open folder \n");
		return -1;
	}

	while ((dp = readdir(dir)) != NULL) {
		if(!strcmp(dp->d_name, "."))
			continue;
		if(!strcmp(dp->d_name, ".."))
			continue;
		if (strstr(dp->d_name, ".txt") != NULL) {
			if((file = fopen(dp->d_name, "r")) == NULL) {
				printf("Cannot open file\n");
				continue;
			}
			printf("%s: %ld\n", dp->d_name, find_average(file));

			fclose(file);
		}
	}
	closedir(dir);
}