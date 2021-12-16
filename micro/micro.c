#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data
{
	FILE *fd;
	int iw;
	int ih;
	char ibckg;
	char *arr;
	char rtype;
	float rtlx;
	float rtly;
	float rbrx;
	float rbry;
	float rw;
	float rh;
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
	write(1, str, ft_strlen(str));
}

void fne(t_data *d, int ec)
{
	if (ec == 1)
		ft_putstr("Error: Operation file corrupted\n");
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(ec);
}





void getbr(t_data *d)
{
	d->rbrx = d->rtlx + d->rw;
	d->rbry = d->rtly + d->rh; // !!!!!!!  I had it wrong !!!
}


int isone(float rpoint, float rsize)
{
	if (rsize > 0 && rsize < 1 && (rpoint - (int)rpoint != 0))
	{
		if ((int)(rsize + rpoint) > (int)rpoint)
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
	{
		ft_putstr("Error: Argument\n");
		return (1);
	}


	d.arr = NULL;

	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		fne(&d, 1);

	printf("%d\n", d.fd);

	retscan = fscanf(d.fd, "%d %d %c \n", &d.iw, &d.ih, &d.ibckg);
	if (retscan != 3)
		fne(&d, 1);
	if (d.iw > 300 || d.iw < 1 || d.ih > 300 || d.ih < 1)  // I had wrong && !!!!!!!
		fne(&d, 1);



	d.arr = malloc(d.iw * d.ih * sizeof(char));
	if (d.arr == NULL)
		fne(&d, 1);



	i = 0;
	d.fcy = 0;
	while (d.fcy < d.ih)
	{

		d.fcx = 0;
		while (d.fcx < d.iw)
		{
			d.arr[i] = d.ibckg;
			i++;
			d.fcx++;
		}
		d.fcy++;
	}



	while ((retscan = fscanf(d.fd, "%c %f %f %f %f %c \n",
			&d.rtype, &d.rtlx, &d.rtly, &d.rw, &d.rh, &d.rchar)) == 6)
	{
		if (d.rtype != 'R' && d.rtype != 'r')
			fne(&d, 1);
		if (d.rw <= 0. || d.rh <= 0.) //  ????????????
			fne(&d, 1);

		getbr(&d);

		i = 0;
		d.fcy = 0;
		while (d.fcy < d.ih)
		{
			d.fcx = 0;
			while (d.fcx < d.iw)
			{
				if (d.rtype == 'R')
				{
					if (d.fcx >= d.rtlx && d.fcx <= d.rbrx && // ?????? <= ??
						d.fcy >= d.rtly && d.fcy <= d.rbry)
						d.arr[i] = d.rchar;
				}
				else if (d.rtype == 'r')
				{
					if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
						 d.fcy >= d.rtly && d.fcy <= d.rbry &&
						 isone(d.rtlx, d.rw))
						 ||
						 (d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
						  d.fcx >= d.rtlx && d.fcx <= d.rbrx &&
						  isone(d.rtly, d.rh))) // not br, but has to be tl always!
						d.arr[i] = d.rchar;

					if ((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
						 d.fcy <= d.rbry && d.fcy >= d.rtly &&
						 isone(d.rtlx, d.rw))
						 ||
						 (d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
						  d.fcx <= d.rbrx && d.fcx >= d.rtlx &&
						  isone(d.rtly, d.rh)))
						d.arr[i] = d.rchar;						
				}
				i++;
				d.fcx++;
			}
			d.fcy++;
		}
	}

	if (retscan != 6 && retscan != -1)
		fne(&d, 1);

	i = 0;
	d.fcy = 0;
	while (d.fcy < d.ih)
	{
		d.fcx = 0;
		while (d.fcx < d.iw)
		{
			write(1, &d.arr[i], 1);
			i++;
			d.fcx++;
		}
		write(1, "\n", 1);
		d.fcy++;
	}


	fne(&d, 0);
}