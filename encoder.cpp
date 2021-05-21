#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

int h = 65535;
int first_qtr = (h + 1) / 4, half = first_qtr * 2, third_qtr = first_qtr * 3, bits_to_follow = 0;   // в bits_to_follow сколько битов сбрасывать
char temp = 0;

struct Table{
    char s;
    int numb;
    int lb;
    int rb;
};

bool comp(Table l, Table r){
    return l.numb > r.numb;
}

//procedure for transferring the found bits to a file
void bits_plus_follow(int bit, int &count, std::ofstream &res_file){
    if(bit == 0){
        count++;
        if(count == 8){
            count = 0;
            res_file << temp;
            temp = 0;
        }
        for(; bits_to_follow > 0; bits_to_follow--){
            temp = temp | (1 << (7 - count));
            count++;
            if(count == 8){
                count = 0;
                res_file << temp;
                temp = 0;
            }
        }
    }
    else if(bit == 1) {
        temp = temp | (1 << (7 - count));
        count++;
        if (count == 8) {
            count = 0;
            res_file << temp;
            temp = 0;
        }
        for (; bits_to_follow > 0; bits_to_follow--) {
            count++;
            if (count == 8) {
                count = 0;
                res_file << temp;
                temp = 0;
            }
        }
    }
}

int main(){
    int n_symbols = 0;

    // open files
    std::ifstream file("code.txt", std::ios::out | std::ios::binary);
    if (!file){
        std::cerr << "Uh oh, Text.txt could not be opened for reading!" << '\n';
    }
    std::ofstream res_file("output.txt", std::ios::out | std::ios::binary);
    if(!res_file){
        std::cerr << "Uh oh, output.txt could not be opened for writing!" << '\n';
    }

    //count symbols in the text
    std::map <char, int> frequencies;
    std::map <char, int> ::iterator iterator;
    while(!file.eof()){
        char c = file.get();
        frequencies[c]++;
        n_symbols++;
    }

    //create table with intervals
    std::list<Table> table;
    for(iterator = frequencies.begin(); iterator != frequencies.end(); iterator++){
        Table t;
        t.s = iterator->first;
        t.numb = iterator->second;
        table.push_back(t);
    }
    table.sort(comp);
    table.begin()->rb = table.begin()->numb;
    table.begin()->lb = 0;
    std::list<Table>::iterator it = table.begin(), i2;
    i2 = it;
    it++;
    for(; it != table.end(); it++){
        it->lb = i2->rb;
        it->rb = it->lb + it->numb;
        i2++;
    }
    
    //get information about symbols
    int count=0;
    for (iterator = frequencies.begin(); iterator != frequencies.end(); iterator++){
		if (iterator->second != 0){
		    ++count;
		}
    }

    // write header
	res_file.write((char*)(&count), sizeof(count));
	for (int k=0; k<256; k++){
		if (frequencies[char(k)] > 0){
			char c=char(k);
			res_file.write((char*)(&c), sizeof(c));
			res_file.write((char*)(&frequencies[char(k)]), sizeof(frequencies[char(k)]));
		}
	}
    file.clear();
	file.seekg(0);

	// start compress file
    int idx = 0, denom = table.back().rb, l = 0;
    char temp = 0;
    n_symbols = 0;
    
    while(!file.eof()){
        char c = file.get(); idx++;
        for(it = table.begin(); it != table.end(); it++){
            if(c == it->s){
                break;
            }
        }
        if(c != it->s){
            std::cout << "Error!" << std::endl;
            break;
        }
        int l2 = l;
        l = l + it->lb * (h - l + 1) / denom;
        h = l2 + it->rb * (h - l2 + 1) / denom - 1;
        for(;;){    // обрабатываем варианты
            if(h < half){   //переполнение
                bits_plus_follow(0, n_symbols, res_file);
            }
            else if(l >= half){
                bits_plus_follow(1, n_symbols, res_file);
                l -= half;
                h -= half;
            }
            else if((l >= first_qtr) && (h < third_qtr)){
                bits_to_follow++;
                l -= first_qtr;
                h -= first_qtr;
            }
            else{
                break;
            }
            l += l;
            h += h + 1;
        }
    }
    res_file << temp;
    
    file.close();
    res_file.close();
    
    return 0;
}
