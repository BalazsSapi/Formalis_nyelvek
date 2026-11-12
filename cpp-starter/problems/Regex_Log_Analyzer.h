#ifndef FORMALIS_NYELVEK_REGEX_LOG_ANALYZER_H
#define FORMALIS_NYELVEK_REGEX_LOG_ANALYZER_H

#include "../problem.hpp"

class Regex_Log_Analyzer : public Problem {
public:
    void initialize_parser(cxxopts::Options &options) override;
    bool is_chosen_problem(const cxxopts::ParseResult &args) override;
    int run(const cxxopts::ParseResult &args) override;
};

#endif //FORMALIS_NYELVEK_REGEX_LOG_ANALYZER_H
