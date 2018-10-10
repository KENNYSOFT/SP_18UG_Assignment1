#include<stdio.h>
#include<stdlib.h>
#include<time.h>
unsigned int a=1,b=1,c;
int main()
{
	unsigned long long i;
	struct timeval tv1,tv2;
	gettimeofday(&tv1,NULL);
	for(i=1;i<=1000000000;++i)
	{
		c=a+b;
		a=b;
		b=c;
		if(i%100000000==0)
		{
			printf("%lld th fibonacci number modulo 4294967296 is %u\n",i,c);
		}
	}
	gettimeofday(&tv2,NULL);
	printf("one billion, elapsed time %dms\n",tv2.tv_sec*1000+tv2.tv_usec/1000-tv1.tv_sec*1000-tv1.tv_usec/1000);
	return 0;
}
