// Brute force method - try all possible routes & pick the best one
// Brute force method - try all possible routes & pick the best one
// Brute force method - try all possible routes & pick the best one
#include <cstdlib>
#include <vector>
#include <cmath>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>  
#include <string>
#include <unordered_map>
using namespace std;

struct City
{
	int x;
	int y;
};
typedef vector<City> Cities;

struct Node {
	int city;
	vector<Node*> children; // connections
};

class Map
{
private:
	// Properties
	Cities cities;
	int** graph; // graph[y][x]
	int* mst;
	int* key_set; // holds the weights of the vertices;
	bool* unused_vertices; // holds unused vertices

	// Private functions
	// adds a city to the vector <City> cities
	int make_mst() {
		// First vertex will always = -1 to ensure it stays root
		key_set[0] = 0;
		mst[0] = -1;

		int path = INT16_MAX;
		int count = 0; // this holds current number of edges
		// Number of edges for an MST will always be n - 1
		for (int i = 0; i < n; i++)
		{
			int min = INT16_MAX;
			int min_index = -1;
			// get min key index val from unused vertices
			for (int i = 0; i < n; i++)
			{
				if (unused_vertices[i] == false &&
					key_set[i] < min)
				{
					min = key_set[i];
					min_index = i;
				}
			}
			int current_path = 0;
			unused_vertices[min_index] = true; // select vertex

			//make mst; remember: graph[y][x]
			// graph[min_index][j] is non-zero for all adjacent vertices
			// thats is: graph[0][0] = 0; graph[1][1] = 0 ... graph[n][n] = 0
			// unused_vertices[j] is false for vertices not used
			// if graph[min_key][j] < key[n] update key
			for (int j = 0; j < n; j++)
			{
				if (graph[min_index][j] && unused_vertices[j] == false
					&& graph[min_index][j] < key_set[j])
				{
					mst[j] = min_index;
					key_set[j] = graph[min_index][j];
					current_path += graph[j][mst[j]];
				}
				current_path += graph[i][0];

			}
			path = std::min(path, current_path);
		}
		return path;
	}
	// testing all permutations of the cities vector, finding the
	// least length traversal
	int bruteForce()
	{
		vector<int> route = get_default_tour();
		int shortest_path_len = traversal(route);
		//shuffle the vector
		while (std::next_permutation(route.begin() + 1, route.end()))
		{
			// call traversal on the given vector
			double current_path_len = traversal(route);
			// if path is shorter than shorest path, make it the new shortests path
			if (current_path_len < shortest_path_len)
			{
				shortest_path_len = current_path_len;
			}
		}
		return shortest_path_len;
	}
	// finds the length of the vector
	int traversal(vector<int> city_list)
	{
		//vector<City> city_list = getCity();
		int total_dist = distance(city_list[0], city_list[n - 1]);
		for (int i = 1; i < n; i++)
		{
			total_dist += distance(city_list[i - 1], city_list[i]);
		}
		return total_dist;
	}
	// getter for cities vector
	Cities getCities()
	{
		return cities;
	}
	vector<int> get_default_tour() const {
		vector<int> tour(n);
		for (int i = 0; i < n; ++i) {
			tour[i] = i;
		}
		return tour;
	}
	void print_graph()
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				cout << graph[i][j] << " ";
			}
			cout << endl;
		}
	}

	Node* structure_mst()
	{
		Node* root = new Node;
		// get "roots:"
		unordered_map<int, vector<int>> roots;
		for (int i = 1; i < n; i++)
		{
			roots[mst[i]].push_back(i);
		}
		helper(root, roots, 0);

		return root;
	}
	void helper(Node* root, unordered_map<int, vector<int>> data, int i) {
	
			// Base case 
			if (root == NULL) return;

			// Store current node 
			root->city = i;

			// Return if node contains no children
			if (data[i].empty()) return;

			// Else iterate and recur to add children
			for (int j = 0; j < data[i].size(); j++)
			{
				Node* new_child = new Node;
				helper(new_child, data, data[i][j]);
				root->children.push_back(new_child);
			}
			return;
	}

	void print_mst_preorder(Node* root)
	{
		if (root == NULL) return;

		cout << root->city;

		if (root->children.empty()) return;

		for (int i = 0; i < root->children.size(); i++)
		{
			print_mst_preorder(root->children[i]);
		}
	}

	void print_mst()
	{
		cout << "Edge \tWeight \n";
		for (int i = 1; i < n; i++)
		{
			cout << mst[i] << " - " << i << " \t" << graph[i][mst[i]] << endl;
		}
		//cout << "Perorder\n";
		//cout << mst[2] << ", " << 2 << ", ";
		//for (int i = 3; i < n; i++)
		//{
		//	cout << i << ", ";
		//}
		//	cout << mst[1] << ", " << 1 << endl;
		
		cout << "Print n-way MST\n";

		Node* mst = structure_mst();
		print_mst_preorder(mst);
	}

public:
	// constructor
	Map(Cities city_map)
	{
		cities = city_map;
		n = city_map.size();

		// initialize 2d array(graph)
		graph = new int* [n];
		for (int i = 0; i < n; i++) {
			graph[i] = new int[n];
			for (int j = 0; j < n; j++) {
				graph[i][j] = 0;
			}
		}
		// fill graph
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				graph[i][j] = graph[j][i] = distance(i, j);
			}
		}

		mst = new int[n];
		key_set = new int[n];
		unused_vertices = new bool[n];
		// Initialize keys
		for (int i = 0; i < n; i++)
		{
			key_set[i] = INT16_MAX;
			unused_vertices[i] = false;
		}


	};

	// Public properties
	int n; // number of cities

	// Public functions

	// finds the distance between two cities
	int distance(int start, int end)
	{
		City a = cities[start];
		City b = cities[end];
		return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
	}

	void print_default_tests()
	{
		cout << "Default linear tour length: " << traversal(get_default_tour()) << endl;
		cout << "Brute force solution: " << bruteForce() << endl;
		cout << "MST solution: " << make_mst() << endl;
		cout << "Graphs: \n";
		print_graph();
		print_mst();
		cout << endl;
	}

};

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
	generated_random_map.print_default_tests();

	return 0;
}
