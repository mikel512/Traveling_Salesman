#include "Map.h"

// Private functions
// adds a city to the vector <City> cities
void Map::make_mst() {
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
	return;
}
// testing all permutations of the cities vector, finding the
// least length traversal
int Map::bruteForce()
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
int Map::traversal(vector<int> city_list)
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
Cities Map::getCities()
{
	return cities;
}
// Gets linear tour
vector<int> Map::get_default_tour() const {
	vector<int> tour(n);
	for (int i = 0; i < n; ++i) {
		tour[i] = i;
	}
	return tour;
}
// prints weighted graph
void Map::print_graph()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << graph[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
// structures MST into n-way tree
Node* Map::structure_mst()
{
	Node* root = new Node;
	root->degree = 0;
	// get "roots:"
	unordered_map<int, vector<int>> roots;
	for (int i = 1; i < n; i++)
	{
		roots[mst[i]].push_back(i);
	}
	helper(root, roots, 0);

	mst_root = root;
	return root;
}
// tree builder helper
void Map::helper(Node* root, unordered_map<int, vector<int>> data, int i) {

	// Base case 
	if (root == NULL) return;

	// Store current node 
	root->city = i;

	// Return if node contains no children
	if (data[i].empty()) return;

	// Else iterate and recur to add children
	for (int j = 0; j < data[i].size(); j++)
	{
		// new child nodes have a degree of at least 1
		Node* new_child = new Node;
		new_child->degree = 1;
		helper(new_child, data, data[i][j]);
		root->children.push_back(new_child);
		root->degree++;
	}
	return;
}
// arranges preorder traversal in vector
void Map::arrange_preorder(Node* root, vector<int>& full_path)
{
	if (root == NULL) return;

	full_path.push_back(root->city);

	if (root->children.empty()) return;

	for (int i = 0; i < root->children.size(); i++)
	{
		arrange_preorder(root->children[i], full_path);
	}
}
// prints MST edge by edge
void Map::print_mst()
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

	Node* mst = structure_mst();
	vector<int> full_path;
	arrange_preorder(mst, full_path);
	full_path.push_back(0);
	cout << "Print preorder vector\n";
	for (auto i : full_path)
	{
		cout << i << " ";
	}
	cout << "\nMST Solution: " << get_mst_distance(full_path) << endl;
}
// adds total distance from preorder traversal
int Map::get_mst_distance(vector<int> path)
{
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		count += graph[path[i]][path[i + 1]];
	}
	return count;
}
// Make Minium Weight perfect matching set of Edges
void Map::get_minium_weight_perfect_matching()
{
	// get set of odd degree vertices O
	// by handshaking lemma, size of set O should be even
	get_odd_edges_from_mst();

	// make subgraph from set O
	make_subgraph();

	// find all edges of subgraph
	auto edges = get_subgraph_edges();

	// set minimum weight perfect matching
	set_mwpm(edges);

}
void Map::set_mwpm(vector<tuple<int, int>> edge_set)
{
	// amount of edges to find = O/2
	vector<tuple <int, int>> mwpm;
	int weight = INT16_MAX;
	for (int i = 0; i < edge_set.size(); i++)
	{
		vector<tuple <int, int>> temp;
		auto edge = edge_set[i];
		edge_set.erase(edge_set.begin() + i);
		temp.push_back(edge);
		for (int j = 0;j < edge_set.size(); j++)
		{
			// check if edges are not incidental
			if (get<0>(edge) == get<0>(edge_set[j]) ||
				get<1>(edge) == get<0>(edge_set[j]) ||
				get<1>(edge) == get<1>(edge_set[j]) ||
				get<0>(edge) == get<1>(edge_set[j])
				)
			{
				continue;
			}
			temp.push_back(edge_set[j]);
			// if all edges acquired get weight
			if (temp.size() == odd_vertices.size() / 2)
			{
				int current_weight = get_total_weight(temp);
				if (current_weight < weight)
				{
					weight = current_weight;
					mwpm = temp;
				}
			}

		}
		edge_set.insert(edge_set.begin() + i, edge);
	}
	for (auto i : mwpm) 
	{
		cout << get<0>(i) << get<1>(i) << " ";
	}
	cout << endl;

	return;
}

int Map::get_total_weight(vector<tuple<int, int>> incident_edges)
{
	int count = 0;
	for (auto i : incident_edges)
	{
		count += graph[get<0>(i)][get<1>(i)];
	}
	return count;
}

vector<tuple<int, int>> Map::get_subgraph_edges()
{
	// using set O find unique pairs

	// set of unique pairs = (size(O) - 1)!
	// # of pairs in the end = size(O)/2
	vector<tuple <int, int>> edge_set_M;
	// get unique pairs with edge length
	for (int i = 0; i < odd_vertices.size(); i++)
	{
		int vertex = odd_vertices[i];
		odd_vertices.erase(odd_vertices.begin() + i);
		for (int j = 0;j < odd_vertices.size(); j++)
		{
			tuple<int, int> one = make_tuple(vertex, odd_vertices[j]);
			tuple<int, int> two = make_tuple(odd_vertices[j], vertex);
			// check if pair already exists in map
			if (find(edge_set_M.begin(), edge_set_M.end(), one) != edge_set_M.end() ||
				find(edge_set_M.begin(), edge_set_M.end(), two) != edge_set_M.end())
			{
				continue;
			}
			edge_set_M.push_back(one);
		}
		odd_vertices.insert(odd_vertices.begin() + i, vertex);
	}
	for (auto i : edge_set_M) {
		cout << get<0>(i) << get<1>(i) << " ";
	}
	cout << endl;
	return edge_set_M;
}
void Map::make_subgraph()
{
	int subgraph_size = odd_vertices.size();
	// initialize subgraph first
	subgraph = new int* [subgraph_size];
	for (int i = 0; i < subgraph_size; i++) {
		subgraph[i] = new int[subgraph_size];
		for (int j = 0; j < subgraph_size; j++) {
			subgraph[i][j] = 0;
		}
	}
	// fill graph
	for (int i = 0; i < subgraph_size; i++)
	{
		for (int j = 0; j < subgraph_size; j++)
		{
			subgraph[i][j] = subgraph[j][i] = subgraph_distance(i, j);
		}
	}
	// Print subgraph:
	cout << "Printing subgraph: \n";
	for (int i = 0; i < subgraph_size; i++)
	{
		for (int j = 0; j < subgraph_size; j++)
		{
			cout << subgraph[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

}
int Map::subgraph_distance(int start, int end)
{
	int first = odd_vertices[start];
	int second = odd_vertices[end];
	// use overlaying graph to get distances
	int distance = graph[second][first];
	return distance;
}
void Map::get_odd_edges_from_mst()
{
	vector<int> set_O;
	// traverse mst to find odd vertices
	traverse_mst_finding_odds(mst_root, set_O);
	// Print:
	cout << "Printing set O:\n";
	for (auto i : set_O)
	{
		cout << i << " ";
	}
	cout << endl;
	odd_vertices = set_O;
}
void Map::traverse_mst_finding_odds(Node* n, vector<int>& set)
{
	if (n == NULL)
	{
		return;
	}
	// if node degree is odd
	if (n->degree % 2 == 1)
	{
		set.push_back(n->city);
	}
	for (auto child : n->children)
	{
		traverse_mst_finding_odds(child, set);
	}
}
// constructor
Map::Map(Cities city_map)
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


}

// Public functions
// finds the distance between two cities
int Map::distance(int start, int end)
{
	City a = cities[start];
	City b = cities[end];
	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

void Map::print_default_tests()
{
	cout << "Default linear tour length: " << traversal(get_default_tour()) << endl;
	cout << "Brute force solution: " << bruteForce() << endl;
	make_mst();
	cout << "Wighted graph: \n";
	print_graph();
	print_mst();
	cout << endl;
}


