#include "Vertix.h"

Vertix::Vertix(){
}


Vertix::~Vertix(){
}

void Vertix::Load(std::istream &st) {
#ifndef TIMERAND
	srand(SEED);
#else
	srand(time(NULL));
#endif
	size_t temp, temp1, i = 0, j = 0;
	if (st.rdstate() & std::ios::failbit) {
		std::exception e("Failed to load data from file");
		throw e;
	}
	do {
		std::string ssf;
		std::getline(st, ssf);
		std::stringstream lineTok(ssf);
		std::string token;
		while (lineTok >> token) { //split original data to tokens searching for numbers in header
			if (token.compare("Nodes:") == 0)
				lineTok >> nodes;
			if (token.compare("Edges:") == 0)
				lineTok >> edges;
		}
	} while (!(nodes && edges || st.eof()));
	if (!nodes) {
		std::exception e("Cannot find number of nodes, data might be corrupt");
		throw e;
	}
	while (!isdigit(temp = st.peek()))
		st.get(); //pass remainings of the header
	adj = new std::vector<std::pair<size_t, size_t>>[nodes];
	while (!st.eof()) {
		st >> temp >> temp1;
		i = rand() % RANDOM_ENUM; //alas, we don't have weight, so we generate them
			adj[temp].push_back(std::make_pair(temp1, i));
			adj[temp1].push_back(std::make_pair(temp, i));
			++j;
			if (j % 10240 == 0)
				std::cout << "Loading " << j << "/" << edges << "\n";
	}
}

//really slow, careful
void Vertix::Show(size_t node) {
	for (size_t j = 0; j < adj[node].size(); ++j)
		std::cout << j << ". " << adj[node][j].first << " " << adj[node][j].second << " " << std::endl;
	std::cin.get();
	std::cin.get();
}

void Vertix::Dijkstra(size_t source) {
	//Custom Comparator for Determining priority for priority queue (shortest edge comes first)
	class prioritize {
	public:
		bool operator ()(std::pair<int, int> &p1, std::pair<int, int> &p2) {
			return p1.second > p2.second;
		}
	};
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, prioritize> pq; //Priority queue to store vertex,weight pairs
	pq.push(std::make_pair(source, distArr[source] = 0)); //Pushing the source with distance from itself as 0
	while (!pq.empty())
	{
		std::pair<int, int> curr = pq.top(); //Current vertex. The shortest distance for this has been found
		pq.pop();
		int cv = curr.first, cw = curr.second; //'cw' the final shortest distance for this vertex
		if (visited[cv]) //If the vertex is already visited, no point in exploring adjacent vertices
			continue;
		visited[cv] = true;
		for (size_t i = 0; i < adj[cv].size(); i++) //Iterating through all adjacent vertices
			if (!visited[adj[cv][i].first] && adj[cv][i].second + cw < distArr[adj[cv][i].first]) //If this node is not visited and the current parent node distance+distance from there to this node is shorted than the initial distace set to this node, update it
				pq.push(std::make_pair(adj[cv][i].first, (distArr[adj[cv][i].first] = adj[cv][i].second + cw))); //Set the new distance and add to priority queue
	}
}

size_t Vertix::Size() {
	return nodes;
}

void Vertix::Relief(size_t i) {
	size_t j = 0;
	for (j = 0; j < adj[i].size(); ++j) {
		adj[adj[i][j].first].erase(adj[adj[i][j].first].begin()); //theoretically elements should be in front
	}
	adj[i].clear();
}

#ifndef ASYNCIO
void Vertix::FullDijkstra(std::ostream &os) {
#else
void Vertix::FullDijkstra(FILE *fPtr) {
#endif
	size_t i;
#ifdef ASYNCIO
	std::future<bool> print;
#endif
	for (i = 0; i < nodes; ++i) {
		std::cout << "Searching for " << i << " distances";
		if (distArr)
			delete[] distArr;
		distArr = new size_t[nodes];
		for (size_t i = 0; i < nodes; i++) //Set initial distances to Infinity
			distArr[i] = SIZE_MAX;
		if (visited)
			delete[] visited;
		visited = new bool[nodes]();
		Dijkstra(i);
#ifndef ASYNCIO
		Out(os, i);
#else
		size_t min = SIZE_MAX, mean = 0, max = 0, j, e = 0;
		for (j = 0; j < nodes; ++j) {
			if (j != i) {
				if (distArr[j] != SIZE_MAX) {
					++e;
					mean += distArr[j];
					if (distArr[j] < min)
						min = distArr[j];
					if (distArr[j] > max)
						max = distArr[j];
				}
			}
		}
		mean /= nodes;
		std::chrono::milliseconds span(100);
		while (print.wait_for(span) == std::future_status::timeout) {
			std::cout << "waiting" << std::endl;
		}
		print = Out(fPtr, i, min, mean, max, e);
#endif
	}
}
#ifndef ASYNCIO
void Vertix::Out(std::ostream &of, size_t source) {
	size_t min = SIZE_MAX, mean = 0, max = 0, i, e = 0;
	of << "Source is: " << source;
	for (i = 0; i < nodes; ++i) {
		if (i != source) {
			if (distArr[i] != SIZE_MAX) {
				++e;
				mean += distArr[i];
				if (distArr[i] < min)
					min = distArr[i];
				if (distArr[i] > max)
					max = distArr[i];
			}
		}
	}
	mean /= nodes;
	of << "\nThere's " << e << " existing links and " << nodes - e - 1 << " non-existing\n";
	of << "The shortest distance is " << min << ", longest is " << max << " and mean is " << mean;
	of << "\nUnique shortest distance to other vertex from here is: \n";
	for (i = 1; i <= Size(); i++) {
		of << "Vertex: " << i << ", Distance: ";
		distArr[i] != SIZE_MAX ? (of << ((distArr[i] == source) ? distArr[i] - 1 : distArr[i]) << "\n") : (of << "-1" << "\n");
	}
	of << std::endl;
}
#else
bool Vertix::Out(FILE *fPtr, size_t source, size_t min, size_t mean, size_t max, size_t e) {
	size_t *arrCopy = new size_t[nodes];
	memcpy(arrCopy, distArr, sizeof(size_t) * nodes);
	fprintf(fPtr, "Source is: %ll \nThere's %ll existing links and %ll non-existing\n \
The shortest distance is %ll, longest is %ll and mean is %ll\
\nUnique shortest distance to other vertex from here are: \n", source, e, nodes - e - 1, min, max, mean);
	for (size_t i = 1; i <= nodes; i++) {
		fprintf(fPtr, "Vertex: %ll, Distance: %ll\n", source, ((distArr[i] != SIZE_MAX) ? ((distArr[i] == source) ? distArr[i] - 1 : distArr[i]) : -1));
	}
	return true;
}
#endif