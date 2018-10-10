#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int i;
	double x,y,v=1;
	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	srand(time(NULL));
	for(i=1;i<=300000000;++i)
	{
		x=(double)rand()/RAND_MAX*(rand()%100)+5;
		y=(double)rand()/RAND_MAX*(rand()%100)+5;
		v=v+x;
		v=v*y;
		v=v/x;
		if(i%100000000==0)printf("%d clear\n",i);
	}
	gettimeofday(&tv2,NULL);
	printf("300 million, %dms\n",tv2.tv_sec*1000+tv2.tv_usec/1000-tv1.tv_sec*1000-tv1.tv_usec/1000);
	return 0;
}
