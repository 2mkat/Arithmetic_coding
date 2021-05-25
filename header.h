#pragma once

#include <iostream>

struct Table{
    char symbol;
    int amount_s;
    int left_edge;
    int right_edge;
};

class Variable {
public:
    int high;
    int first_qtr, half, third_qtr, bits_to_follow;   // в bits_to_follow сколько битов сбрасывать
    char temp;

    Variable();
};

void write_to_file(std::ofstream&, int, std::map<char, int>);
bool comp(Table, Table);
void check_symbol(std::list<Table> &table, char c, std::list<Table>::iterator &it);
std::list<Table> build_table(std::map<char, int> frequencies);
std::map<char, int> read_from_file(std::ifstream&);
