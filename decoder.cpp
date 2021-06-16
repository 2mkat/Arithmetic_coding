#include <iostream>
#include <map>
#include <list>

#include "header.cpp"

void decompress_data(std::ifstream& file,  std::ofstream& res_file, std::list<Table>& table) {
    auto it = table.begin();
    Variable v;
    int n_symbols = 0;
    int l = 0, denom = table.back().right_edge;

    int value = (file.get() << 8) | file.get();
    char c = file.get();
    while(!file.eof()){
        int freq = ((value - l + 1) * denom - 1) / (v.high - l + 1);
        for(it = table.begin(); it->right_edge <= freq; it++);   // поиск слова
        int l2 = l;
        l = l + (it->left_edge) * (v.high - l + 1) / denom;
        v.high = l2 + (it->right_edge) * (v.high - l2 + 1) / denom - 1;
        for(;;){    // обработка вариантов
            if(v.high < v.half);   // если переполнение, ничего не делаем
            else if(l >= v.half){
                l -= v.half;
                v.high -= v.half;
                value -= v.half;
            }
            else if((l >= v.first_qtr) && (v.high < v.third_qtr)){
                l -= v.first_qtr;
                v.high -= v.first_qtr;
                value -= v.first_qtr;
            } else{
                break;
            }
            l += l;
            v.high += v.high + 1;
            value += value + (((short)c >> (7 - n_symbols)) & 1);
            n_symbols++;
            if(n_symbols == 8){
                c = file.get();
                n_symbols = 0;
            }
        }
        res_file << it->symbol;
    }
}

int main(){

    // open files
    std::ifstream file("output.txt", std::ios::out | std::ios::binary);
    if (!file){
        std::cerr << "Uh oh, output.txt could not be opened for reading!" << '\n';
    }
    std::ofstream res_file("res.txt", std::ios::out | std::ios::binary);
    if(!res_file){
        std::cerr << "Uh oh, res.txt could not be opened for writing!" << '\n';
    }

    // read symbols for freq table
    std::map<char, int> m = read_from_file(file);

    // building table
    std::list<Table> table = build_table(m);

    // decompressor
    decompress_data(file, res_file, table);

    file.close();
    res_file.close();
    return 0;
}
