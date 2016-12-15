#include "common.h"
#include "Vertix.h"


int main() {
	std::ios_base::sync_with_stdio(false);
	std::string filename = "soc-slashdot0902.txt";
	//std::cin >> filename;
	std::ifstream st;
	st.open(filename);
	Vertix v;
	try {
		v.Load(st);
	}
	catch (std::exception e) {
		std::cout << e.what();
		std::cin.get();
		std::cin.get();
		exit(EXIT_FAILURE);
	}
	v.FullDijkstra();
	return EXIT_SUCCESS;
}