//
// Created by domokos.balazs on 11/5/2026.
//

#ifndef FORMALIS_NYELVEK_AUTOMATON_MINIMIZER_H
#define FORMALIS_NYELVEK_AUTOMATON_MINIMIZER_H

#include "../problem.hpp"

class Automaton_Minimizer : public Problem {
    void initialize_parser(cxxopts::Options &options) override;
    bool is_chosen_problem(const cxxopts::ParseResult &args) override;
    int run(const cxxopts::ParseResult &args) override;
};


#endif //FORMALIS_NYELVEK_AUTOMATON_MINIMIZER_H
