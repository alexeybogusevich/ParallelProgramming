#include "pch.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <conio.h>
#include <time.h>
#include "mpi.h"

const int NUMBER_OF_NODES = 10;
const static int tag = 1;

using namespace std;

void fill_array(int **dist);
void free_resources(int **dist);
void show_distances(int **dist);

int main(int argc, char** argv) 
{
	MPI_Init(&argc, &argv);

	MPI_Status status;
	int rank, NUMBER_OF_THREADS;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &NUMBER_OF_THREADS);

	if (NUMBER_OF_THREADS == 1)
	{
		fprintf(stderr, "Number of processes cannot be 1!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	const int ROOT = 0;

	int *data = (int *)malloc(NUMBER_OF_NODES*NUMBER_OF_NODES * sizeof(int));
	int **dist = (int **)malloc(NUMBER_OF_NODES * sizeof(int *));

	for (int i = 0; i < NUMBER_OF_NODES; i++) 
	{
		dist[i] = &(data[NUMBER_OF_NODES*i]);
	}

	double time_from, time_to, lapsed_time;

	if (rank == ROOT) 
	{
		fill_array(dist);
		show_distances(dist);
		time_from = MPI_Wtime();
	}

	int step = NUMBER_OF_NODES / NUMBER_OF_THREADS;

	MPI_Bcast(*dist, NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, MPI_COMM_WORLD); // send the array to every process

	if (rank != ROOT) 
	{
		int start = step * rank; // set start and stop indices for variable k
		int stop = start + step;

		if (rank == NUMBER_OF_THREADS - 1)
		{
			stop = NUMBER_OF_NODES;
		}

		for (int k = start; k < stop; k++)
		{
			for (int i = 0; i < NUMBER_OF_NODES; i++)
			{
				for (int j = 0; j < NUMBER_OF_NODES; j++)
				{
					if (i == j) continue;
					if (dist[i][j] > dist[i][k] + dist[k][j])
					{
						dist[i][j] = dist[i][k] + dist[k][j];
					}
				}
			}
		}

		MPI_Send(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, 1, MPI_COMM_WORLD);
	}
	else 
	{
		for (int source = 1; source < NUMBER_OF_THREADS; source++)
		{
			MPI_Recv(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
		}

		double time_to = MPI_Wtime();
		show_distances(dist);

		double lapsed_time = time_to - time_from;
		printf("%s%d\n", "Number of nodes: ", NUMBER_OF_NODES);
		printf("Elapsed time: %.6f seconds.\n", lapsed_time);
	}

	free(dist);

	MPI_Finalize();
	return 0;
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