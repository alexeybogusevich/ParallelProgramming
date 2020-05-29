#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <algorithm> 

const int NUMBER_OF_THREADS = 4;
const int NUMBER_OF_NODES = 2500;

using namespace std;

void fill_array(int **dist);
void free_resources(int **dist);
void show_distances(int **dist);
void mp_implementation(int **dist);

int main()
{
	int **dist = new int *[NUMBER_OF_NODES];
	for (size_t i = 0; i != NUMBER_OF_NODES; i++)
	{
		dist[i] = new int[NUMBER_OF_NODES];
	};

	printf("%s%d\n", "Number of nodes: ", NUMBER_OF_NODES);

	mp_implementation(dist);
	
	free_resources(dist);
}

void mp_implementation(int **dist) 
{
	double time_start, time_end;

	fill_array(dist);

	//show_distances(dist);

	int step = NUMBER_OF_NODES / NUMBER_OF_THREADS;
	int start;
	int thread_id;
	int stop;
	int i, j, k;

	time_start = omp_get_wtime(); // start measuring time

	#pragma omp parallel private(thread_id, start, stop)
	{
		thread_id = omp_get_thread_num();
		start = step * thread_id; // set start and stop indices for variable k
		stop = start + step;

		if (thread_id == omp_get_num_threads() - 1) 
		{
			stop = NUMBER_OF_NODES;
		}

		// run Floyd-Warshall algorithm using threads
		#pragma omp parallel for private(i, j)
		for (k = start; k < stop; k++) 
		{
			for (i = 0; i < NUMBER_OF_NODES; i++) 
			{
				for (j = 0; j < NUMBER_OF_NODES; j++) 
				{
					if (i == j) continue;
					if (dist[i][j] > dist[i][k] + dist[k][j])
					{
						dist[i][j] = dist[i][k] + dist[k][j];
					}
				}
			}
		}
	}

	time_end = omp_get_wtime(); // end measuring time
	double elapsed_time = time_end - time_start;

	//show_distances(dist);

	printf("OpenMP: total elapsed time: %.6f sec. Number of threads: %d.\n", elapsed_time, NUMBER_OF_THREADS);
}

void fill_array(int **dist)
{
	srand(42);

	for (int i = 0; i < NUMBER_OF_NODES; i++)
	{
		for (int j = 0; j < NUMBER_OF_NODES; j++)
		{
			if (i == j)
			{
				dist[i][j] = 0;
			}
			else
			{
				dist[i][j] = rand() % 100; // [0, 100 - 1]
			}
		}
	}
}

void free_resources(int **dist)
{
	for (int i = 0; i < NUMBER_OF_NODES; i++)
	{
		free(dist[i]);
	}
	free(dist);
}

void show_distances(int **dist)
{
	printf("  ");
	for (int i = 0; i < NUMBER_OF_NODES; i++)
	{
		printf("%3s%d ", "N", i);
	}
	printf("\n");
	for (int i = 0; i < NUMBER_OF_NODES; i++)
	{
		printf("N%d", i);
		for (int j = 0; j < NUMBER_OF_NODES; j++)
		{
			printf("%5d", dist[i][j]);
		}
		printf("\n");
	}
}