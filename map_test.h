#pragma once
#include "test_file.h"
#include "Map.h"
#include <string>
#include <fstream>
//#include "profile.h"
namespace map_test {
/*	void read_and_count() {
		My_STL::map<std::string ,int> word_map;
		std::ifstream input;
		input.open("source.txt");
		std::string word;
		time_clock::ProfilerInstance::start();
		while (input.good()) {
			input >> word;
			++word_map[word];
		}
		time_clock::ProfilerInstance::finish();
		time_clock::ProfilerInstance::dumpDuringTime(cout);
		std::ofstream output("result.txt");
		//output << "long" << endl;
		for (auto iter = word_map.begin(); iter != word_map.end(); ++iter)
			output << iter->first <<":"<< iter->second << endl;
	}
	*/
	void map_test() {
		My_STL::map<std::string, int> word_map;
		word_map[std::string("long")] = 1;
		word_map[std::string("fuck")] = 2;
		//word_map.
		for (auto iter = word_map.begin(); iter != word_map.end(); ++iter)
			cout << iter->first << ":" << iter->second << endl;
		//cout << word_map.count(("lon")) << endl;
		//cout << word_map.upper_bound("long")->first << endl;
		word_map.erase("long");
		for (auto iter = word_map.begin(); iter != word_map.end(); ++iter)
			cout << iter->first << ":" << iter->second << endl;

	}
	
}
 
