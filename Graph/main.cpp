#include "common.h"
#include "Vertix.h"


int main() {
	size_t source;
	std::string filename;
	std::cin >> filename;
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
	std::cout << "Select node to measure distance" << std::endl;
	std::cin >> source;
	//v.Show();
	v.Dijkstra(source);
	std::ofstream of;
	of.open("dijkstra.txt");
	of << "Source is: " << source 
		<< ". The shortest distance to every other vertex from here is: " << std::endl;
	for (size_t i = 1; i <= v.Size(); i++) {
		of << "Vertex: " << i << ", Distance: ";
		v[i] != SIZE_MAX ? (of << v[i] << std::endl) : (of << "-1" << std::endl);
	}
	return EXIT_SUCCESS;
}