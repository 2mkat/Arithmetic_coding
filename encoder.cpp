#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

int h = 65535;
int first_qtr = (h + 1) / 4, half = first_qtr * 2, third_qtr = first_qtr * 3, bits_to_follow = 0;   // в bits_to_follow сколько битов сбрасывать
char temp = 0;

struct Range{
    char s;
    int numb;
    int lb;
    int rb;
};

struct Sort{;//сравнивает 2, если Sort=1 то левый меньше правого и если 0, то наоборот
	bool operator() (Range l, Range r){
		return l.numb > r.numb;
	}
};

void bits_plus_follow(int bit, int &count, ofstream &g){    //процедура переноса найденных битов в файл
    if(bit == 0){
        count++;
        if(count == 8){
            count = 0;
            g << temp;
            temp = 0;
        }
        for(; bits_to_follow > 0; bits_to_follow--){
            temp = temp | (1 << (7 - count));
            count++;
            if(count == 8){
                count = 0;
                g << temp;
                temp = 0;
            }
        }
    }
    else if(bit == 1) {
        temp = temp | (1 << (7 - count));
        count++;
        if (count == 8) {
            count = 0;
            g << temp;
            temp = 0;
        }
        for (; bits_to_follow > 0; bits_to_follow--) {
            count++;
            if (count == 8) {
                count = 0;
                g << temp;
                temp = 0;
            }
        }
    }
}

int main(){
    int count = 0;//будем считать кол-во символов в тексте в общем, чтобы высчитывать вероятность символов
    ifstream f("input.txt", ios::out | ios::binary);
    ofstream g("output.txt", ios::out | ios::binary);
    map <char, int> m;//создаём мапу с символами и сооветствующими кол-вами повторений
    map <char, int> ::iterator ii;
    list<Range> L;
    while(!f.eof()){
        char c = f.get();
        m[c]++;
        count++;
    }
    for(ii = m.begin(); ii != m.end(); ii++){
        Range p;
        p.s = ii->first;
        p.numb = ii->second;
        L.push_back(p);
    }
    L.sort(Sort());
    L.begin()->rb = L.begin()->numb;
    L.begin()->lb = 0;
    list<Range>::iterator it = L.begin(), i2;
    i2 = it;
    it++;
    for(; it!=L.end(); it++){
        it->lb = i2->rb;
        it->rb = it->lb + it->numb;
        i2++;
    }
    int ik=0;
    for (ii = m.begin(); ii != m.end(); ii++){ //проходим по всей мапе и считаем сколько символов будет кодироваться(разных)
		if (ii->second != 0){
		    ik += 40;	  //добавляем к таблице 40 символов (сумма бит char и int)
		}
    }
	g.write((char*)(&ik), sizeof(ik));										//записываем информацию о разновидностях символов и их частот
	for (int k=0; k<256; k++){
		if (m[char(k)] > 0){
			char c=char(k);
			g.write((char*)(&c), sizeof(c));									//забивка символа
			g.write((char*)(&m[char(k)]), sizeof(m[char(k)]));					//забивка значения символа
		}
	}
    f.clear();
	f.seekg(0);

    //int l = 0, h = 65535, idx = 0, denom = L.back().rb;
    //int first_qtr = (h + 1) / 4, half = first_qtr * 2, third_qtr = first_qtr * 3, bits_to_follow = 0;   // в bits_to_follow сколько битов сбрасывать
    int idx = 0, denom = L.back().rb, l = 0;
    char temp = 0;
    count = 0;
    while(!f.eof()){
        char c = f.get(); idx++;
        for(it = L.begin(); it!=L.end(); it++){
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
                bits_plus_follow(0, count, g);
            }
            else if(l >= half){
                bits_plus_follow(1, count, g);
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
    g << temp;
    f.close();
    g.close();
    return 0;
}