#include "common.h"
#include "Vertix.h"

#ifndef RUN_TESTS
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

#else
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
TEST_CASE("Null data in modules") {
	Vertix v;
	CHECK_THROWS(v.FullDijkstra());
}

TEST_CASE("Empty graph") {
	Vertix v;
	std::ifstream fs("empty.txt");
	std::string st;
	v.Load(fs);
	v.FullDijkstra();
	fs.close();
}

TEST_CASE("One point") {

}

TEST_CASE("No edges") {

}

#endif