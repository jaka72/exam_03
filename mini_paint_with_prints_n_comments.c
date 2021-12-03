#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
	Apparently not needed to check for scan_ret -1, it never happens.


	TO BE ABLE TO USE powf AND sqrtf
	gcc mini_paint.c -std=c99 -lm 

	The last fscanf (whne nothing else to read) returns -1
		In this case just proceed to printing the array

*/

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


// void print_data(t_data *d)
// {
// 	printf("width %d, height %d, bckg '%c'\n", d->width, d->height, d->bckg_char);
// 	printf("%c, %f, %f, %f, %c\n", d->circle_type, d->center_x, d->center_y, d->radius, d->circle_char);
// }

void calc_dist_from_center(t_data *d)
{
	d->triangle_x = d->center_x - d->field_x;
	d->triangle_y = d->center_y - d->field_y;
	d->triangle_hypotenusa = sqrtf(powf(d->triangle_x, 2.0) + powf(d->triangle_y, 2.0));
}


int main(int argc, char *argv[])
{
	int i;
	int scan_ret;
	t_data d;
	
	if (argc != 2)
		return (1);

	d.fd = fopen(argv[1], "r");
	scan_ret = fscanf(d.fd, "%d   %d %c  \n", &d.width, &d.height, &d.bckg_char);
	if (scan_ret != 3)
	{
		//printf("Error, bad first scan: %d\n", scan_ret);
		printf("Error: Operation file corrupted\n");
		fclose(d.fd);
		return (1);
	}
	//printf("First scan return: %d\n", scan_ret);


	if (d.width > 300 || d.width < 1|| d.height > 300 || d.height < 1)
	{
		//printf("Error, width or height too high\n");
		printf("Error: Operation file corrupted\n");
		fclose(d.fd);
		return (1);
	}

	d.arr = calloc(d.width * d.height, sizeof(char));
	if (d.arr == NULL)
	{
		//printf("Error, malloc failed");
		printf("Error: Operation file corrupted\n");
		fclose(d.fd);
		return (1);
	}


	int ret_scanf;
	
	while ((ret_scanf = fscanf(d.fd, "%c %f  %f %f %c  \n", &d.circle_type, &d.center_x, &d.center_y,
		&d.radius, &d.circle_char)) == 5)
	{
		//printf("scanret: %d\n", ret_scanf);

		// if (ret_scanf != 5 && ret_scanf != -1) // HER PROBABLY NEVER EXECUTED
		// {
		// 	printf("Error, xxx issue with operation file, scanret: %d\n", ret_scanf);
		// 	printf("Error: Operation file corrupted\n");
		// 	if (ret_scanf == -1)
		// 	{
		// 		printf("Scanret: %d\n", ret_scanf);
		// 		return (0);
		// 	}
		// 	return (1);
		// }
		if (d.circle_type != 'c' && d.circle_type != 'C')
		{
			//printf("Error, Circle type is invalid\n");
			printf("Error: Operation file corrupted\n");
			return (1);
		}
		if (/*d.radius == 0 || */ d.radius <= -0)
		{
			//printf("Error, Radius is zero\n");
			printf("Error: Operation file corrupted\n");
			return (1);
		}
		//perror("error ");
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
					{
						//printf("%c", d.circle_char);
						d.arr[i] = d.circle_char;
					}
					//else // Do nothing, it will fill at the end, what remains empty
				}
				else if (d.circle_type == 'C') // FILLED CIRCLE
				{
					if (d.triangle_hypotenusa <= d.radius)
					{
						d.arr[i] = d.circle_char;
						//printf("%c", d.arr[i]);
					}
				}
				d.field_x++;
				i++;
			}
			//printf("\n");
			d.field_y++;
		}
		//printf("\n");
	}


	if (ret_scanf != 5 && ret_scanf != -1)
	{
		//printf("Error, xxx issue with operation file, scanret: %d\n", ret_scanf);
		printf("Error: Operation file corrupted\n");
		// if (ret_scanf == -1)
		// {
		// 	//printf("Scanret: %d\n", ret_scanf);
		// 	return (0);
		// }
		return (1);
	}

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
	return (0);
}

