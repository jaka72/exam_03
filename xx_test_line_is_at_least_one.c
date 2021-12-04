#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


int line_is_atleast_one(float rect_point, float rect_size)
{
	if (rect_size > 0. && rect_size < 1.)
	{
		//if (floor(rect_point + rect_size) > floor(rect_point))
		if ((int)(rect_point + rect_size) > (int)(rect_point))
			return (1);
		return (0);
	}
	return (1);
}



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

typedef struct s_data
{
	char arr[10];
} t_data;


int main(void)
{
	// float a;
	// float b;
	// int c;

	//char arr[10];

	// a = 3.65;
	// b = 0.35;
	// c = 11;
	// int res; 

	t_data d;

	d.arr[0] = 'X';
	d.arr[1] = 'Y';
	d.arr[2] = 'Z';

	// printf("floor a =  %f\n", floor(a));
	// printf("a + c = %f\n", a + c);
	// printf("floor(a + b) = %f\n", floor(a + b));
	// printf("floor(a + b) > floor(a) = %d\n", floor(a + b) > floor(a));

	//res = line_is_atleast_one(a, b);
	//printf("res %d\n", res);

	ft_putstr("bla vla flad aodsoakdjasd\n");


	write(1, &d.arr[0], 1);
	write(1, "\n", 1);
		write(1, "Error: Operation file corrupted\n", 33);


	return (0);
}