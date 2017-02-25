#include <iostream>
#include <thread>
#include <random>
#include <cmath>
#include <atomic>
#include <ctime>

using namespace std;


/*
	Created by Yonah Mann -- 25/2/17

	DESCRIPTION:
		This program run the Monty Carlo algorithm. The goal
		is to calculate the ratio PI using randomness.

	ALGORITHM:
		Imagine a square S with length 2 in the Cartesian system
		where the square points are on (0, 0), (0, 2), (2, 2), (2, 0).
		Also imagine a circle C with radius 1 which lies inside square S.
		The ratio of the #hits / #attempts, should be approximately the
		ratio of area(C) / area(S).

		1) Generate 2 random numbers in range [0, 2].
		2) Increment the number of attempts taken on experiment.
		3) If point generated is within circle C, increment number of hits.
		4) PI = 4 * #hits / #attempts.
		*/

const long long int NUM_OF_EXPERIMENTS = 20000000;
const int NUM_OF_THREADS = 15;  // generally recommend around 15 threads

const int SQUARE_LENGTH = 2;
const int CIRCLE_RADIUS = 1;

// pseudo-random number generator
uniform_real_distribution<double> uniform(0, SQUARE_LENGTH);
default_random_engine engine;


/*
	Counters are declared as atomic
	for concurrency safety.
*/

// number of times the square was hit
atomic<long long int> hits(0);

// number of times the experiment done
atomic<long long int> attempts(0);


/*
	A function for each thread to generate
	a random point and update data.
*/
void run_thread() {
	while (1) {
		// get random point
		double x = uniform(engine);
		double y = uniform(engine);

		// if point is in circle, increment number of hits
		hits += sqrt((x - CIRCLE_RADIUS) * (x - CIRCLE_RADIUS) + (y - CIRCLE_RADIUS) * (y - CIRCLE_RADIUS)) <= CIRCLE_RADIUS;

		if (++attempts >= NUM_OF_EXPERIMENTS) // reached end of experiment
			return;
	}

}


clock_t start;
clock_t finish;

double duration;

void start_timer() {
	start = clock();
}
void stop_timer() {
	finish = clock();
	duration = (finish - start) / (double)CLOCKS_PER_SEC;
}

int main() {
	thread workers[NUM_OF_THREADS];

	start_timer();

	// start workers
	for (int k = 0; k < NUM_OF_THREADS; ++k)
		workers[k] = thread(run_thread);

	// wait for threads to finish
	for (int k = 0; k < NUM_OF_THREADS; ++k)
		workers[k].join();

	double pi = 4.0 * hits / attempts;


	stop_timer();

	cout << "DONE: " << endl << endl 
		<< "#EXPERIMENTS = " << NUM_OF_EXPERIMENTS
		<< endl << "#THREADS = " << NUM_OF_THREADS
		<< endl << "ELAPSED TIME = " << duration << " s"
		<< endl << endl << "PI = " << pi << endl;
	cin.get();

	return EXIT_SUCCESS;
}
