#pragma once

#include "common.h"

class Vertix {
	std::vector<std::pair<size_t, size_t>> *adj;
	bool *visited;
	size_t nodes = 0;
	size_t edges = 0;
	size_t *distArr;
	size_t *precessorArr;
public:
	Vertix();
	size_t Size();
	size_t operator[](size_t i);
	void Load(std::istream &);
	void Dijkstra(size_t);
	void StrongConnComp(std::ostream);
	void Show();
	~Vertix();
};

