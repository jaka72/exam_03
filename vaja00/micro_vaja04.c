#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


typedef struct s_data
{
	FILE *fd;
	char *arr;
	int iwidth;
	int iheight;
	char ibckg;
	char rtype;
	float rtlx;
	float rtly;
	float rbrx;
	float rbry;
	float rwidth;
	float rheight;
	char rchar;
	float fcx;
	float fcy;
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
	// write(1, str, ft_strlen(str));
	printf("%s", str);
}

void freenexit(t_data *d, int exitcode)
{
	if (exitcode == 1)
		ft_putstr("Error: Operation file corrupted\n");
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(exitcode);
}

void get_br_point(t_data *d)
{
	d->rbrx = d->rtlx + d->rwidth;
	d->rbry = d->rtly + d->rheight;
}

int isone(float rpoint, float rsize)
{
	if (rsize > 0. && rsize < 1.)
	{
		if ( (int)(rpoint + rsize) > (int)rpoint )
			return (1);
		return (0);
	}
	return (1);
}

int main(int argc, char *argv[])
{
	int i;
	int retscan;
	t_data d;

	if (argc != 2)
		return (1);
	
	d.arr = NULL;

	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		freenexit(&d, 1);

	retscan = fscanf(d.fd, "%d %d %c \n", &d.iwidth, &d.iheight, &d.ibckg);
	if (retscan != 3)
		freenexit(&d, 1);
	if (d.iwidth > 300 || d.iwidth < 0 || d.iheight > 300 || d.iheight < 0)
		freenexit(&d, 1);

	d.arr = calloc(d.iwidth * d.iheight, sizeof(char));
	if (d.arr == NULL)
		freenexit(&d, 1);  // not sure


	i = 0;
	d.fcy = 0;
	while ((retscan = fscanf(d.fd, "%c %f %f %f %f %c \n",
				&d.rtype, &d.rwidth, &d.rheight, &d.rtlx, &d.rtly, &d.rchar)) == 6)
	{
		if (d.rtype != 'R' && d.rtype != 'r')
			freenexit(&d, 1);
		if (d.rwidth <= -0. || d.rheight <= -0.)
		//if (d.rwidth < 0.00000000 || d.rheight < 0.00000000)
			freenexit(&d, 1);

		get_br_point(&d);

		i = 0;
		d.fcy = 0;
		while (d.fcy < d.iheight)
		{
			d.fcx = 0;
			while (d.fcx < d.iwidth)
			{
				if (d.rtype == 'R')
				{
					if (d.fcx >= d.rtlx && d.fcx <= d.rbrx &&
						d.fcy >= d.rtly && d.fcy >= d.rbry)
						d.arr[i] = d.rchar;
				}

				else if (d.rtype == 'r')
				{
					if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
						d.fcy >= d.rtly && d.fcy <= d.rbry &&
						isone(d.rtlx, d.rwidth)) 
						||
						(d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
						d.fcx >= d.rtlx && d.fcx <= d.rbrx &&
						isone(d.rtly, d.rheight)))
						d.arr[i] = d.rchar;

					if ((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
						d.fcy <= d.rbry && d.fcy >= d.rtly &&
						isone(d.rbrx, d.rwidth)) 
						||
						(d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
						d.fcx <= d.rbrx && d.fcx >= d.rtlx &&
						isone(d.rbry, d.rheight)))
						d.arr[i] = d.rchar;
				}
				i++;
				d.fcx++;
			}
			d.fcy++;
		}
	}


	if (retscan != 6 && retscan != -1)
		freenexit(&d, 1);

	i = 0;
	d.fcy = 0;
	while (d.fcy < d.iheight)
	{
		d.fcx = 0;
		while (d.fcx < d.iwidth)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.ibckg;
			// write(1, &d.arr[i], 1);
			printf("%c", d.arr[i]);
			i++;
			d.fcx++;
		}
		//write(1, "\n", 1);
		printf("%c", '\n');
		d.fcy++;
	}

	free(d.arr);
	fclose(d.fd);
	exit(0);
}