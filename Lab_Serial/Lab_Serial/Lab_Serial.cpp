// Lab_Serial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm> 
#include <chrono>


const int NUMBER_OF_NODES = 2500;

void serial_implementation(int **dist);
void fill_array(int **dist);
void free_resources(int **dist);
void show_distances(int **dist);


using namespace std;

int main()
{
	int **dist = new int *[NUMBER_OF_NODES];
	for (size_t i = 0; i != NUMBER_OF_NODES; i++) 
	{
		dist[i] = new int[NUMBER_OF_NODES];
	};

	printf("%s%d\n", "Number of nodes: ", NUMBER_OF_NODES);
	serial_implementation(dist);
	free_resources(dist);
}

void serial_implementation(int **dist) 
{
	fill_array(dist);

	//show_distances(dist);

	clock_t t1 = clock();
	for (int k = 0; k < NUMBER_OF_NODES; k++)
	{
		for (int i = 0; i < NUMBER_OF_NODES; i++) 
		{
			for (int j = 0; j < NUMBER_OF_NODES; j++) 
			{
				if (i == j) continue;
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	clock_t t2 = clock();

	//show_distances(dist);

	printf("Serial implementation: total elapsed time: %.2f sec.\n",
		double(t2 - t1)/1000);
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