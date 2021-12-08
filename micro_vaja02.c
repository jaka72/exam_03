#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data
{
	FILE *fd;
	int iwidth;
	int iheight;
	char ibckg;
	float fcx;
	float fcy;
	char rtype;
	float rtlx;
	float rtly;
	float rbrx;
	float rbry;
	float rwidth;
	float rheight;
	char rchar;
	char *arr;
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

void free_n_exit(t_data *d, int exitcode)
{
	if (exitcode == 1)
//		write(1, "Error: Operation file corrupted\n", 33);
		ft_putstr("Error: Operation file corrupted\n");
	if (d->arr != NULL)	
		free(d->arr); // !!!!!!!!!!!!!!!!!!! FORGOT TO CHECK 
	fclose(d->fd);
	exit(exitcode);
}

void get_br_corner(t_data *d)
{
	d->rbrx = d->rtlx + d->rwidth;
	d->rbry = d->rtly + d->rheight;
}

int check_if_one(float rpoint, float rsize)
{
	if (rsize < 1. && rsize > 0.) // !!!!!!!!!!!!!!!!!!!!!!!
	{
		if ((int)(rpoint + rsize) > (int)rpoint)
			return (1);
		return (0);
	}
	return (1);
}


int main(int argc, char *argv[])
{
	int i;
	int ret_scan;
	t_data d;

	if (argc != 2)
		return (1);
	d.fd = fopen(argv[1], "r"); // !!!!!!!!!!!!!!!!!!!!!!! FORGOT
	if (d.fd == NULL)
		free_n_exit(&d, 1);

	d.arr = NULL;
	ret_scan = fscanf(d.fd, "%d %d %c \n", &d.iwidth, &d.iheight, &d.ibckg);
	if (ret_scan != 3)
		free_n_exit(&d, 1);
	if (d.iwidth > 300 || d.iwidth < 0 || d.iheight > 300 || d.iheight < 0) // !!!!!!!!!!!!!!!! less then 1 !!!
		free_n_exit(&d, 1);
	d.arr = calloc(d.iwidth * d.iheight, sizeof(char));
	if (d.arr == NULL)
		free_n_exit(&d, 1);

	// !!!!!!!!!!!!!!!!!!!! FORGOT & prefixes
	while ((ret_scan = fscanf(d.fd, "%c %f %f %f %f %c \n", &d.rtype, &d.rtlx, &d.rtly, &d.rwidth, &d.rheight, &d.rchar)) == 6)
	{
		if (d.rtype != 'r' && d.rtype != 'R')
			free_n_exit(&d, 1);
		if (d.rwidth <= -0. || d.rheight <= -0.) // !!!!!!!!!!!!!!!!!!!!!!!!!!! <= !!!
			free_n_exit(&d, 1);				// !!!!! ALSO, NOT TL/BR, BUT WIDTH/HEIGHT !!!

		get_br_corner(&d);

		i = 0;
		d.fcy = 0;
		while (d.fcy < d.iheight)
		{
			d.fcx = 0;
			while (d.fcx < d.iwidth)
			{

				if (d.rtype == 'R')
				{
					if (d.fcx >= d.rtlx && d.fcy >= d.rtly && d.fcx <= d.rbrx && d.fcy <= d.rbry)
						d.arr[i] = d.rchar;
				}
				if (d.rtype == 'r')
				{
					if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
						 d.fcy >= d.rtly && d.fcy <= d.rbry
						 && check_if_one(d.rtlx, d.rwidth)) // !!!!!!!!!!!! 1st ARG NOT fcx, BUT rtlx !!! 
						|| 
						(d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
						 d.fcx >= d.rtlx && d.fcx <= d.rbrx
						 && check_if_one(d.rtly, d.rheight)))
						d.arr[i] = d.rchar;

					if ((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
						 d.fcy <= d.rbry && d.fcy >= d.rtly
						 && check_if_one(d.rtlx, d.rwidth))
						||
						(d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
						 d.fcx <= d.rbrx && d.fcx >= d.rtlx
						 && check_if_one(d.rtly, d.rheight))   )
						d.arr[i] = d.rchar;

					// if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
					// 	 d.fcy >= d.rtly  && d.fcy <= d.rbry 
					// 	 && (check_if_one(d.rtlx, d.rwidth) == 1))
					// 	 ||
					// 	(d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
					// 	 d.fcx >= d.rtlx && d.fcx <= d.rbrx
					// 	 && (check_if_one(d.rtly, d.rheight) == 1) ))
					// 	d.arr[i] = d.rchar;

					// if	((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
					// 	  d.fcy <= d.rbry    && d.fcy >= d.rtly
					// 	  && (check_if_one(d.rtlx, d.rwidth) == 1 )) ||
					// 	(d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
					// 	 d.fcx <= d.rbrx &&d.fcx >= d.rtlx
					// 	 && (check_if_one(d.rtly, d.rheight) == 1) ))
					//  	d.arr[i] = d.rchar;
				}

				i++;
				d.fcx++;
			}
			d.fcy++;
		}
	}

	if (ret_scan != 6 && ret_scan != -1)
		free_n_exit(&d, 1);


	i = 0;
	d.fcy = 0;
	while (d.fcy < d.iheight)
	{
		d.fcx = 0;
		while (d.fcx < d.iwidth)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.ibckg;
			write(1, &d.arr[i], 1);
			//ft_putstr(&d.arr[i]);
			i++;
			d.fcx++; 	// !!!!!!!!!!!!!!!!!!!! HAD iwidth INSTEAD OF fcx
		}
		//write(1, "\n", 1);
		ft_putstr("\n");
		d.fcy++; 	// !!!!!!!!!!!!!!!!!!!! HAD iheight INSTEAD OF fcy
	}



	free(d.arr);
	fclose(d.fd);
	exit(0); // OR exitnfree(&D)
}




















































