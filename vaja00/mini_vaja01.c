#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


typedef struct s_data
{
	FILE *fd;
	char *arr;
	int iwidth;
	int iheight;
	char ibckg;
	char ctype;
	float ccenterx;
	float ccentery;
	float cradius;
	char cchar;
	float ta;
	float tb;
	float th;
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

void get_hipoten(t_data *d) // !!!!!!!!!!!!! first center !!!
{
	d->ta = d->ccenterx - d->fcx;
	d->tb = d->ccentery - d->fcy;
	d->th = powf(d->ta, 2) + powf(d->tb, 2);
	d->th = sqrtf(d->th);
	//printf("%.2f\n", d->th);
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
	if (d.iwidth > 300 || d.iwidth <= 0 || d.iheight > 300 || d.iheight <= 0)
		freenexit(&d, 1);

	d.arr = calloc(d.iwidth * d.iheight, sizeof(char));
	if (d.arr == NULL)
		freenexit(&d, 1);  // not sure


	while ((retscan = fscanf(d.fd, "%c %f %f %f %c \n", 
			&d.ctype, &d.ccenterx, &d.ccentery, &d.cradius, &d.cchar)) == 5)
	{
		if (d.cradius <= -0) // or maybe < 0.0000000 
			freenexit(&d, 1);
		if (d.ctype != 'c' && d.ctype != 'C')
			freenexit(&d, 1);



		i = 0;
		d.fcy = 0;
		while (d.fcy < d.iheight)
		{

			d.fcx = 0;
			while (d.fcx < d.iwidth)
			{

				get_hipoten(&d); // THIS WAS OUTSIDE LOOPS !!!

				if (d.ctype == 'C')
				{

						if (d.th <= d.cradius) // !!!!!!!!!!!! not point, but hyp!
						d.arr[i] = d.cchar;
				}
				if (d.ctype == 'c')
				{
					if (d.th <= d.cradius && d.th > d.cradius - 1)
					{
						d.arr[i] = d.cchar;
					}
				}

				i++;
				d.fcx++;
			}
			d.fcy++;
		}
	}
	
	if (retscan != 5 && retscan != -1)
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
			write(1, &d.arr[i], 1);
			i++;
			d.fcx++;
		}
		write(1, "\n", 1);
		d.fcy++;
	}


	free(d.arr);
	fclose(d.fd);
	exit(0);
}