dd#include <iostream>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <queue>
#include <stack>

#define P 5381

#include "Hashtable.h"

// generic implementation of graph node
template <typename Tkey>
struct Vertex {
	// identification data
	// = a key, coresponding to an int index
	// the corespondence is realised using a hashtable
	// to which the graph has a reference
	Tkey id;
	// list of vertices to which the vertex links
	std::vector<Tkey> edges;
};

// Generic implementation of graph
// which stores elements of type Tkey.
// methods function more efficiently on integer data,
// therefore the graph uses two hashtables to
// make the conversion between Tkey - int and vice-versa.
template <typename Tkey>
class Graph {
 public:
 	Hashtable<Tkey, int> *t;
 	Hashtable<int, Tkey> *h;
 	int n_vertices;
 	std::vector<struct Vertex<Tkey> > vertices;

 	Graph(int n, Hashtable<Tkey, int>* _t, Hashtable<int, Tkey>* _h) {
 		n_vertices = n;
 		t = _t;
 		h = _h;
 		vertices.resize(n);
 		for (int i = 0; i < n; ++i) {
 			vertices[i].edges.resize(0);
 		}
 	}

 	~Graph() {}

 	// function to convert vertices' index to Tkey id
 	// using hashtable to which h points
 	void addVerticesData() {
 		for (int i = 0; i < vertices.size(); ++i) {
 			// operator= must be implemented for type Tkey
 			vertices[i].id = h->get(i);	
 		}
 	}

 	// Function that adds a directed edge
 	// between 2 vertices
 	void addEdge(Tkey src, Tkey dst) {
 		// get int corespondent of src
 		// adding Tkey type destination
 		int s;
 		s = t->get(src);
 		vertices[s].edges.push_back(dst);
 	}

 	void DFS(Tkey start_node, bool visited[]) {
 		// get index of starting node based on key
 		int s_node = t->get(start_node);
 		// std::cout << start_node << " ";
 		// mark it as visited
 		visited[s_node] = true;
 		// std::cout << "Node [" << start_node << "] set to true" << std::endl;
 		// iterate through the nodes he connects to
 		for (int i = 0; i < vertices[s_node].edges.size(); ++i) {
 			// verify if current node has already been marked
 			int node = t->get(vertices[s_node].edges[i]);
 			if (!visited[node]) {
 				// if not, call DFS with current node as the starting node
 				DFS(vertices[s_node].edges[i], visited);
 			}
 		}
	}

	void BFS(Tkey start_node, bool visited[]) {
		int i;
		for (i = 0; i < n_vertices; ++i) {
			visited[i] = false;
		}
		int u = t->get(start_node);
		std::queue<std::string> q;
		q.push(start_node);
		visited[u] = true;

		while (!q.empty()) {
			int f = t->get(q.front());
			q.pop();

			for (i = 0; i < vertices[f].edges.size(); ++i) {
				if (!visited[t->get(vertices[f].edges[i])]) {
					q.push(vertices[f].edges[i]);
					visited[t->get(vertices[f].edges[i])] = true;
				}
			}
		}
	}

	void iterativeDFS(Tkey start_node, bool visited[]) {
		for (int i = 0; i < n_vertices; ++i) {
			visited[i] = false;
		}
		std::stack<std::string> s;
		int u = t->get(start_node);
		s.push(start_node);
		while (!s.empty()) {
			u = t->get(s.top());
			s.pop();
			if (!visited[u]) {
				visited[u] = true;
			}
			for (int i = 0; i < vertices[u].edges.size(); ++i) {
				if (!visited[t->get(vertices[u].edges[i])]) {
					s.push(vertices[u].edges[i]);
				}
			}

		}
	}

	Graph<Tkey> getTranspose() {
		Graph<Tkey> g(n_vertices, t, h);
		for (int v = 0; v < n_vertices; ++v) {
			for (int i = 0; i < vertices[v].edges.size(); ++i) {
				g.addEdge(vertices[v].edges[i], vertices[v].id);
			}
		}
		g.addVerticesData();
		return g;	
	}

	bool isValid() {
		int i;
		bool visited[n_vertices];
		for (i = 0; i < n_vertices; ++i) {
			visited[i] = false;
		}
		DFS(vertices[0].id, visited);
		for (i = 0; i < n_vertices; ++i) {
			if (!visited[i]) return false;
		}
		
		for (i = 0; i < n_vertices; ++i) {
			visited[i] = false;
		}
		
		Graph<Tkey> G = getTranspose();
		DFS(G.vertices[0].id, visited);
		
		for (i = 0; i < n_vertices; ++i) {
			if (!visited[i]) return false;
		}
		return true;
	}


 	void testhash(Tkey key) {
 		std::cout << key << " " << t->get(key) << std::endl;
 		std::cout << t->get(key) << " " << h->get(t->get(key)) << std::endl;
 	}

 	void display() {
 		for (int i = 0; i < n_vertices; ++i) {
 			std::cout << i << ": " << vertices[i].id << "\n\t";
 			for (int j = 0; j < vertices[i].edges.size(); ++j) {
 				std::cout << "[" << vertices[i].edges[j]
 						  << " " << t->get(vertices[i].edges[j])
 						  << "] ";
 			}
 			std::cout << std::endl;
 		}
 	}
};

// Function that returns the hash of a string
int hash(std::string key) {
    int hkey = 0;
    for (int i = 0; i < key.size(); i++)
        hkey = (hkey * P + key[i]) % VMAX;
    return hkey;
}

// Function that returns the hash of an integer
int hash2(int key) {
	return key * 5831 % VMAX;
}

void solveTask1(std::vector<std::pair<std::string, int> > cities) {
	std::ofstream out;
	out.open("task1.out");
	int n_max = -1, i_max;
	for (int i = 0; i < cities.size(); ++i) {
		if (cities[i].second > n_max) {
			n_max = cities[i].second;
			i_max = i;
		}
	}
	out << cities[i_max].first << " " << n_max << std::endl;
	out.close();
}

void solveTask2(Graph<std::string> *map, bool *isValid) {
	std::ofstream out;
	out.open("task2.out");
	if (map->isValid()) { 
		out << "HARTA VALIDA\n";
		*isValid = true;
	} else {
		out << "HARTA INVALIDA\n";
		*isValid = false;
	}
	out.close();
}

void solveTask3(Graph<std::string> *map, Graph<std::string> *pam,
				std::vector<std::string> &p,
				std::vector<std::pair<std::string,int> > &c, bool isValid) {
	
	std::ofstream out;
	out.open("task3.out");
	if (isValid) {
		out << "Exista drum intre oricare doua orase\n";
		out.close();
		return;
	}
	bool visited[c.size()], t_visited[c.size()];
	for (int j = 0; j < p.size(); ++j) {
		for (int j = 0; j < c.size(); ++j) {
			visited[j] = t_visited[j] = false;
		}
		map->iterativeDFS(p[j], visited);
		pam->iterativeDFS(p[j], t_visited);
		out << "Orase inaccesibile pentru " << p[j] << ":";
		for (int k = 0; k < c.size(); ++k) {
			if (!visited[k] || !t_visited[k]) {
				out << " " << c[k].first;
			} 
		}
		out << std::endl;
	}
	out.close();

}

struct bus {
	int pass;
	std::string src;
	std::string dst;
};

int main() {
	// String-integer mapped hashtable 
	Hashtable<std::string, int> s2i(VMAX, &hash);
	// Integer-string mapped hashtable
	Hashtable<int, std::string> i2s(VMAX, &hash2);
	int N, M, B, K;
	// Buffers for reading input
	std::string buff1, buff2;
	// In pair: first = name of city, second = how crowded 
	std::vector<std::pair<std::string, int> > cities;
	std::cin >> N >> M;
	cities.resize(N);
	// Getting the name of the cities
	// Adding them to the vector and the hashtables
	for (int i = 0; i < N; ++i) {
		std::cin >> cities[i].first;
		cities[i].second = 0;
		s2i.put(cities[i].first, i);
		i2s.put(i, cities[i].first);
	}
	// Graph initialisation
	Graph<std::string> map(N, &s2i, &i2s);
	map.addVerticesData();
	for (int i = 0; i < M; ++i) {
		// Street between City1 and City2
		std::cin >> buff1 >> buff2;
		map.addEdge(buff1, buff2);
		cities[s2i.get(buff2)].second++;
	}
	std::cin >> B;
	std::vector<std::string> gas_stations;
	gas_stations.resize(B);
	for (int i = 0; i < B; ++i) {
		std::cin >> gas_stations[i];
	}
	std::cin >> K;
	std::vector<std::string> tobeproccesed;
	tobeproccesed.resize(K);
	for (int i = 0; i < K; ++i) {
		std::cin >> tobeproccesed[i];
	}

	/* test to check if data is correct
	for (int i = 0; i < N; ++i) {
		std::cout << cities[i].first << " " << cities[i].second << std::endl;
	}
	*/
	Graph<std::string> pam = map.getTranspose();
	bool isValid;
	solveTask1(cities);
	solveTask2(&map, &isValid);
	// if (N != 5000 && M != 20017)
	solveTask3(&map, &pam, tobeproccesed, cities, isValid);



	return 0;
}