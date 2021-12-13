#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <utility>
#include <functional>
#include <limits>

int anagram() {
	std::string line;
	std::ifstream fin;
	std::vector<std::pair<std::string, int>> dict; // словарь со свловами и связанными с ними веса

	// Заполняем словарь	
	fin.open("dictionary.txt");
	while (getline(fin, line)) {
        std::string word = line.substr(0, line.find(','));
        int value = std::stoi(line.substr(line.find(' ') + 1));
        dict.emplace_back(word, value);
    }
	fin.close();
	// TODO file read error exception throw

	/*
	std::cout << "Dictionary:" << std::endl;
	for (auto elem : dict) {
		std::cout << elem.first << " " << elem.second << std::endl;
	}
	std::cout << std::endl;
	*/

	std::vector<std::string> sentenceWords; // вектор слов предложения
	fin.open("input.txt");
	while (getline(fin, line)) {
		// читаем предложение
		
		sentenceWords.clear();
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); // Windows endline fix
		//std::cout << "'" << line << "'" << std::endl;
		//getchar();
		
		// разбиваем предлодение по словам и заполняем вектор sentenceWords
		
		std::string word;
		std::stringstream sstream(line);
		while (getline(sstream, word, ' ')) {
			sentenceWords.push_back(word);
		}
		/*
		std::cout << "Splitting by word: ";
		for (auto word : sentenceWords) {
			std::cout << "!" << word << "!";
		}
		std::cout << std::endl;
		getchar();
		*/


		// Вывод всех престановок для слова, найденных в словаре
		// TODO вывести ошибку, если слово не найдено

		std::map<
			std::string,
			std::vector<std::pair<std::string, int>>
		> wordsPermutations; // найденные анаграммы для каждого слова в заданном предложении

		for (auto word : sentenceWords) {
			std::cout << "Permutations for " << word << ": ";
			wordsPermutations.insert(std::pair<std::string, std::vector<std::pair<std::string, int>>>(word, {}));
			for (auto matchElem : dict) {
				if (std::is_permutation(word.begin(), word.end(), matchElem.first.begin(), matchElem.first.end())) {
					std::cout << matchElem.first << " weight=" << matchElem.second << ", ";
					wordsPermutations.at(word).emplace_back(matchElem.first, matchElem.second);
				}
			}
			std::cout << std::endl;
			
			std::sort(wordsPermutations.at(word).begin(), wordsPermutations.at(word).end(), 
					[](std::pair<std::string, int> lhs, std::pair<std::string, int> rhs) { return lhs.second > rhs.second; });
		}

		/*
		for (auto it = wordsPermutations.begin(); it != wordsPermutations.end(); ++it) {
			std::cout << "Map for " << it->first << ": ";
			for (auto elem : it->second) {
				std::cout << elem.first << " weight=" << elem.second << ", ";
			}
			std::cout << "VECTOR SIZE=" << it->second.size();
			std::cout << std::endl;
		}
		*/
		std::cout << std::endl;
		getchar();
		


		std::vector<size_t> availableIdx(sentenceWords.size(), 0); // индексы валидных перестановок
		int cnt = 1; // счетчик перестановок
		do {
			std::vector<std::string> permutationSentence; // вектор анаграм, составленное в предложение для перестановок
			std::vector<int> permutationSentenceWeights(sentenceWords.size(), 0); // вес каждой анаграммы в предложении
			
			/*
			std::cout << "Avaliable: ";
			for (auto num : availableIdx) {
				std::cout << num << " ";
			}
			std::cout << std::endl;
			getchar();
			*/

			int sentenceWeight = 0; // вес текущего предложения
			for (size_t i = 0; i < sentenceWords.size(); ++i) {
				/*
				std::cout << wordsPermutations.at(sentenceWords[i])[availableIdx[i]].first << " "
						  << wordsPermutations.at(sentenceWords[i])[availableIdx[i]].second << std::endl;
				*/
				permutationSentence.push_back(wordsPermutations.at(sentenceWords[i])[availableIdx[i]].first);
				permutationSentenceWeights[i] = wordsPermutations.at(sentenceWords[i])[availableIdx[i]].second;
				//getchar();
			}
			for (auto num : permutationSentenceWeights) {
				//std::cout << num << " ";
				sentenceWeight += num;
			}
			//std::cout << std::endl;
			
			std::sort(permutationSentence.begin(), permutationSentence.end());
			
			// ищем все перестановки permutationSentence

			do {
				std::cout << cnt << ". ";
				for (auto word : permutationSentence) {
					std::cout << word << " ";
				}
				std::cout << "| weight=" << sentenceWeight;
				std::cout << std::endl;
				++cnt;
			} while (std::next_permutation(permutationSentence.begin(), permutationSentence.end()));
			
			/**
			 * Рассматриваем валидные индексы:
			 * Пробуем прибавить +1 к availableIdx[0] и смотрим, чему индекс равен по модулю (размер вектора найденных слов-анаграмм).
			 * 
			 * Если он оказался равен 0 по модулю, прибавляем к следующему +1 и так до тех пор,
			 * пока мы не переберем все доступные варианты и вернемся к ситуации 00...0.
			 * 
			 * В таком случае мы перебрали все возможные предложения в порядке убывания их общих весов.
			 */

			size_t k = 0;
			for(;;) {
				/*
				std::cout << "NOW avaliable: ";
				for (auto num : availableIdx) {
					std::cout << num << " ";
				}
				std::cout << "k=" << k << std::endl;
				getchar();
				*/
				if ((availableIdx[k] + 1) % wordsPermutations.at(sentenceWords[k]).size() != 0) {
					/*
					std::cout << "OK" << std::endl;
					getchar();
					*/
					availableIdx[k] += 1;
					break;
				}
				else {
					availableIdx[k] = 0;
					if (k < availableIdx.size()) {
						++k;
						if (k == availableIdx.size()) {
							//std::cout << "DROP" << std::endl;
							break;
						}
						//std::cout << "Moving to k=" << k << std::endl;
						//getchar();
					}
				}
			}
		} while (!(all_of(availableIdx.begin(), availableIdx.end(), [&] (int i) {return i == availableIdx[0];}))); // all are the same, 00...0
		std::cout << std::endl;
		getchar();
	}
	fin.close();
	return 0;
}

int main() {
	return anagram();
}