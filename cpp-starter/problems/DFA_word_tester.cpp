//
// Created by domokos.balazs on 9/24/2026.
//

#include "DFA_word_tester.h"

#include <fstream>
#include <iostream>

using namespace std;

// Initialize the parser for the DFA problem
void DFA_word_tester::initialize_parser(cxxopts::Options &options) {
    options.add_options()
            ("check", "Tests the DFA.", cxxopts::value<std::string>());
}

// Check if the DFA problem is chosen
bool DFA_word_tester::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("check") > 0;
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

int DFA_word_tester::run(const cxxopts::ParseResult &args) {
    const auto inputFilename = args["input"].as<string>();
    auto outputFilename = args["output"].as<string>();

    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return 1;
    }

    string line;
    getline(inputFile, line);

    getline(inputFile, line);

    getline(inputFile, line);
    auto startNodeName = line;

    getline(inputFile, line);
    auto endNodeNames = split(line, ' ');

    auto m = map<string, int>();
    auto graph = map<string, map<char, string> >();
    while (getline(inputFile, line)) {
        vector<string> tokens = split(line, ' ');
        if (tokens.size() == 3) {
            graph[tokens[0]][tokens[1][0]] = tokens[2];
        } else {
            cerr << "Error reading input file at the edge lines: " << inputFilename << endl;
            return 1;
        }
    }
    auto wordsLine = args["check"].as<string>();
    auto words = split(wordsLine, ',');

    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }

    for (auto word : words) {
        auto actualNodeName = startNodeName;
        auto ok = true;
        for (auto w : word) {
            if (graph[actualNodeName].find(w) != graph[actualNodeName].end()) {
                actualNodeName = graph[actualNodeName][w];
            }
            else {
                ok = false;
                break;
            }
        }
        if (ok && find(endNodeNames.begin(),endNodeNames.end(),actualNodeName) != endNodeNames.end()) {
            outputFile << "IGEN" << endl;
        }
        else {
            outputFile << "NEM" << endl;
        }
    }
    return 0;
}
