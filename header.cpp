#include <iostream>
#include <fstream>

#include "header.h"

Variable::Variable() {
    high = 65535;
    first_qtr = (high + 1) / 4;
    half = first_qtr * 2;
    third_qtr = first_qtr * 3;
    bits_to_follow = 0;
    temp = 0;
}   // default constructor


void write_to_file(std::ofstream &res_file, int count, std::map <char, int> frequencies) {
    res_file.write((char*)(&count), sizeof(count));
    for (int k=0; k<256; k++){
        if (frequencies[char(k)] > 0){
            char c=char(k);
            res_file.write((char*)(&c), sizeof(c));
            res_file.write((char*)(&frequencies[char(k)]), sizeof(frequencies[char(k)]));
        }
    }
}

bool comp(Table l, Table r) {
    return l.amount_s > r.amount_s;
}

int count_symbols(std::map <char, int> frequencies) {
    int count = 0;
    std::map <char, int>::iterator iterator;

    for (iterator = frequencies.begin(); iterator != frequencies.end(); iterator++){
        if (iterator->second != 0){
            ++count;
        }
    }

    return count;
}

std::list<Table> build_table(std::map<char, int> frequencies) {
    std::list<Table> table;
    std::map <char, int>::iterator iterator;

    for(iterator = frequencies.begin(); iterator != frequencies.end(); iterator++) {
        Table t;
        t.symbol = iterator->first;
        t.amount_s = iterator->second;
        table.push_back(t);
    }

    table.sort(comp);
    table.begin()->right_edge = table.begin()->amount_s;
    table.begin()->left_edge = 0;
    std::list<Table>::iterator it = table.begin(), it2;
    it2 = it;
    it++;
    for(; it != table.end(); it++){
        it->left_edge = it2->right_edge;
        it->right_edge = it->left_edge + it->amount_s;
        it2++;
    }

    return table;
}

std::map<char, int> read_from_file(std::ifstream& file) {
    int count, sym;
    char s;
    std::map<char, int> m;

    file.read((char*)&count, sizeof(count));
    while (count > 0){
        file.read((char*)&s, sizeof(s));
        file.read((char*)&sym, sizeof(sym));
        --count;
        m[s] = sym;
    }

    return m;
}