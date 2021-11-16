#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

void anagram() {
	std::string line;
	std::ifstream fin;
	std::map<std::string, int> dictionary;
    
	fin.open("dictionary.txt");
	while (getline(fin, line)) {
        std::string word = line.substr(0, line.find(','));
        int value = std::stoi(line.substr(line.find(' ') + 1));
        dictionary[word] = value;
    }
	fin.close();

    std::vector<std::string> sentenceWords;
	fin.open("input.txt");
	while (getline(fin, line)) {
		sentenceWords.clear();
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		std::cout << "'" << line << "'" << std::endl;
		getchar();
		std::string word;
		std::stringstream sstream(line);
		while (getline(sstream, word, ' ')) {
			sentenceWords.push_back(word);
		}
		for (auto word : sentenceWords) {
			std::cout << "!" << word << "!";
		}
		std::cout << std::endl;
		getchar();
	}
	fin.close();
}

int main() {
	anagram();
	return 0;
}