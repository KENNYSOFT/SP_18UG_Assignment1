#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 800
int a[N+10][N+10],b[N+10][N+10],c[N+10][N+10];
int main()
{
	int i,j,k,t;
	struct timeval tv1,tv2;
	srand(time(NULL));
	gettimeofday(&tv1,NULL);
	for(i=1;i<=N;++i)
	{
		for(j=1;j<=N;++j)
		{
			a[i][j]=rand();
			b[i][j]=rand();
		}
	}
	for(i=1;i<=N;++i)for(j=1;j<=N;++j)for(k=1;k<=N;++k)c[i][j]=c[i][j]+a[i][k]*b[k][j];
	gettimeofday(&tv2,NULL);
	printf("%dx%d matrix multiplication completed in %d ms\n",N,N,tv2.tv_sec*1000+tv2.tv_usec/1000-tv1.tv_sec*1000-tv1.tv_usec/1000);
	return 0;
}
