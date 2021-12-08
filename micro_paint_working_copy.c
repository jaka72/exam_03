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
	float point_x;
	float point_y;
	float rect_xtl;
	float rect_ytl;
	float rect_xbr;
	float rect_ybr;
	float rect_width;
	float rect_height;
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
	d->rect_xbr = d->rect_xtl + d->rect_width;
	d->rect_ybr = d->rect_ytl + d->rect_height;
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

int line_is_atleast_one(float rect_point, float rect_size)
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
	if (d.width > 300 || d.width < 1|| d.height > 300 || d.height < 1)
		free_and_exit(&d, 1);
	d.arr = calloc(d.width * d.height, sizeof(char));
	if (d.arr == NULL)
		free_and_exit(&d, 1);
	while ((ret_scanf = fscanf(d.fd, "%c %f  %f %f %f %c  \n",
		&d.rect_type, &d.rect_xtl, &d.rect_ytl,
		&d.rect_width, &d.rect_height, &d.rect_char)) == 6)
	{
		//perror("D");

		get_bottom_right_point(&d);

		//printf("rect char: [%c]\n", d.rect_char);
		if (d.rect_type != 'r' && d.rect_type != 'R')
			free_and_exit(&d, 1);
		if (d.rect_width <= -0 || d.rect_height <= -0)
			free_and_exit(&d, 1);
		d.point_y = 0;
		i = 0;
		while (d.point_y < d.height)
		{
			d.point_x = 0;
			while (d.point_x < d.width)
			{
				// get_bottom_right_point(&d);
				if (d.rect_type == 'r') // EMPTY RECT
				{
					if ((d.point_x >= d.rect_xtl && d.point_x < d.rect_xtl + 1 &&
						 d.point_y > d.rect_ytl  && d.point_y <= d.rect_ybr 
						 //d.point_y >= d.rect_ytl  && d.point_y <= d.rect_ybr // also working
						 //&& (line_is_atleast_one(d.rect_xtl, d.rect_width) == 1) ) ||
						 && (line_is_atleast_one(d.rect_xtl, d.rect_width) == 1))
						 ||
						(d.point_y >= d.rect_ytl && d.point_y < d.rect_ytl + 1 &&
						 d.point_x >= d.rect_xtl && d.point_x <= d.rect_xbr
						 //d.point_x >= d.rect_xtl && d.point_x < d.rect_xbr
						 && (line_is_atleast_one(d.rect_ytl, d.rect_height) == 1) ))
						d.arr[i] = d.rect_char;
						//d.arr[i] = 'x';

					if	((d.point_x <= d.rect_xbr && d.point_x > d.rect_xbr - 1 &&
						  d.point_y <= d.rect_ybr    && d.point_y >= d.rect_ytl
						  && (line_is_atleast_one(d.rect_xtl, d.rect_width) == 1 )) ||
						(d.point_y <= d.rect_ybr && d.point_y > d.rect_ybr - 1 &&
						 //d.point_x < d.rect_xbr &&d.point_x > d.rect_xtl
						 d.point_x <= d.rect_xbr &&d.point_x >= d.rect_xtl
						 && (line_is_atleast_one(d.rect_ytl, d.rect_height) == 1) ))
					 	d.arr[i] = d.rect_char;
				}
				else if (d.rect_type == 'R') // FILLED RECT
				{
					if (d.rect_xtl <= d.point_x && d.point_x <= d.rect_xbr &&
						d.rect_ytl <= d.point_y && d.point_y <= d.rect_ybr)
						d.arr[i] = d.rect_char;
				}
				d.point_x++;
				i++;
			}
			d.point_y++;
		}
	}
	//perror("F");
	//printf("retscan %d\n", ret_scanf);

	if (ret_scanf != 6 && ret_scanf != -1)
		free_and_exit(&d, 1);
	//perror("H");


	i = 0;
	d.point_y = 0;
	while (d.point_y < d.height)
	{
		d.point_x = 0;
		while (d.point_x < d.width)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.bckg_char;
			printf("%c", d.arr[i]);			// ISSUE !
			//write(1, &d.arr[i], 1);
			d.point_x++;
			i++;
		}
		d.point_y++;
		printf("\n");						// ISSUE !
		//write(1, "\n", 1);
		//ft_putstr("\n");
	}
	fclose(d.fd);
	free(d.arr);
	exit(0);
}
