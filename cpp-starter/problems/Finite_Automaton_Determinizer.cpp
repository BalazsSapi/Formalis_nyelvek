//
// Created by domokos.balazs on 10/8/2026.
//

#include "Finite_Automaton_Determinizer.h"

#include <fstream>
#include <iostream>
#include <set>

using namespace std;

// Initialize the parser for the Determinizer problem
void Finite_Automaton_Determinizer::initialize_parser(cxxopts::Options &options) {
    options.add_options()
            ("det", "Tests the DFA.", cxxopts::value<std::string>());
}

// Check if the Determinizer problem is chosen
bool Finite_Automaton_Determinizer::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("det") > 0;
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

set<string> split_set(const string &s, char delimiter) {
    set<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.emplace(token);
    }
    return tokens;
}

int Finite_Automaton_Determinizer::run(const cxxopts::ParseResult &args) {
    const auto inputFilename = args["input"].as<string>();
    auto outputFilename = args["output"].as<string>();

    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return 1;
    }

    string line;
    getline(inputFile, line);
    auto nodeNames = split_set(line, ' ');

    getline(inputFile, line);
    auto abcCharacters = split_set(line, ' ');

    getline(inputFile, line);
    auto startNodeName = line;

    getline(inputFile, line);
    auto endNodeNames = split_set(line, ' ');

    auto m = map<string, int>();
    unordered_map<string, unordered_map<string, set<string>>> graph;
    while (getline(inputFile, line)) {
        vector<string> tokens = split(line, ' ');
        if (tokens.size() == 3 && nodeNames.count(tokens[0]) == 1 && nodeNames.count(tokens[2]) == 1 && abcCharacters.count(tokens[1]) == 1) {
            graph[tokens[0]][tokens[1]].insert(tokens[2]);
        } else {
            cerr << "Error reading input file at the edge lines: " << inputFilename << endl;
            return 1;
        }
    }

    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }

    map<set<string>, map<string, set<string>>> newAutomaton;
    vector<set<string>> orderOfNewNodes;

    orderOfNewNodes.push_back({startNodeName});

    int actualNewNodeIndex = 0;
    while (actualNewNodeIndex < orderOfNewNodes.size()) {
        for (const auto& actualHeader : orderOfNewNodes[actualNewNodeIndex]) {
            for (const auto& character : abcCharacters) {
                for (const auto& node : graph[actualHeader][character]) {
                    newAutomaton[orderOfNewNodes[actualNewNodeIndex]][character].insert(node);
                    if (endNodeNames.count(node) == 1) {
//! pair kulcsú map C++ a gemini-ban. Az utolsó dolognál meg kellene nézni a választ.
                    }
                }
            }
        }
        for (auto character : abcCharacters) {
            if (find(orderOfNewNodes.begin(), orderOfNewNodes.end(), newAutomaton[orderOfNewNodes[actualNewNodeIndex]][character]) == orderOfNewNodes.end()) {
                orderOfNewNodes.push_back(newAutomaton[orderOfNewNodes[actualNewNodeIndex]][character]);
//!
            }
        }
        actualNewNodeIndex++;
    }
    return 0;
}
