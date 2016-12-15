#include "Vertix.h"

void Out(size_t &source, size_t &min, size_t &mean, size_t &max, size_t &e, size_t &nodes, size_t *arrCopy);

int getNum() {
	return rand() % RANDOM_ENUM;
}

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
		//aRand = rand() % RANDOM_ENUM;
		st >> temp >> temp1;
		i = rand() % RANDOM_ENUM + 1; //alas, we don't have weight, so we generate them
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

void Vertix::FullDijkstra() {
	size_t i;
	size_t min = 0, mean = 0, max = 0, j, e = 0;
	visited = new bool[nodes];
	for (i = 0; i < nodes; ++i) {
		std::thread thr(Out, std::ref(i), std::ref(min), std::ref(mean), 
			std::ref(max), std::ref(e), std::ref(nodes), std::ref(distArr));
		//if (i % 100) //we will sync every 100 threads to keep ram consumption low
		//	thr.detach();
		
		std::cout << "Searching for " << i << " distances\n";
		if (visited)
			memset(visited, 0, sizeof(bool) * nodes);
		distArr = new size_t[nodes]; //we give pointer to save thread and allow it to free data
		memset(distArr, SIZE_MAX, sizeof(size_t) * nodes);
		Dijkstra(i);
		min = SIZE_MAX;
		mean = 0;
		max = 0;
		e = 0;
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
		//Relief(i);
		thr.join();
	}
}

void Out(size_t &source, size_t &min, size_t &mean, size_t &max, size_t &e, size_t &nodes, size_t *arrCopy) {
	if (mean == 0)
		return;
	FILE *fPtr;
	char *filename = new char[FILENAME_MAX];
	sprintf(filename, "Node %zu links.txt", source - 1);
	fPtr = fopen(filename, "w");
	assert(fPtr != nullptr);
	assert(source < 89000);
	fprintf(fPtr, "Source is: %zu\nThere's %zu existing links and %zu non-existing\nThe shortest distance is %zu, longest is %zu and mean is %zu\nUnique shortest distance to other vertex from here are:\n", source - 1, e, nodes - e - 1, min, max, mean);
	for (size_t i = 0; i < source - 1; i++) {
		fprintf(fPtr, "Vertex: %zu, Distance: %lld\n", i, ((arrCopy[i] != SIZE_MAX) ? arrCopy[i] : -1));
	} //needed to passthrough our source node
	for (size_t i = source; i <= nodes; i++) {
		fprintf(fPtr, "Vertex: %zu, Distance: %lld\n", i, ((arrCopy[i] != SIZE_MAX) ? arrCopy[i] : -1));
	}
	delete[] arrCopy; //free ram
	fclose(fPtr); 
}