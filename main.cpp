#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <utility>

int anagram() {
	std::string line;
	std::ifstream fin;
	std::vector<std::pair<std::string, int>> dict;

	// Заполняем словарь	
	fin.open("dictionary.txt");
	while (getline(fin, line)) {
        std::string word = line.substr(0, line.find(','));
        int value = std::stoi(line.substr(line.find(' ') + 1));
        dict.emplace_back(word, value);
    }
	fin.close();
	// TODO file read error exception throw

	std::cout << "Dictionary:" << std::endl;
	for (auto elem : dict) {
		std::cout << elem.first << " " << elem.second << std::endl;
	}
	std::cout << std::endl;
	
	/*
	std::string word = "lsvei";

	std::pair<std::string, int> maxSuitableWord {"", -999};
	for (auto elem : dict) {
		if (std::is_permutation(word.begin(), word.end(), elem.first.begin()) &&
			elem.second > maxSuitableWord.second) 
		{
			maxSuitableWord = elem;
		}
	}
	std::cout << maxSuitableWord.first << " " << maxSuitableWord.second << std::endl;
	*/


	std::vector<std::string> sentenceWords;
	fin.open("input.txt");
	while (getline(fin, line)) {
		// читаем предложение
		sentenceWords.clear();
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Windows endline fix
		std::cout << "'" << line << "'" << std::endl;
		getchar();
		
		// разбиваем предлодение по словам и заполняем вектор sentenceWords
		std::string word;
		std::stringstream sstream(line);
		while (getline(sstream, word, ' ')) {
			sentenceWords.push_back(word);
		}
		std::cout << "Splitting by word: ";
		for (auto word : sentenceWords) {
			std::cout << "!" << word << "!";
		}
		std::cout << std::endl;
		getchar();

		// Вывод всех престановок для слова, найденных в словаре
		// TODO хранение перестановок, сортированных по весу ???
		// TODO вывести ошибку, если слово не найдено
		for (auto word : sentenceWords) {
			std::cout << "Permutations for " << word << ": ";
			
			for (auto matchElem : dict) {
				if (std::is_permutation(word.begin(), word.end(), matchElem.first.begin(), matchElem.first.end())) {
					std::cout << matchElem.first << " weight=" << matchElem.second << ", ";
				}
			}
			std::cout << std::endl;
			getchar();
		}


		// Формируем предложение для перестановки
		std::vector<std::string> permutationSentence;
		int sentenceWeight = 0;
		for (auto word : sentenceWords) {
			std::pair<std::string, int> maxSuitableWord {"", -1};
			for (auto elem : dict) {
				if (std::is_permutation(word.begin(), word.end(), elem.first.begin()) &&
					elem.second > maxSuitableWord.second) 
				{
					maxSuitableWord = elem;
				}
			}
			permutationSentence.push_back(maxSuitableWord.first);
			sentenceWeight += maxSuitableWord.second;
		}

		std::sort(permutationSentence.begin(), permutationSentence.end());
		int cnt = 1;
		do {
			std::cout << cnt << ". ";
			for (auto word : permutationSentence) {
				std::cout << word << " ";
			}
			std::cout << "| weight=" << sentenceWeight;
			std::cout << std::endl;
			++cnt;
		} while (std::next_permutation(permutationSentence.begin(), permutationSentence.end()));

		std::cout << std::endl;
		getchar();
	}
	fin.close();
	return 0;
}

int main() {
	return anagram();
}