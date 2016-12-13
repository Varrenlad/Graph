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
	distArr = new size_t[nodes];
	visited = new bool[nodes]();
	while (!st.eof()) {
		st >> temp >> temp1;
		i = rand() % RANDOM_ENUM; //alas, we don't have weight, so we generate them
		//if (temp != temp1) { //remove loops
			adj[temp].push_back(std::make_pair(temp1, i));
			adj[temp1].push_back(std::make_pair(temp, i));
		//} 
			++j;	
			if (j % 51 == 0) {
				//system("cls");
				std::cout << "Loading " << j << "/" << edges << std::endl;
			}
	}
}

//really slow, careful
void Vertix::Show() {
	for (size_t i = 0; i < nodes; ++i) {
		for (size_t j = 0; j < adj[i].size(); ++j)
			std::cout << adj[i][j].first << " " << adj[i][j].second << " ";
		std::cout << std::endl;
	}
}

void Vertix::Dijkstra(size_t source) {
	{
		for (size_t i = 0; i < nodes; i++) //Set initial distances to Infinity
			distArr[i] = SIZE_MAX;
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
}

size_t Vertix::Size() {
	return nodes;
}

size_t Vertix::operator[](size_t i) {
	return distArr[i];
}