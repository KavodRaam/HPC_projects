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
    double m_sun = 2*pow(10, 30), G = 6.674*pow(10, -11), light_year = 9*pow(10, 12), time_step = pow(2, 20);
	int v = 200, N = 992; //,total_time_steps = pow(2, 12);
    double stars_old[N][4], stars_new[N][4];  // [[star: x_pos, y_pos, x_vel, y_vel]]
    int my_rank, num_of_processes, stars_per_processor, name_length;
    double start_Wtime, end_Wtime;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    // temporary
    int total_time_steps = pow(2, 4);

    // MPI init
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Get_processor_name(processor_name, &name_length);

    start_Wtime = MPI_Wtime();
    srand(SEED + my_rank);

    stars_per_processor = floor(N/num_of_processes);
    double local_stars_old[stars_per_processor][4], local_stars_new[stars_per_processor][4];
    

    for (int star = 0; star < stars_per_processor; star ++)
    {
        local_stars_new[star][0] = light_year*fmod((double)rand()/RAND_MAX,100);
        local_stars_new[star][1] = light_year*fmod((double)rand()/RAND_MAX,100);
        local_stars_new[star][2] = v*(0.5 + fmod((double)rand()/RAND_MAX,1));
        local_stars_new[star][3] = v*(0.5 + fmod((double)rand()/RAND_MAX,1));
    }

    MPI_Gather(local_stars_new, stars_per_processor*4, MPI_DOUBLE,
    stars_new, stars_per_processor*4, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print Initial state of stars
    if (my_rank == 0)
    {
        for (int star = 0; star < N; star ++)
        {
            printf("star %d, location:(%f, %f), velocity(%f, %f)\n", star+1, stars_new[star][0],
            stars_new[star][1], stars_new[star][2], stars_new[star][3]);
        }
    }

    for (int i; i<total_time_steps; i++)
    {
        
    }






    // Print wall time
    end_Wtime = MPI_Wtime();
    if (my_rank == 0)
    {
        printf("wall clock time = %f\n",end_Wtime-start_Wtime);
    }

    // To extract data run using: mpirun -np 2 ./nbody.c > data.txt
    MPI_Finalize();
    
    return 0;
}
