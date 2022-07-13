#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct Args
{
    char *a;
    int n0;
    int offset;
    int n_local;
    int first;
    struct timespec start, stop, ex_time;
    int count;
} arguments;

void *eratosthenes(void *arg)
{
    clock_gettime(CLOCK_MONOTONIC, &(((arguments*)arg)->start));
    int i, j, jnxt, k;

    jnxt = ((arguments*)arg)->offset + ((arguments*)arg)->n_local;

    for (i = 2; i < ((arguments*)arg)->n0; ++i)
    {
        if ( ((arguments*)arg)->a[i])
        {
            k = ((arguments*)arg)->offset;
            if(!k) break;
            if (k % i)
            {
                k = k - (k % i) + i;
            }
            for (j = k; j < jnxt; j+=i)
            {
                if ( ((arguments*)arg)->a[j] )
                {
                    if (j >= ((arguments*)arg)->first) ++((arguments*)arg)->count;
                    ((arguments*)arg)->a[j] = 0;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &(((arguments*)arg)->stop));
    ((arguments*)arg)->ex_time.tv_nsec = ((arguments*)arg)->stop.tv_nsec - ((arguments*)arg)->start.tv_nsec;
    ((arguments*)arg)->ex_time.tv_sec = ((arguments*)arg)->stop.tv_sec - ((arguments*)arg)->start.tv_sec;
    pthread_exit(NULL);
}


int main(int argc, char **argv)
{

    int first = atoi(argv[1]);
    int last = atoi(argv[2]);
    char *a;
    int n_global = last + 1;
    int nproc = atoi(argv[3]);
    int n0, n_local;
    int count_global;
    double maxtime, sumtime;
    arguments *arg;
    FILE *out, *t;

    int i, j;

    pthread_t *tid;
    tid = (pthread_t *)malloc(nproc * sizeof(pthread_t));
    arg = (arguments *)malloc(nproc * sizeof(arguments));

    a = (char*)malloc(n_global * sizeof(char));
    memset (a, 1, n_global * sizeof(char));
    a[0] = a[1] = 0;

    n0 = round(sqrt(n_global*1.));
    
    clock_gettime(CLOCK_MONOTONIC, &(arg[0].start));
    arg[0].a = a;
    arg[0].n0 = n0;
    arg[0].offset = 0;
    arg[0].n_local = n0;
    arg[0].first = first;
    arg[0].count = 0;   
    if (nproc > 1)
    {
        if (n0 > first) n_local = (n_global - n0 - 1) / (nproc - 1) + 1;
        else n_local = (n_global - first - 1)/(nproc - 1) + 1;
        for (i = 2; i * i < n0; ++i)
        {
            if (a[i])
            {
                for (j = i*i; j < n0; j += i)
                {
                    if (a[j])
                    {
                        if (j >= first) ++arg[0].count;
                        a[j] = 0;
                    }
                }
            }
        }

        for (i = 1; i < nproc; ++i)
        {
            arg[i].a = a;
            arg[i].n0 = n0;
            if (n0 > first) arg[i].offset = n0 + (i - 1) * n_local;
            else arg[i].offset = first + (i - 1) * n_local;
            arg[i].first = first;
            if (arg[i].offset < n_global - n_local - 1)
            {
                arg[i].n_local = n_local;
            }
            else
                if (arg[i].offset < n_global)
                {
                    arg[i].n_local = n_global - arg[i].offset;
                }
                else
                {
                    arg[i].n_local = 0;
                    arg[i].offset = 0;
                }
            arg[i].count = 0;        
            if (pthread_create(&(tid[i]), NULL, eratosthenes, &(arg[i])))
            {
                printf("Error: thread %d is not created\n", i);
                return EXIT_FAILURE;
            }
        }
    }
    else
    {
        for (i = 2; i*i < n_global; ++i)
        {
            if (a[i])
            {
                for (j = i*i; j < n_global; j += i)
                {
                    if (a[j])
                    {
                        if (j >= first) ++arg[0].count;
                        a[j] = 0;
                    }
                }
            }
        }
    }
    
    

    count_global = n_global - first - arg[0].count;
    if (!first) count_global-=2;
    if (first==1) count_global--;

    for (i = 1; i < nproc; ++i)
    {
        if (pthread_join(tid[i], NULL))
        {
            printf("Error: thread %d has not finished\n", i);
            return EXIT_FAILURE;
        }
        count_global -= arg[i].count;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &(arg[0].stop));
    arg[0].ex_time.tv_nsec = arg[0].stop.tv_nsec - arg[0].start.tv_nsec;
    arg[0].ex_time.tv_sec = arg[0].stop.tv_sec - arg[0].start.tv_sec;
    maxtime = arg[0].ex_time.tv_nsec * 0.000001;
    sumtime = 0;
    for (i = 0; i < nproc; ++i)
    {
        sumtime += arg[i].ex_time.tv_nsec * 0.000001;
        
    }    

    out = fopen(argv[4], "w");
	if (!out) 
	{
    	    printf("error: file is not opened \n");
    	    exit (-1);
	}
	printf ("%d numbers \n", count_global);
	for (i = first; i < n_global; ++i)
    {
    	    if (a[i]) 
    	    {
    		    fprintf(out, "%d ", i);
    		    if(! (i % 10)) fprintf(out, "\n");
    	    }
    }
    fclose(out);

    free(tid);
    free(arg);
    free(a);
    return 0;
}