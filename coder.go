package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
	"sort"
)

// CalcFreq reads everything from ByteReader and returns byte frequencies.
func CountFreq(br io.ByteReader) (map[byte]uint, uint64) {
	freq := make(map[byte]uint)
	var lenText uint64

	v, err := br.ReadByte()
	for err == nil {
		freq[v]++
		lenText++
		v, err = br.ReadByte()
	}

	return freq, lenText
}

func Sort(freq *map[byte]uint){
	type kv struct {
		Key   byte
		Value uint
	}

	var ss []kv
	for k, v := range *freq {
		ss = append(ss, kv{k, v})
	}

	sort.Slice(ss, func(i, j int) bool {
		return ss[i].Value > ss[j].Value
	})
	freq_res := make(map[byte]uint)
	for i := range ss{
		freq_res[ss[i].Key] = ss[i].Value
	}
	*freq = freq_res
}

type Table struct {
	s byte
	amount uint
	a float64
	b float64
}

func CreateTable(freq map[uint8]uint, lenText uint64) map[byte]*Table{
	var table = make(map[byte]*Table)
	var i float64
	for key, value := range freq {
		t := &Table{
			s:  key,
			amount: value,
			a: float64(i),
			b: i + float64(value)/float64(lenText),
		}
		i := i + float64(value)/float64(lenText)
		fmt.Println(i)
		table[key] = t
	}

	return table
}

func main() {
	//Open input file for reading
	inFile, err := os.Open("input.txt")
	if err != nil {
		fmt.Fprintf(os.Stderr, "can't open file")
		return
	}
	defer inFile.Close()

	//Open output file for writing data
	outFile, err := os.Create("output")
	if err != nil {
		fmt.Fprintf(os.Stderr, "can't create file ")
		return
	}
	defer outFile.Close()

	// Calculate bytes and frequencies
	log.Println("calculating length file and frequencies ")
	r := bufio.NewReader(inFile)
	freq, lenText := CountFreq(r)
	fmt.Println(lenText)

	//sort freq in less
	log.Println("sort freq")
	Sort(&freq)
	for key, value := range freq{
		fmt.Println(string(key))
		fmt.Println(value)
	}

	// Count AB for every symbol
	log.Println("Create table")
	table := CreateTable(freq, lenText)

	log.Println("print table")
	for key := range table {
		fmt.Println(string(key))
		//fmt.Println(table[key].amount)
		//fmt.Println(table[key].a)
		//fmt.Println(table[key].b)
	}
}
