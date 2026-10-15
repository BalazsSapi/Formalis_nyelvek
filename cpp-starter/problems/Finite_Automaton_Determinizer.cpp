//
// Created by domokos.balazs on 10/8/2026.
//

#include "Finite_Automaton_Determinizer.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

using namespace std;

// Initialize the parser for the Determinizer problem
void Finite_Automaton_Determinizer::initialize_parser(cxxopts::Options &options) {
    options.add_options()
            ("det", "Tests the DFA.", cxxopts::value<bool>());
}

// Check if the Determinizer problem is chosen
bool Finite_Automaton_Determinizer::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("det") > 0;
}

static vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

static set<string> split_set(const string &s, char delimiter) {
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
    unordered_map<string, unordered_map<string, set<string> > > graph;
    while (getline(inputFile, line)) {
        vector<string> tokens = split(line, ' ');
        if (tokens.size() == 3 && nodeNames.count(tokens[0]) == 1 && nodeNames.count(tokens[2]) == 1 && abcCharacters.
            count(tokens[1]) == 1) {
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

    map<set<string>, map<string, set<string> > > newAutomaton;
    vector<set<string> > orderOfNewNodes;
    map<set<string>, bool> isEndNode;

    orderOfNewNodes.push_back({startNodeName});

    isEndNode[{startNodeName}] = endNodeNames.count(startNodeName) == 1;


    int actualNewNodeIndex = 0;
    while (actualNewNodeIndex < orderOfNewNodes.size()) {
        const set<string> current = orderOfNewNodes[actualNewNodeIndex];
        map<string, set<string> > targets;
        map<string, bool> targetHasEnd;

        for (const auto &header: current) {
            for (const auto &character: abcCharacters) {
                for (const auto &node: graph[header][character]) {
                    targets[character].insert(node);
                    if (endNodeNames.count(node) == 1) {
                        targetHasEnd[character] = true;
                    }
                }
            }
        }
        for (const auto &character: abcCharacters) {
            const auto &target = targets[character];
            if (target.empty()) continue;
            newAutomaton[current][character] = target;

            if (isEndNode.count(target) == 0) {
                isEndNode[target] = targetHasEnd[character];
                orderOfNewNodes.push_back(target);
            }
        }
        actualNewNodeIndex++;
    }


    map<set<string>, int> idx;
    for (int i = 0; i < orderOfNewNodes.size(); i++) idx[orderOfNewNodes[i]] = i;

    // 1. sor: állapotok
    for (int i = 0; i < orderOfNewNodes.size(); i++) {
        if (i > 0) outputFile << " ";
        outputFile << "s" << i;
    }
    outputFile << "\n";

    // 2. sor: ábécé
    bool first = true;
    for (const auto &character: abcCharacters) {
        if (!first) outputFile << " ";
        outputFile << character;
        first = false;
    }
    outputFile << "\n";

    // 3. sor: kezdőállapot
    outputFile << "s0\n";

    // 4. sor: végállapotok
    first = true;
    for (int i = 0; i < orderOfNewNodes.size(); i++) {
        if (isEndNode[orderOfNewNodes[i]]) {
            if (!first) outputFile << " ";
            outputFile << "s" << i;
            first = false;
        }
    }
    outputFile << "\n";

    // átmenetek:
    for (int i = 0; i < orderOfNewNodes.size(); i++) {
        for (const auto& [character, target] : newAutomaton[orderOfNewNodes[i]]) {
            outputFile << "s" << i << " " << character << " s" << idx[target] << "\n";
        }
    }
    return 0;
}
