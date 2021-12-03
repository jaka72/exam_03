#include <stdio.h>
#include <stdlib.h>

int main()
{
	int width, height;
	char	bckg;
	int res_fscan;
	FILE *file;

	file = fopen("map_5x3.txt", "r");
	res_fscan = fscanf(file, "%d %d %c", &width, &height, &bckg);

	printf("res scan: %d\n", res_fscan);
	printf("bckg: %c\n", bckg);
	return (0);
}