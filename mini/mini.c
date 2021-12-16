#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>



typedef struct s_data
{
	FILE *fd;
	int iw;
	int ih;
	char ibckg;
	char *arr;
	char ctype;
	float ccx;
	float ccy;
	float cradius;
	char cchar;
	float fcx;
	float fcy;
	float ta;
	float tb;
	float th;

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



void get_hipoten(t_data *d)
{
	d->ta = d->ccx - d->fcx;
	d->tb = d->ccy - d->fcy;
	d->th = sqrtf(powf(d->ta, 2) + powf(d->tb, 2));
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

	//printf("a)\n");

	d.arr = NULL;

	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		fne(&d, 1);

	//printf("fd %d \n", FILE);


	retscan = fscanf(d.fd, "%d %d %c \n", &d.iw, &d.ih, &d.ibckg);
	//printf(" iw %d, ih %d, ibckg %c \n", d.iw, d.ih, d.ibckg);

	if (retscan != 3)
		fne(&d, 1);
	if (d.iw > 300 || d.iw < 1 || d.ih > 300 || d.ih < 1)  // I had wrong && !!!!!!!
		fne(&d, 1);

	//printf(" iw %d, ih %d, ibckg %c \n", d.iw, d.ih, d.ibckg);


	d.arr = malloc(d.iw * d.ih * sizeof(char));
	if (d.arr == NULL)
		fne(&d, 1);

	//printf("a) \n");


	i = 0;
	d.fcy = 0;
	while (d.fcy < d.ih)
	{
		//printf("a) i%d\n", i);

		d.fcx = 0;
		while (d.fcx < d.iw)
		{
			d.arr[i] = d.ibckg;
			//write(1, &d.arr[i], 1);
			i++;
			d.fcx++;
		}
		//write(1, "\n", 1);
		d.fcy++;
	}



	while ((retscan = fscanf(d.fd, "%c %f %f %f %c \n",
			&d.ctype, &d.ccx, &d.ccy, &d.cradius, &d.cchar)) == 5)
	{
		if (d.ctype != 'C' && d.ctype != 'c')
			fne(&d, 1);
		if (d.cradius <= 0.) //  ????????????
			fne(&d, 1);

		i = 0;
		d.fcy = 0;
		while (d.fcy < d.ih)
		{
			d.fcx = 0;
			while (d.fcx < d.iw)
			{
				get_hipoten(&d);

				if (d.ctype == 'C')
				{
					if (d.th <= d.cradius)
						d.arr[i] = d.cchar;
				}
				else if (d.ctype == 'c')
				{
					if (d.th <= d.cradius && d.th > d.cradius - 1)
						d.arr[i] = d.cchar;
				}
				i++;
				d.fcx++;
			}
			d.fcy++;
		}
	}

	if (retscan != 5 && retscan != -1)
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