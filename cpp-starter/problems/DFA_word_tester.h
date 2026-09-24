//
// Created by domokos.balazs on 9/24/2026.
//

#ifndef PROJECT_DFA_WORD_TESTER_H
#define PROJECT_DFA_WORD_TESTER_H

#include "../problem.hpp"

class DFA_word_tester : public Problem {
    void initialize_parser(cxxopts::Options &options) override;
    bool is_chosen_problem(const cxxopts::ParseResult &args) override;
    int run(const cxxopts::ParseResult &args) override;
};


#endif //PROJECT_DFA_WORD_TESTER_H
