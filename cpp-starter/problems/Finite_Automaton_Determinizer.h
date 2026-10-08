//
// Created by domokos.balazs on 10/8/2026.
//

#ifndef PROJECT_FINITE_AUTOMATON_DETERMINIZER_H
#define PROJECT_FINITE_AUTOMATON_DETERMINIZER_H

#include "../problem.hpp"

class Finite_Automaton_Determinizer {
    void initialize_parser(cxxopts::Options &options) override;
    bool is_chosen_problem(const cxxopts::ParseResult &args) override;
    int run(const cxxopts::ParseResult &args) override;
};


#endif //PROJECT_FINITE_AUTOMATON_DETERMINIZER_H
