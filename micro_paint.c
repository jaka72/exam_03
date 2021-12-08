#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/*
	WIth write() it gets very slow ???

	Also, using write() to print error message, it causes some kind of error ???

*/






typedef struct t_data
{
	FILE *fd;
	int width;
	int height;
	char *arr;
	char bckg_char;
	char rect_char;
	char rect_type;
	float fcx;
	float fcy;
	float rtlx;
	float rtly;
	float rbrx;
	float rbry;
	float rwidth;
	float rheight;
} t_data;

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void get_bottom_right_point(t_data *d)
{
	d->rbrx = d->rtlx + d->rwidth;
	d->rbry = d->rtly + d->rheight;
}

void free_and_exit(t_data *d, int exitcode)
{
	if (exitcode == 1)
		//printf("Error: Operation file corrupted\n");
		ft_putstr("Error: Operation file corrupted\n");
		//write(1, "Error: Operation file corrupted\n", 33);  // NOT WORKING ???
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(exitcode);
}

int check_if_one(float rect_point, float rect_size)
{
	if (rect_size > 0. && rect_size < 1.)
	{
		//if (floor(rect_point + rect_size) > floor(rect_point))
		if ((int)(rect_point + rect_size) > (int)rect_point)
			return (1);
		return (0);
	}
	return (1);
}

int main(int argc, char *argv[])
{
	int i;
	int ret_scanf;
	t_data d;

	//perror("A");

		//write(1, "Error: Operation file corrupted\n", 33);


	if (argc != 2)
		return (1);
	d.arr = NULL;
	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		free_and_exit(&d, 1);
	ret_scanf = fscanf(d.fd, "%d   %d %c  \n", &d.width, &d.height, &d.bckg_char);
	if (ret_scanf != 3)
		free_and_exit(&d, 1);
	if (d.width > 300 || d.width < 0|| d.height > 300 || d.height < 0)
		free_and_exit(&d, 1);
	d.arr = calloc(d.width * d.height, sizeof(char));
	if (d.arr == NULL)
		free_and_exit(&d, 1);
	while ((ret_scanf = fscanf(d.fd, "%c %f  %f %f %f %c  \n",
		&d.rect_type, &d.rtlx, &d.rtly,
		&d.rwidth, &d.rheight, &d.rect_char)) == 6)
	{
		//perror("D");

		get_bottom_right_point(&d);

		//printf("rect char: [%c]\n", d.rect_char);
		if (d.rect_type != 'r' && d.rect_type != 'R')
			free_and_exit(&d, 1);
		if (d.rwidth <= -0 || d.rheight <= -0)
			free_and_exit(&d, 1);
		d.fcy = 0;
		i = 0;
		while (d.fcy < d.height)
		{
			d.fcx = 0;
			while (d.fcx < d.width)
			{
				// get_bottom_right_point(&d);
				if (d.rect_type == 'r') // EMPTY RECT
				{
					if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
						 d.fcy >= d.rtly  && d.fcy <= d.rbry 
						 && (check_if_one(d.rtlx, d.rwidth) == 1))
						 ||
						(d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
						 d.fcx >= d.rtlx && d.fcx <= d.rbrx
						 && (check_if_one(d.rtly, d.rheight) == 1) ))
						d.arr[i] = d.rect_char;

					if	((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
						  d.fcy <= d.rbry    && d.fcy >= d.rtly
						  && (check_if_one(d.rtlx, d.rwidth) == 1 )) ||
						(d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
						 d.fcx <= d.rbrx && d.fcx >= d.rtlx
						 && (check_if_one(d.rtly, d.rheight) == 1) ))
					 	d.arr[i] = d.rect_char;
				}
				else if (d.rect_type == 'R') // FILLED RECT
				{
					if (d.rtlx <= d.fcx && d.fcx <= d.rbrx &&
						d.rtly <= d.fcy && d.fcy <= d.rbry)
						d.arr[i] = d.rect_char;
				}
				d.fcx++;
				i++;
			}
			d.fcy++;
		}
	}
	//perror("F");
	//printf("retscan %d\n", ret_scanf);

	if (ret_scanf != 6 && ret_scanf != -1)
		free_and_exit(&d, 1);
	//perror("H");


	i = 0;
	d.fcy = 0;
	while (d.fcy < d.height)
	{
		d.fcx = 0;
		while (d.fcx < d.width)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.bckg_char;
			printf("%c", d.arr[i]);			// ISSUE !
			//write(1, &d.arr[i], 1);
			d.fcx++;
			i++;
		}
		d.fcy++;
		printf("\n");						// ISSUE !
		//write(1, "\n", 1);
		//ft_putstr("\n");
	}
	fclose(d.fd);
	free(d.arr);
	exit(0);
}
