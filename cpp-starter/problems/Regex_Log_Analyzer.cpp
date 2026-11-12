#include "Regex_Log_Analyzer.h"

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Initialize the parser for the Minimizer problem
void Regex_Log_Analyzer::initialize_parser(cxxopts::Options &options) {
    options.add_options()
            ("rgx", "Runs the regex search task with the given number.", cxxopts::value<int>());
}

// Check if the Regex problem is chosen
bool Regex_Log_Analyzer::is_chosen_problem(const cxxopts::ParseResult &args) {
    return args.count("rgx") > 0;
}

static const regex PID_OF_LINE(R"(^[0-9-]+ [0-9:.]+ +([0-9]+) +[0-9]+ [IDVEFAW] )");

static const regex ERROR_LINE(R"(^[0-9-]+ [0-9:.]+ +([0-9]+) +[0-9]+ E )");

static const regex JAVA_FILE(R"((\w+\.java):)");

static const regex MAIN_THREAD_ERROR(R"(^[0-9-]+ [0-9:.]+ +([0-9]+) +\1 E )");

static void task1(const vector<string> &lines, ostream &out) {
    const vector<regex> patterns = {
        regex(R"(^10-15 )"),
        regex(R"(^10-15 10:18:51\.)"),
        regex(R"(^[0-9-]+ [0-9]+:[0-9]+:51\.)"),
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ [IDVEFAW] lowmemorykiller:)"),
        regex(R"(^[0-9-]+ 10:18:51\.[0-9]+ +221 )"),
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ E )"), // errors
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ W PackageManager:)"),
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ D ExoPlayer[^:]*:)"),
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ [EW] .*\.java:)"),
        regex(R"(^[0-9-]+ [0-9:.]+ +[0-9]+ +[0-9]+ [IDVEFAW] [^:]*: .*Thread)"),
    };

    vector<int> counts(patterns.size(), 0);
    for (const auto &line: lines) {
        for (size_t i = 0; i < patterns.size(); i++) {
            if (regex_search(line, patterns[i])) {
                counts[i]++;
            }
        }
    }
    for (int count: counts) {
        out << count << "\n";
    }
}

static void task2(const vector<string> &lines, ostream &out) {
    for (const auto &line: lines) {
        if (!regex_search(line, ERROR_LINE)) continue;
        for (sregex_iterator it(line.begin(), line.end(), JAVA_FILE), end; it != end; ++it) {
            out << (*it)[1] << "\n";
        }
    }
}

static void task3(const vector<string> &lines, ostream &out) {
    set<string> seen;
    smatch match;
    for (const auto &line: lines) {
        if (!regex_search(line, match, PID_OF_LINE)) continue;
        const string pid = match[1].str();
        if (seen.insert(pid).second) {
            out << pid << "\n";
        }
    }
}

static void task4(const vector<string> &lines, ostream &out) {
    map<string, int> errors;
    vector<string> order; // pids in order of their first error
    smatch match;
    for (const auto &line: lines) {
        if (!regex_search(line, match, ERROR_LINE)) continue;
        const string pid = match[1].str();
        if (++errors[pid] == 1) {
            order.push_back(pid);
        }
    }
    if (order.empty()) return;

    string best = order.front();
    for (const auto &pid: order) {
        if (errors[pid] > errors[best]) {
            best = pid;
        }
    }
    out << "PID=" << best << " (" << errors[best] << " errors)\n";
}

static void task5(const vector<string> &lines, ostream &out) {
    int count = 0;
    for (const auto &line: lines) {
        if (regex_search(line, MAIN_THREAD_ERROR)) {
            count++;
        }
    }
    out << count << "\n";
}

int Regex_Log_Analyzer::run(const cxxopts::ParseResult &args) {
    const auto inputFilename = args["input"].as<string>();
    const auto outputFilename = args["output"].as<string>();
    const int task = args["rgx"].as<int>();

    ifstream inputFile(inputFilename);
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }

    switch (task) {
        case 1: task1(lines, outputFile);
            break;
        case 2: task2(lines, outputFile);
            break;
        case 3: task3(lines, outputFile);
            break;
        case 4: task4(lines, outputFile);
            break;
        case 5: task5(lines, outputFile);
            break;
        default:
            cerr << "Unknown regex task number: " << task << endl;
            return 1;
    }
    return 0;
}
