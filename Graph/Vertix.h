#pragma once

#include "common.h"

class Vertix {
	std::vector<std::pair<size_t, size_t>> *adj;
	std::list<long long> *adjL;
	bool *visited;
	size_t nodes = 0;
	size_t edges = 0;
	size_t *distArr;
	void SCCUtil(int, int[], int[], 
		std::stack<long long> *, bool[]);
	void _SCC();
public:
	Vertix();
	size_t Size();
	size_t operator[](size_t i);
	void Load(std::istream &);
	void Dijkstra(size_t);
	void Show();
	void SCC();
	~Vertix();
};

