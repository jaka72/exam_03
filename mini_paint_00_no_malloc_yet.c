#include <stdio.h>
#include <math.h>


typedef struct t_data
{
	FILE *fd;
	int width;
	int height;
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


void print_data(t_data *d)
{
	printf("width %d, height %d, bckg '%c'\n", d->width, d->height, d->bckg_char);
	printf("%c, %f, %f, %f, %c\n", d->circle_type, d->center_x, d->center_y, d->radius, d->circle_char);
}

void calc_dist_from_center(t_data *d)
{
	d->triangle_x = d->center_x - d->field_x;
	d->triangle_y = d->center_y - d->field_y;
	d->triangle_hypotenusa = sqrtf(powf(d->triangle_x, 2.0) + powf(d->triangle_y, 2.0));
	//printf("hypo %f\n", d->triangle_hypotenusa);
}


int is_in_circle(t_data *d, float field_x)
{
	float triangle_x, triangle_y, triangle_hypotenusa;

	if (field_x < 0 || field_x > d->width + 1)
	{
		//printf("|%.1f", field_x);
		return (0);
	}
	triangle_x = d->center_x - field_x;
	triangle_y = d->center_y - d->field_y;
	triangle_hypotenusa = sqrtf(powf(triangle_x, 2.0) + powf(triangle_y, 2.0));
	if (triangle_hypotenusa < d->radius + 1)
	{
		//printf("|");
		return (1);
	}
	//printf("X");
	return (0);
}


int main(int argc, char *argv[])
{
	//int temp;
	//int i, j;
	t_data d;
	
	if (argc != 2)
		return (1);

	d.fd = fopen(argv[1], "r");
	//d.fd = fopen("map_10x10.txt", "r");
	//d.fd = fopen("map_example2.txt", "r");
	fscanf(d.fd, "%d   %d %c  \n", &d.width, &d.height, &d.bckg_char);

	//temp = 0;
	while (fscanf(d.fd, "%c %f   %f %f %c  \n", &d.circle_type, &d.center_x, &d.center_y,
		&d.radius, &d.circle_char) == 5)
	{
		//printf("temp: %d\n", temp);
		//perror("error ");
		//print_data(&d);
		d.field_y = 0;
		while (d.field_y < d.height)
		{
			d.field_x = 0;
			while (d.field_x < d.width)
			{
				calc_dist_from_center(&d);
				if (d.circle_type == 'c') // EMPTY CIRCLE
				{
					if (d.triangle_hypotenusa <= d.radius)
					{
						// if (is_in_circle(&d, d.field_x - 1.) && is_in_circle(&d, d.field_x + 1.))
						// 	printf("%c", d.bckg_char);
						// else if  (is_in_circle(&d, d.field_x) && d.field_x == 0)
						// 	printf("%c", d.bckg_char);
						// else if  (is_in_circle(&d, d.field_x) && d.field_x == d.width - 1.)
						// 	printf("%c", d.bckg_char);
						// else
							printf("%c", d.circle_char);
					}
					else
						printf("%c", d.bckg_char);
				}
				else if (d.circle_type == 'C') // FILLED CIRCLE
				{
					if (d.triangle_hypotenusa <= d.radius)
						printf("%c", d.circle_char);
					else
						printf("%c", d.bckg_char);
				}
				d.field_x++;
			}
			printf("\n");
			d.field_y++;
		}
		//temp++;
		printf("\n");

	}
	fclose(d.fd);

	
	return (0);
}