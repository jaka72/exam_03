

typedef struct s_data
{
	FILE *fd;
	int iwidth;
	int iheight;
	char ibckgr;

	char ctype;
	float ccenterx;
	float ccentery;
	float radius;
	char cchar;
	char *arr;

	float fcx;
	float fcy;

	float ta;
	float tb;
	float th;



} t_data;


void calc_dist_from_center(t_data *d)
{
	d->ta = d->ccenterx - d->fcx;
	d->tb = d->ccentery - d->fcy;
	d->th = powf(d->ta, 2) + powf(d->ta, 2);
	d->th = sqrtf(d->th);
}

int main()
{
	int i;
	t_data d;

	calc_dist_from_center(&d);
	if (d.ctype == 'C')
	{
		if (d.th <= d.radius)
			d.arr[i] = d.cchar;
	}
	else if (d.ctype == 'c')
	{
		if (d.th <= d.radius && d.th > d.radius - 1)
			d.arr[i] = d.cchar;
	}
}