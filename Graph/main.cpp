#include "common.h"
#include "Vertix.h"


int main() {
	size_t source;
	std::ios_base::sync_with_stdio(false);
	std::string filename = "soc-slashdot0902.txt";
	//std::cin >> filename;
	std::ifstream st;
	FILE *fp;
	fopen_s(&fp, "dijkstrra.txt", "w");
	st.open(filename);
	Vertix v(fp);
	try {
		v.Load(st);
	}
	catch (std::exception e) {
		std::cout << e.what();
		std::cin.get();
		std::cin.get();
		exit(EXIT_FAILURE);
	}
#ifndef ASYNCIO
	std::ofstream of;
	of.open("dijkstra.txt");
	v.FullDijkstra(of);
	of.close();
#else
	v.FullDijkstra();


#endif
	return EXIT_SUCCESS;
}