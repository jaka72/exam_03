#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




typedef struct t_data
{
	FILE *fd;
	char *arr;
	int width;
	int height;
	char bckg;
	float px;
	float py;
	char type;
	float r_width;
	float r_height;
	float rtlx;
	float rtly;
	float rbrx;
	float rbry;
	char r_char;
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

int exit_n_free(t_data *d, int exitcode)
{
	if (exitcode == 1)
		ft_putstr("Error: Operation file corrupted\n");
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(exitcode);
}

void calc_br_point(t_data *d)
{
	d->rbrx = d->rtlx + d->r_width;
	d->rbry = d->rtly + d->r_height;
}

int line(float r_point, float r_size) // WAS OPpOSITE !!!!!
{
	if (r_size > 0. && r_size < 1.)
	{
		if ((int)(r_point + r_size) > (int)r_point)
			return (1);
		return (0);
	}
	return (1);
}


int main(int argc, char *argv[])
{
	t_data d;
	int i;
	int ret_scan;

	//perror("A");


	if (argc != 2)
		return (1); // ???? msg
	d.arr = NULL;
	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		exit_n_free(&d, 1);
	ret_scan = fscanf(d.fd, "%d %d %c\n", &d.width, &d.height, &d.bckg);
	if (ret_scan != 3)
		exit_n_free(&d, 1);
	if (d.width > 300 || d.width < 0 || d.height > 300 || d.height < 0)
		exit_n_free(&d, 1);
	d.arr = calloc(d.width * d.height, sizeof(char));
	if (d.arr == NULL)
		return (1); // ???? msg

	//perror("B");


	while ((ret_scan = fscanf(d.fd, "%c %f %f %f %f %c\n",
		&d.type, &d.rtlx, &d.rtly, &d.r_width, &d.r_height, &d.r_char)) == 6)
	{

		//perror("C");

		calc_br_point(&d);
		if (d.type != 'R' && d.type != 'r')
			exit_n_free(&d, 1);
		if (d.r_width <= -0 || d.r_height <= -0)
			exit_n_free(&d, 1);

		i = 0;
		d.py = 0;
		while (d.py < d.height)
		{
			d.px = 0;
			while (d.px < d.width)
			{
				if (d.type == 'r')
				{
					if ((d.px >= d.rtlx && d.px < d.rtlx + 1 &&
						d.py >= d.rtly && d.py <= d.rbry && (line(d.rtlx, d.r_width) == 1))
						||
						(d.py >= d.rtly && d.py < d.rtly + 1 &&
						d.px >= d.rtlx && d.px <= d.rbrx && (line(d.rtly, d.r_height)== 1)))
						d.arr[i] = d.r_char;

					if ((d.px <= d.rbrx && d.px > d.rbrx - 1 &&
						d.py <= d.rbry && d.py >= d.rtly && (line(d.rtlx, d.r_width) == 1)) // line !!!
						||
						(d.py <= d.rbry && d.py > d.rbry - 1 &&
						d.px <= d.rbrx && d.px >= d.rtlx && (line(d.rtly, d.r_height)== 1)))
						d.arr[i] = d.r_char;
				}
				else if (d.type == 'R')
				{
					if (d.px >= d.rtlx && d.px <= d.rbrx &&
						d.py >= d.rtly && d.py <= d.rbry)
						d.arr[i] = d.r_char;
				}
				d.px++;
				i++;
			}
			d.py++;
		}
	}
	//perror("F");
	//printf("retscan %d\n", ret_scan);

	if (ret_scan != 6 && ret_scan != -1)
		exit_n_free(&d, 1);

	//perror("H");

	i = 0;
	d.py = 0;
	while (d.py < d.height)
	{
		d.px = 0;
		while (d.px < d.width)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.bckg;
			write(1, &d.arr[i], 1);
			d.px++;
			i++;
		}
		write(1, "\n", 1);
			d.py++;
	}
	//perror("K");


	free(d.arr);
	fclose(d.fd);
	exit(0);
}
