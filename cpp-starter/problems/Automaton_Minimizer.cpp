//
// Created by domokos.balazs on 11/5/2026.
//

#include "Automaton_Minimizer.h"
#include <fstream>
#include <iostream>
#include <set>

using namespace std;

// Initialize the parser for the Minimizer problem
void Automaton_Minimizer::initialize_parser(cxxopts::Options &options) {
    options.add_options()
            ("mini", "Tests the Minimalization.", cxxopts::value<bool>());
}

// Check if the Minimizer problem is chosen
bool Automaton_Minimizer::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("mini") > 0;
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

int Automaton_Minimizer::run(const cxxopts::ParseResult &args) {
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

    map<string, map<string, bool> > starNodePairs;
    int idx = 0;
    map<string, int> indexOf;
    vector<string> names;
    for (auto it1 = nodeNames.begin(); it1 != nodeNames.end(); it1++) {
        indexOf[*it1] = idx++;
        names.push_back(*it1);
        for (auto it2 = next(it1); it2 != nodeNames.end(); it2++) {
            starNodePairs[*it1][*it2] = (endNodeNames.count(*it1) != endNodeNames.count(*it2));
        }
    }

    unordered_map<string, unordered_map<string, string> > graph;
    while (getline(inputFile, line)) {
        vector<string> tokens = split(line, ' ');
        if (tokens.size() == 3 && nodeNames.count(tokens[0]) == 1 && nodeNames.count(tokens[2]) == 1 && abcCharacters.
            count(tokens[1]) == 1) {
            graph[tokens[0]][tokens[1]] = tokens[2];
        } else {
            cerr << "Error reading input file at the edge lines: " << inputFilename << endl;
            return 1;
        }
    }

    bool change;
    vector<int> united;
    do {
        change = false;
        united = vector<int>(nodeNames.size(), -1);
        for (auto &row: starNodePairs) {
            for (auto &cell: row.second) {
                if (cell.second == true) {
                    continue;
                }
                for (auto c: abcCharacters) {
                    if (graph[row.first][c] < graph[cell.first][c]) {
                        if (starNodePairs[graph[row.first][c]][graph[cell.first][c]] == true) {
                            cell.second = true;
                            change = true;
                            break;
                        }
                    } else if (graph[row.first][c] > graph[cell.first][c]) {
                        if (starNodePairs[graph[cell.first][c]][graph[row.first][c]] == true) {
                            cell.second = true;
                            change = true;
                            break;
                        }
                    }
                }
                if (united[indexOf[cell.first]] == -1) {
                    united[indexOf[cell.first]] = indexOf[row.first];
                }
            }
        }
    } while (change == true);


    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }

    for (int i = 0; i < nodeNames.size(); i++) {
        if (united[i] == -1) {
            outputFile << "s" << i << " ";
        }
    }
    outputFile << "\n";

    for (const auto &c: abcCharacters) {
        outputFile << c << " ";
    }
    outputFile << "\n";

    if (united[indexOf[startNodeName]] == -1) {
        outputFile << "s" << indexOf[startNodeName] << "\n";
    } else {
        outputFile << "s" << united[indexOf[startNodeName]] << "\n";
    }


    for (int i = 0; i < nodeNames.size(); i++) {
        if (united[i] == -1 && endNodeNames.count(names[i]) == 1) {
            outputFile << "s" << i << " ";
        }
    }
    outputFile << "\n";

    // Transitions: only from representatives, so there are no duplicates
    for (int i = 0; i < nodeNames.size(); i++) {
        if (united[i] == -1) {
            for (const auto &c: abcCharacters) {
                outputFile << "s" << i << " " << c << " ";
                if (united[indexOf[graph[names[i]][c]]] == -1) {
                    outputFile << "s" << indexOf[graph[names[i]][c]] << "\n";
                }
                else {
                    outputFile << "s" << united[indexOf[graph[names[i]][c]]] << "\n";
                }
            }
        }
    }
    return 0;
}
