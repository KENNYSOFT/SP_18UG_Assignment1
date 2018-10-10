#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	unsigned long long i,cnt=0;
	double x,y,z,pi;
	struct timeval tv1,tv2;
	srand(time(NULL));
	gettimeofday(&tv1,NULL);
	for(i=1;i<=1000000000;++i)
	{
		x=(double)rand()/RAND_MAX;
		y=(double)rand()/RAND_MAX;
		z=x*x+y*y;
		if(z<=1)cnt++;
		if(i%100000000==0)
		{
			pi=(double)cnt/i*4;
			printf("%d tries, estimation of pi = %.10lf\n",i,pi);
		}
	}
	gettimeofday(&tv2,NULL);
	printf("1 billion tries, total time %dms\n",tv2.tv_sec*1000+tv2.tv_usec/1000-tv1.tv_sec*1000-tv1.tv_usec/1000);
	return 0;
}
