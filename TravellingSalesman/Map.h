#pragma once
#ifndef MAP_H
#define MAP_H
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
	int degree;
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
	Node* mst_root;
	int** subgraph;
	vector<int> odd_vertices;
	vector<tuple<int, int>> mwpf;

	// Private functions
	// adds a city to the vector <City> cities
	void make_mst();

	// testing all permutations of the cities vector, finding the
	// least length traversal
	int bruteForce();
	// finds the length of the vector
	int traversal(vector<int> city_list);
	// getter for cities vector
	Cities getCities();
	// get default tour
	vector<int> get_default_tour()const;
	// prints adjacency graph(weighted graph)
	void print_graph();
	// returns MST in tree form
	Node* structure_mst();
	// recursive function that builds MST 
	void helper(Node* root, unordered_map<int, vector<int>> data, int i);
	// print preorder
	void arrange_preorder(Node* root, vector<int>& full_path);
	// prints mst 
	void print_mst();
	// adds total distance from preorder traversal
	int get_mst_distance(vector<int> path);
	void traverse_mst_finding_odds(Node* n, vector<int>& set);
	void get_odd_edges_from_mst();
	void make_subgraph();
	int subgraph_distance(int start, int end);
	vector<tuple<int, int>> get_subgraph_edges();
	void set_mwpm(vector<tuple<int, int>> edge_set);
	int get_total_weight(vector<tuple<int, int>> incident_edges);
public:
	// Public properties
	int n; // number of cities
	// constructor
	Map(Cities city_map);


	// Public functions
	void get_minium_weight_perfect_matching();

	// finds the distance between two cities
	int distance(int start, int end);

	void print_default_tests();

};

#endif

