#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_data
{
	FILE *fd;
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
	char *arr;
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

void freenexit(t_data *d, int exitcode)
{
	if (exitcode == 1)
		ft_putstr("Error: Operation file corrupted\n");
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(exitcode);
}

void get_rbr_point(t_data *d)
{
	d->rbrx = d->rtlx + d->rwidth; // !!!!!!!!!!!!! left x plus width !!
	d->rbry = d->rtly + d->rheight;
}

int is_one(float rpoint, float rsize)
{
	if (rsize > 0. && rsize < 1.) // !!!!! I had the opposite !!!!! size < 0  ??? why!
	{
		if ((int)(rpoint + rsize) > (int)rpoint) // not sure ?? OK
			return (1);
		return (0);
	}
	return (1);
}


int main(int argc, char *argv[])
{
	t_data d;
	int retscan;
	int i;

	if (argc != 2)
		return (1);
	
	d.arr = NULL;

	d.fd = fopen(argv[1], "r");
	if (d.fd == NULL)
		freenexit(&d, 1);

	retscan = fscanf(d.fd, "%d %d %c \n", &d.iwidth, &d.iheight, &d.ibckg);
	if (retscan != 3)
		freenexit(&d, 1);
	if (d.iwidth > 300 || d.iwidth < 0 || d.iheight > 300 || d.iheight < 0) // !! is it really 0 ?
	{
		//printf("B\n");
		freenexit(&d, 1);													// NO! MUST BE 1 !!!!!
	}


	d.arr = calloc(d.iwidth * d.iheight, sizeof(char));
	if (d.arr == NULL)
		freenexit(&d, 1);

	//get_rbr_point(&d); // !!!!!!!!!!!! NO! THIS CAN ONLY BE AFTER 2nd SCANF !!!

	while ((retscan = fscanf(d.fd, "%c %f %f %f %f %c \n",
					&d.rtype, &d.rtlx, &d.rtly, &d.rwidth, &d.rheight, &d.rchar)) == 6)
	{
		if (d.rwidth <= -0 || d.rheight <= -0) // ?? not sure <= -0. ??? maybe does not matter
			freenexit(&d, 1);						// can also be > 0.00000000
		if (d.rtype != 'R' && d.rtype != 'r')
			freenexit(&d, 1);

		get_rbr_point(&d); // !!!!!!!!!!!! MUST BE AFTER 2nd scanf

		i = 0;
		d.fcy = 0;
		while (d.fcy < d.iheight)
		{
			d.fcx = 0;
			while (d.fcx < d.iwidth)
			{
				if (d.rtype == 'R')
				{
					// if (d.fcx >= d.rtlx && d.fcx <= d.rbrx &&
					// 	d.fcy >= d.rtly && d.fcy <= d.rbry)
					if (d.rtlx <= d.fcx && d.fcx <= d.rbrx &&
						d.rtly <= d.fcy && d.fcy <= d.rbry)
					{
						d.arr[i] = d.rchar;
						//printf("[%c]", d.rchar);
						//printf(" A \n");
					}
						//printf("i%d", i);
						//printf("[%c]", d.rchar);
				}

				if (d.rtype == 'r')
				{
					if ((d.fcx >= d.rtlx && d.fcx < d.rtlx + 1 &&
						d.fcy >= d.rtly && d.fcy <= d.rbry
						&& is_one(d.rtlx, d.rwidth))
						|| 
						(d.fcy >= d.rtly && d.fcy < d.rtly + 1 &&
						 d.fcx >= d.rtlx && d.fcx <= d.rbrx
						 && is_one(d.rtly, d.rheight)))
						d.arr[i] = d.rchar;

					if ((d.fcx <= d.rbrx && d.fcx > d.rbrx - 1 &&
						 d.fcy <= d.rbry && d.fcy >= d.rtly
						 && is_one(d.rbrx, d.rwidth))
						 ||
						 (d.fcy <= d.rbry && d.fcy > d.rbry - 1 &&
						  d.fcx <= d.rbrx && d.fcx >= d.rtlx
						  && is_one(d.rbry, d.rheight)))
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

	// STH MISSING  - NO, ONLY 1 CHECK IS HERE!

	i = 0;
	d.fcy = 0;
	while (d.fcy < d.iheight)
	{
		d.fcx = 0;
		while (d.fcx < d.iwidth)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.ibckg;
				//d.arr[i] = 'w';
			//printf("%c", d.arr[i]);
			write(1, &d.arr[i], 1);
			//	write(1, "t", 1);
			i++;
			d.fcx++;
		}
		write(1, "\n", 1);
		d.fcy++;
	}
	
	//printf("ibckg [%c]\n", d.ibckg);
	//printf("rchar [%c]\n", d.rchar);
	//printf("rwidth %.2f, rheight %.2f\n", d.rwidth, d.rheight);


	free(d.arr);
	fclose(d.fd);
	exit(0);
}

//////////////////////////////
