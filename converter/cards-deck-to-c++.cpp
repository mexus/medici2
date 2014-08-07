#include <iostream>
#include <string>
#include <map>

std::map<std::string, std::string> suits{
	{"п", "Spades"},
	{"ч", "Hearts"},
	{"б", "Diamonds"},
	{"к", "Clubs"}
};	

std::map<std::string, std::string> ranks{
	{"6", "Six"},
	{"7", "Seven"},
	{"8", "Eight"},
	{"9", "Nine"},
	{"X", "Ten"},
	{"В", "Jack"},
	{"Д", "Queen"},
	{"К", "King"},
	{"Т", "Ace"}
};

void Guess(const std::string& line, std::size_t &pos, const std::map<std::string, std::string>& map, std::string& result){
	if (pos > line.size())
		return ;
	for (auto &pair : map){
		auto &name = pair.first;
		if (line.compare(pos, name.size(), name) == 0){
			result = pair.second;
			pos += name.size();
			break;
		}
	}
}

int main(int argc, char **argv){
	std::string argument;
	for (int i = 1; i < argc; ++i){
		if (i != 1)
			argument += ' ';
		argument += argv[i];
	}
	std::size_t N = argument.size();
	std::string result("{");
	bool first(true);
	for (std::size_t i = 0; i < N; ++i){
		std::string guessedRank, guessedSuit;
		Guess(argument, i, ranks, guessedRank);
		if (guessedRank.empty()){
			std::cout << "Can't find a rank at " << "(" << argument.substr(i, 9) << ")" << "\n";
			break ;
		}
		Guess(argument, i, suits, guessedSuit);
		if (guessedSuit.empty()){
			std::cout << "Can't find a suit at " << "(" << argument.substr(i, 9) << ")" << "\n";
			break ;
		}

		if (first) first = false; else result += ", ";
		result += "{" + guessedRank + ", " + guessedSuit + "}";
	}
	std::cout << result << "}\n";
	return 0;
}

