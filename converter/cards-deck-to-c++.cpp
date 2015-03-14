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

    std::string result("{");
    std::string stationars("{");

    std::size_t i = 0;
    std::size_t N = argument.size();
    std::string lastCard;
    while (i < N){
        if (argument[i] == ']' && !lastCard.empty()){
            if (stationars.size() != 1) stationars += ", ";
            stationars += lastCard;
            lastCard.clear();
            ++i;
            continue;
        }
        std::string guessedRank, guessedSuit;
        Guess(argument, i, ranks, guessedRank);
        if (guessedRank.empty()){
            ++i;
            continue;
        }
        Guess(argument, i, suits, guessedSuit);
        if (guessedSuit.empty()){
            ++i;
            continue;
        }
        if (result.size() != 1) result += ", ";
        lastCard = "{" + guessedSuit + ", " + guessedRank + "}";
        result += lastCard;
    }
    std::cout << "Deck:\n";
    std::cout << result << "}\n";
    std::cout << "Stationars:\n";
    std::cout << stationars << "}\n";
    return 0;
}

