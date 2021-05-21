#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

int h = 65535;
int first_qtr = (h + 1) / 4, half = first_qtr * 2, third_qtr = first_qtr * 3;

struct Table{
    char s;
    int n_s;
    int a;
    int b;
};

bool comp(Table l, Table r){
    return l.n_s > r.n_s;
}

int main(){
    int n_symbols = 0;

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
    int x1, x2;
	char s;
	map<char, int> m;
    map <char, int> ::iterator iterator;

	file.read((char*)&x1, sizeof(x1));
    while (x1 > 0){
		file.read((char*)&s, sizeof(s));
		file.read((char*)&x2, sizeof(x2));
		--x1;
		m[s] = x2;
    }

    // building table
    list<Table> table;
    for(iterator = m.begin(); iterator != m.end(); iterator++){
        Table t;
        t.s = iterator->first;
        t.n_s = iterator->second;
        table.push_back(t);
    }
    table.sort(comp);
    table.begin()->b = table.begin()->n_s;
    table.begin()->a = 0;
    list<Table>::iterator it = table.begin(), it2;
    it2 = it;
    it++;
    for(; it != table.end(); it++){
        it->a = it2->b;
        it->b = it->a + it->n_s;
        it2++;
    }

    // decompressor
    n_symbols = 0;
    int l = 0, denom = table.back().b;

    int value = (file.get() << 8) | file.get();
    char c = file.get();
    while(!file.eof()){
        int freq = ((value - l + 1) * denom - 1) / (h - l + 1);
        for(it = table.begin(); it->b <= freq; it++);   // поиск слова
        int l2 = l;
        l = l + (it->a) * (h - l + 1) / denom;
        h = l2 + (it->b) * (h - l2 + 1) / denom - 1;
        for(;;){    // обработка вариантов
            if(h < half);   // если переполнение, ничего не делаем
            else if(l >= half){
                l -= half;
                h -= half;
                value -= half;
            }
            else if((l >= first_qtr) && (h < third_qtr)){
                l -= first_qtr;
                h -= first_qtr;
                value -= first_qtr;
            } else{
                break;
            }
            l += l;
            h += h + 1;
            value += value + (((short)c >> (7 - n_symbols)) & 1);
            n_symbols++;
            if(n_symbols == 8){
                c = file.get();
                n_symbols = 0;
            }
        }
        res_file << it->s;
    }

    file.close();
    res_file.close();
    return 0;
}
