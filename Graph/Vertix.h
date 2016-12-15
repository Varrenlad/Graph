#pragma once

#include "common.h"

class Vertix {
	FILE *log;
	std::vector<std::pair<size_t, size_t>> *adj;
	bool *visited;
	size_t nodes = 0;
	size_t edges = 0;
	size_t *distArr;
	void Dijkstra(size_t);
	void Relief(size_t);
	friend void Out(size_t &, size_t &, size_t &, size_t &, size_t &, size_t &, size_t *);
public:
	Vertix();
	size_t Size();
	void Load(std::istream &);
	void Show(size_t);
	void FullDijkstra();
	~Vertix();
};

