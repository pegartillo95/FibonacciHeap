#include "Fibonacci.hpp"
#include<vector>
#include <iostream>
#include <chrono>
using namespace std;
#include <cstdlib>
#include <ctime>
#include <fstream>


int main() {
	srand(time(NULL));
	FibHeap heap;

	ofstream OutputFile_1;
	ofstream OutputFile_2;
	ofstream OutputFile_3;
	chrono::time_point<chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_time;

	OutputFile_1.open("insert.dat");
	OutputFile_2.open("get_min.dat");
	OutputFile_3.open("get_size.dat");
	if (OutputFile_1.is_open() && OutputFile_2.is_open() && OutputFile_3.is_open()) {
		for (int i = 1; i <= 1000; i++) {
			//Hace la tabla con el tiempo que le lleva a insert
			start = chrono::system_clock::now();
			heap.insert(rand() % 10000 + 1);
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_1 << i << "   " << elapsed_time.count() * 1000 << endl;
			//Hace la tabla con el tiempo que le lleva a get_min
			start = chrono::system_clock::now();
			heap.get_min();
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_2 << i << "   " << elapsed_time.count() * 1000 << endl;

			//Hace la tabla con el tiempo que le lleva a get_size
			start = chrono::system_clock::now();
			heap.get_size();
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_3 << i << "   " << elapsed_time.count() * 1000 << endl;
		}
	}
	OutputFile_1.close();
	OutputFile_2.close();
	OutputFile_3.close();

	//Hace la tabla con el tiempo que le lleva a insert
	FibHeap heap_merge_1;
	OutputFile_1.open("merge.dat");
	if (OutputFile_1.is_open()) {

		for (int i = 1; i <= 1000; i++) {
			FibHeap heap_merge_1;
			for (int k = 1; k <= 10; k++) {
				heap_merge_1.insert(rand() % 10000 + 1);
			}

			FibHeap heap_merge_2;
			for (int j = 1; j <= i; j++) {
				heap_merge_2.insert(rand() % 10000 + 1);
			}
			start = chrono::system_clock::now();
			heap_merge_1.merge(&heap_merge_2);
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_1 << i + 10 << "   " << elapsed_time.count() * 1000 << endl;
		}
	}
	OutputFile_1.close();

	//Hace la tabla con el tiempo que lleva a extract_min
	OutputFile_1.open("extract_min.dat");
	if (OutputFile_1.is_open()) {
		FibHeap heap2;
		for (int i = 1; i <= 10000; i++) {
			heap2.insert(i);
		}
		for (int i = 1; i <= 10000; i++) {
			start = chrono::system_clock::now();
			heap2.extract_min();
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_1 << (10001 - i) << "   " << elapsed_time.count() * 1000 << endl;
		}
	}
	OutputFile_1.close();

	//Hace la tabla con el tiempo que lleva a 
	OutputFile_1.open("decrease_key.dat");
	if (OutputFile_1.is_open()) {
		FibHeap heap3;
		std::vector<FibHeap::Node*> Node_Array = std::vector<FibHeap::Node*>(1000);
		for (int i = 1; i <= 1000; i++) {
			Node_Array[i-1] = heap3.insert(i);
		}
		for (int i = 1000; i >= 1; i--) {
			start = chrono::system_clock::now();
			heap3.decrease_key(Node_Array[i-1], INT_MIN);
			end = chrono::system_clock::now();
			elapsed_time = end - start;
			OutputFile_1 << i << "   " << elapsed_time.count() * 1000 << endl;
			heap3.extract_min();
		}
	}
	OutputFile_1.close();

	return 0;
}