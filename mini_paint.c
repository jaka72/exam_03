#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct t_data
{
	FILE *fd;
	int width;
	int height;
	char *arr;
	char bckg_char;
	char circle_char;
	char circle_type;
	float field_x;
	float field_y;
	float center_x;
	float center_y;
	float radius;
	float triangle_x;
	float triangle_y;
	float triangle_hypotenusa;
} t_data;

void calc_dist_from_center(t_data *d)
{
	d->triangle_x = d->center_x - d->field_x;
	d->triangle_y = d->center_y - d->field_y;
	d->triangle_hypotenusa = sqrtf(powf(d->triangle_x, 2.0) + powf(d->triangle_y, 2.0));
}

void free_and_exit(t_data *d, int exitcode)
{
	if (exitcode == 1)
		printf("Error: Operation file corrupted\n");
	if (d->arr != NULL)
		free(d->arr);
	fclose(d->fd);
	exit(exitcode);
}

int main(int argc, char *argv[])
{
	int i;
	int ret_scanf;
	t_data d;
	
	if (argc != 2)
		return (1);
	d.arr = NULL;
	d.fd = fopen(argv[1], "r");
	ret_scanf = fscanf(d.fd, "%d   %d %c  \n", &d.width, &d.height, &d.bckg_char);
	if (ret_scanf != 3)
		free_and_exit(&d, 1);
	if (d.width > 300 || d.width < 1|| d.height > 300 || d.height < 1)
		free_and_exit(&d, 1);
	d.arr = calloc(d.width * d.height, sizeof(char));
	if (d.arr == NULL)
		free_and_exit(&d, 1);
	while ((ret_scanf = fscanf(d.fd, "%c %f  %f %f %c  \n", &d.circle_type, &d.center_x, &d.center_y,
		&d.radius, &d.circle_char)) == 5)
	{
		if (d.circle_type != 'c' && d.circle_type != 'C')
			free_and_exit(&d, 1);
		if (d.radius <= -0)
			free_and_exit(&d, 1);
		d.field_y = 0;
		i = 0;
		while (d.field_y < d.height)
		{
			d.field_x = 0;
			while (d.field_x < d.width)
			{
				calc_dist_from_center(&d);
				if (d.circle_type == 'c') // EMPTY CIRCLE
				{
					if (d.triangle_hypotenusa <= d.radius && d.triangle_hypotenusa > d.radius - 1)
						d.arr[i] = d.circle_char;
				}
				else if (d.circle_type == 'C') // FILLED CIRCLE
				{
					if (d.triangle_hypotenusa <= d.radius)
						d.arr[i] = d.circle_char;
				}
				d.field_x++;
				i++;
			}
			d.field_y++;
		}
	}
	if (ret_scanf != 5 && ret_scanf != -1)
		free_and_exit(&d, 1);
	i = 0;
	d.field_y = 0;
	while (d.field_y < d.height)
	{
		d.field_x = 0;
		while (d.field_x < d.width)
		{
			if (d.arr[i] == 0)
				d.arr[i] = d.bckg_char;
			printf("%c", d.arr[i]);
			d.field_x++;
			i++;
		}
		d.field_y++;
		printf("\n");
	}
	fclose(d.fd);
	free(d.arr);
	exit(0);
}

