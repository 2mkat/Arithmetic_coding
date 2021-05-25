#include <iostream> 
#include <vector>
#include <map>
#include <list>
#include <algorithm>

#include "header.cpp"

void write_bit_to_file(std::ostream& g, Variable& v, int& count) {
    count = 0;
    g << v.temp;
    v.temp = 0;
}

void add_bits(std::ostream& g, Variable& v, int& count) {
    v.temp = v.temp | (1 << (7 - count));
    count++;
    if(count == 8){
        write_bit_to_file(g, v, count);
    }
}

void check_count(std::ostream& g, Variable& v, int& count) {
    count++;
    if(count == 8){
        write_bit_to_file(g, v, count);
    }
}

//procedure for transferring the found bits to left_edge file
void bits_plus_follow(int bit, int &count, std::ofstream &g, Variable& v) {
    if(bit == 0){
        check_count(g, v, count);
        while(v.bits_to_follow > 0){
            add_bits(g, v, count);
            v.bits_to_follow--;
        }
    }
    else if(bit == 1) {
        add_bits(g, v, count);
        while(v.bits_to_follow > 0) {
            check_count(g, v, count);
            v.bits_to_follow--;
        }
    }
}
void compress_data(std::ifstream& file,  std::ofstream& res_file, std::list<Table>& table) {
    Variable v;
    int denom = table.back().right_edge, low = 0;  // low - нижняя граница отрезка, denom - делитель
    char temp = 0;
    int n_symbols = 0;
    auto it = table.begin();    // итератор для для списка

    file.clear();   // сбрасываем указатель в начало файла
    file.seekg(0);

    while(!file.eof()){
        char c = file.get();

        /*it = std::find_if(table.begin(), table.end(), [c](const Table& c1){return c1.symbol == c;});
        if (it != table.end()) {
            continue;
        }
        else {
            std::cout << "Element not found in table\n";
        }*/

        for(it = table.begin(); it != table.end(); it++){
            if(c == it->symbol){
                break;
            }
        }
        if(c != it->symbol){
            std::cout << "Error!" << '\n';
            break;
        }

        int l2 = low;
        low = low + it->left_edge * (v.high - low + 1) / denom;
        v.high = l2 + it->right_edge * (v.high - l2 + 1) / denom - 1;
        while(true) {    // обрабатываем варианты
            if(v.high < v.half){   //переполнение
                bits_plus_follow(0, n_symbols, res_file, v);
            }
            else if(low >= v.half){
                bits_plus_follow(1, n_symbols, res_file, v);
                low -= v.half;
                v.high -= v.half;
            }
            else if((low >= v.first_qtr) && (v.high < v.third_qtr)){
                v.bits_to_follow++;
                low -= v.first_qtr;
                v.high -= v.first_qtr;
            }
            else{
                break;
            }
            low += low;
            v.high += v.high + 1;
        }
    }
    res_file << temp;
}

int main(){
    // open files
    std::ifstream file("input.txt", std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Uh oh, input.txt could not be opened for reading!" << '\n';
    }
    std::ofstream res_file("output.txt", std::ios::out | std::ios::binary);
    if(!res_file) {
        std::cerr << "Uh oh, output.txt could not be opened for writing!" << '\n';
    }

    //count symbols in the text
    std::map <char, int> frequencies;
    while(!file.eof()){
        char c = file.get();
        frequencies[c]++;
    }

    //create table with intervals
    std::list<Table> table = build_table(frequencies);

    // write header
    write_to_file(res_file, frequencies.size(), frequencies);

    // start compress file
    compress_data(file, res_file, table);

    file.close();
    res_file.close();

    return 0;
}
