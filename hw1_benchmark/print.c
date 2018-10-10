#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int i;
	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	for(i=1;i<=300000000;++i)
	{
		fprintf(stdout,"%d\n",i);
		if(i%100000000==0)fprintf(stderr,"%d cleared\n",i);
	}
	gettimeofday(&tv2,NULL);
	fprintf(stderr,"300 million, total time %dms\n",tv2.tv_sec*1000+tv2.tv_usec/1000-tv1.tv_sec*1000-tv1.tv_usec/1000);
	return 0;
}
