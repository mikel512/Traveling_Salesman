#include <cstdlib>
#include<iostream>
#include <time.h>
#include <string>
#include <unordered_map>
#include "Map.h"
using namespace std;

// Generates a map with the desired number of cities at random coordinates within
// Generates a map with the desired number of cities at random coordinates within
// a 10 by 10 graph.
Map generate_random_map(int des_num_cities)
{
	Cities city_vector;
	for (int i = 0; i < des_num_cities; i++)
	{
		City new_city;
		int rand_num = rand() % 11;
		int rand_num2 = rand() % 11;
		new_city.x = rand_num;
		new_city.y = rand_num2;
		city_vector.push_back(new_city);
	}
	Map new_Map = Map(city_vector);
	return new_Map;
}

int main()
{
	// Tests
	cout << "Unit Tests:\n";
	Cities city_vector_hardcoded;
	City a;
	City b;
	a.x = 3; a.y = 2;
	b.x = 2; b.y = 3;
	City c;
	City d;
	c.x = 5; c.y = 4;
	d.x = 4; d.y = 5;
	city_vector_hardcoded.push_back(c);
	city_vector_hardcoded.push_back(d);
	city_vector_hardcoded.push_back(a);
	city_vector_hardcoded.push_back(b);

	Map test_map = Map(city_vector_hardcoded);

	cout << "Number of cities: " << test_map.n
		<< " EXPECTED OUTPUT: 4\n";
	cout << "Distance from 0 to 1: " << test_map.distance(0, 1)
		<< " EXPECTED OUTPUT: 1\n";
	cout << "Distance from 0 to 2: " << test_map.distance(0, 2)
		<< endl;
	cout << "Distance from 2 to 3: " << test_map.distance(2, 3)
		<< endl;
	cout << "Distance from 2 to 1: " << test_map.distance(2, 1)
		<< endl;
	cout << "Distance from 3 to 1: " << test_map.distance(3, 1)
		<< endl;
	cout << "Distance from 0 to 3: " << test_map.distance(3, 0)
		<< endl;
	test_map.print_default_tests();

	// Implementation
	cout << "Conducting Traveling Salesman solutions:\n";
	srand(time(0));
	Map generated_random_map = generate_random_map(5);
	City e;
	e.x = 9, e.y = 1;
	City f;
	f.x = 15, f.y = 2;
	City g;
	g.x = 1, g.y = 5;
	City h;
	h.x = 7, h.y = 18;
	City i;
	i.x = 3, i.y = 11;
	city_vector_hardcoded.push_back(e);
	city_vector_hardcoded.push_back(f);
	city_vector_hardcoded.push_back(g);
	city_vector_hardcoded.push_back(h);
	city_vector_hardcoded.push_back(i);
	Map TravelingSalesman = Map(city_vector_hardcoded);
	TravelingSalesman.print_default_tests();
	TravelingSalesman.get_minium_weight_perfect_matching();
	generated_random_map.print_default_tests();

	return 0;
}
