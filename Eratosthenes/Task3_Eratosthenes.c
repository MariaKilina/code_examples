#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <math.h>

int main(int argc, char **argv)
{
    int first = atoi(argv[1]);
    int last = atoi(argv[2]);
    char *a;
    int n_global = last + 1;
    int n0, n_local;

    double start, stop;
    double maxtime, sumtime, recvtime, sendtime;

    int size, myrank;
    MPI_Status status;

    FILE *out, *t;

    int i, j, k;

    int *recvcounts, *displs;
    
    int count_global, count_local;

    a = (char*) malloc (n_global * sizeof(char));
    memset (a, 1, n_global * sizeof(char));
    a[0] = a[1] = 0;

    n0 = round(sqrt(n_global));

    MPI_Init(&argc, &argv);
    
    start = MPI_Wtime();
    
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (n0 > first) n_local = (n_global - n0 - 1) / (size - 1) + 1;
    else n_local = (n_global - first - 1)/(size - 1) + 1;
    recvcounts = (int*) malloc (size * sizeof(int));
    displs = (int*) malloc (size * sizeof(int));

    

    if (size > 1)
    {
        if (!myrank)
        {
    	    count_global = last - first + 1;
    	    if (!first) count_global-=2;
    	    if (first==1) count_global--;
    	    count_local = 0;
            recvcounts[0] = displs[0] = 0;
            MPI_Gather(MPI_IN_PLACE, 1, MPI_INT, displs, 1, MPI_INT, 0, MPI_COMM_WORLD);
             MPI_Gather(MPI_IN_PLACE, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(displs, size, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(recvcounts, size, MPI_INT, 0, MPI_COMM_WORLD);
            
            for (i = 2; i * i < n0; ++i)
            {
                if (a[i])
                    for (j = i * i; j < n0; j += i)
                    {
                        if (a[j])
                        {
                    	    if (j >= first) ++count_local;
                    	    a[j] = 0;
                    	}
                    }
            }

            MPI_Bcast(a, n0, MPI_CHAR, 0, MPI_COMM_WORLD);
            count_global -= count_local;
            MPI_Gatherv(MPI_IN_PLACE, n0, MPI_CHAR, a, recvcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);
    	    for (i = 1; i < size; ++i)
    	    {
    		    MPI_Recv(&count_local, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		        count_global -= count_local;
	        }
	 
        }
        else
        {
    	     count_local = 0;
             if (n0 > first) displs[myrank] = n0 + (myrank - 1) * n_local;
             else displs[myrank] = first + (myrank - 1) * n_local;
             if (displs[myrank] < n_global-n_local-1) recvcounts[myrank] = n_local;
             else if (displs[myrank] < n_global) recvcounts[myrank] = n_global - displs[myrank];
             else {displs[myrank] = 0; recvcounts[myrank] = 0;}
             
             MPI_Gather(displs + myrank, 1, MPI_INT, displs + myrank, 1, MPI_INT, 0, MPI_COMM_WORLD);
             MPI_Gather(recvcounts+myrank, 1, MPI_INT, recvcounts+myrank, 1, MPI_INT, 0, MPI_COMM_WORLD);
             
             MPI_Bcast(displs, size, MPI_INT, 0, MPI_COMM_WORLD);
             MPI_Bcast(recvcounts, size, MPI_INT, 0, MPI_COMM_WORLD);
             
             MPI_Bcast(a, n0, MPI_CHAR, 0, MPI_COMM_WORLD);
             for (i = 2; i < n0; ++i)
             {
                 if (a[i])
                {
                    k = displs[myrank];
                    if (!k) break;
                    if (k % i)
                        k = k - (k % i) + i;
                    for (j = k; j < displs[myrank] + n_local && j < n_global; j += i)
                    {
                        if (a[j])
                        {
                    	    if (j >= first) ++count_local;
                    	    a[j] = 0;
                    	}
                    }
                }
             }
             MPI_Gatherv(a + displs[myrank], recvcounts[myrank], MPI_CHAR, a, recvcounts, displs, MPI_CHAR, 0, MPI_COMM_WORLD);
	        MPI_Send(&count_local, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
        
    }
    else
    {
        for (i = 2; i * i < n_global; ++i)
        {
            if (a[i])
                for (j = i * i; j < n_global; j += i)
                    a[j] = 0;
        }
    }
    free(recvcounts);
    free(displs);
    
    stop = MPI_Wtime();
    sendtime = stop - start;
    if (myrank) MPI_Send(&sendtime, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    else
    {
	sumtime = sendtime;
	maxtime = sendtime;
	for (i = 1; i < size; ++i)
	{
	    MPI_Recv(&recvtime, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
	    sumtime += recvtime;
	    if (recvtime > maxtime) maxtime = recvtime;
	}
    }
    
    MPI_Finalize();

    if (!myrank)
    {
	out = fopen(argv[3], "w");
	if (!out) 
	{
    	    printf("error: file is not opened \n");
    	    exit (-1);
	}

	printf ("%d numbers \n", count_global);
	for (i = first; i < n_global; ++i)
    	    if (a[i]) 
    	    {
    		fprintf(out, "%d ", i);
    		if(!((i-first)%10)) fprintf(out, "\n");
    	    }
    	fclose(out);
    	
    }

    free(a);    

    return 0;
}