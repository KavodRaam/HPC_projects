// Raam Kavod

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>

#define SEED 205784218

int main( int argc, char *argv[])
{
    // Variables init
    double y, x, z, d, pi, start_Wtime, end_Wtime;
    double PI25DT = 3.141592653589793238462643;
    int finish = 0, my_rank, num_of_processes, name_length;
    long int Nc=100000000, dots_for_calculation=0, i, dots_in_sphere, dots_per_processor, Ns;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    
    //MPI init
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Get_processor_name(processor_name,&name_length);
	
    dots_per_processor = floor(Nc/num_of_processes);               // Even number of dots for each processor
    fprintf(stderr,"Process %d on %s\n", my_rank, processor_name); // Writing the process rank and name in a standard
                                                                   // error stream and print it (stderr and not stdout)
                                                                   // for good practice
  
    srand(SEED + my_rank); // Each process will get a different seed 
    while (!finish)        // While loop should repeat exactly twice
    {
        if (my_rank == 0) // Only main procces enters
        {
            if (dots_for_calculation==0) // Main process enters only in first while iteration
            {
                dots_for_calculation = dots_per_processor;
                start_Wtime = MPI_Wtime(); // Starts the clock for time measurments
            }
            else // Main process enters only in second (and last) iteration
            {
                dots_for_calculation = 0; // Signals the processes to end the loop
            }
	        
        }
        MPI_Bcast(&dots_for_calculation, 1, MPI_INT, 0, MPI_COMM_WORLD);// Main process broadcast the amount of dots to all processes 
        
        if (dots_for_calculation == 0)
        {
            finish = 1;      // Flag to end the while loop   
        }
        else
        {
            dots_in_sphere = 0;                         // A counter for the dots inside the sphere
            for (i = 1; i <= dots_for_calculation; i++) // A loop that "plots" number of dots stated by dots_per_processor variable
                                                        // and checks by their square sum if they are inside the unit cuircle
            {
            	// Randome coordinantes inside an octant of a cube
                x = (double)my_rank/num_of_processes + ((double)1/num_of_processes)*((double)rand()/RAND_MAX); // Between [i/n, (i+1)/n]
                y = (double)rand()/RAND_MAX;                                                                   // Between [0, (1)]
                z = (double)rand()/RAND_MAX;                                                                   // Between [0, (1)]    
				d = x*x + y*y + z*z;
				if (d <= 1) dots_in_sphere++;                                                                   // If inside sphere, add to counter
            }
            
            MPI_Reduce(&dots_in_sphere, &Ns, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); // Gather and sum using Reduce mpi command
           
         	
         	// Printing diagnostics: result, timing, error, number of dots and processors and 
            if (my_rank == 0)
	    	{
                pi = 6*((double)Ns/(double)Nc);
                end_Wtime = MPI_Wtime();    // Ends the clock for time measurments
	    		printf("Total number of dots = %lld\n", Nc);
				printf("Number of processes = %d\n",num_of_processes);
				printf("Number of dots per process = %lld\n", dots_per_processor);
				printf("Total dots inside the sphere = %lld\n",Ns);
                printf("pi is approximately %.16lf, the error is %.16lf\n",pi, fabs(pi - PI25DT));
				printf("wall clock time = %f\n",end_Wtime-start_Wtime);
			}
        }
    }
    MPI_Finalize();

    return 0;
}

