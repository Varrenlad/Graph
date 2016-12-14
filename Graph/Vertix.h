#pragma once

#include "common.h"

class Vertix {
	std::vector<std::pair<size_t, size_t>> *adj;
	bool *visited;
	size_t nodes = 0;
	size_t edges = 0;
	size_t *distArr;
	void Dijkstra(size_t);
	void Relief(size_t);
#ifdef ASYNCIO
	bool lock = false;
	bool Out(FILE *fPtr, size_t, size_t, size_t, size_t, size_t);
#else
	void Out(std::ostream &, size_t source);
#endif
public:
	Vertix();
	size_t Size();
	void Load(std::istream &);
	void Show(size_t);
#ifndef ASYNCIO
	void FullDijkstra(std::ostream &);
#else
	void FullDijkstra(FILE *);
#endif
	~Vertix();
};

