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

type kv struct {
	Key   byte
	Value uint
}
func Sort(freq *map[byte]uint) []kv{

	var ss []kv
	for k, v := range *freq {
		ss = append(ss, kv{k, v})
	}

	sort.Slice(ss, func(i, j int) bool {
		return ss[i].Value > ss[j].Value
	})

	return ss
}

type Table struct {
	s byte
	amount uint
	a float64
	b float64
}

func CreateTable(freq []kv, lenText uint64) []Table{
	var table []Table
	var i float64
	for idx := range freq {
		table = append(table, Table{freq[idx].Key, freq[idx].Value, i, i + float64(freq[idx].Value)/float64(lenText)})
		i += float64(freq[idx].Value)/float64(lenText)
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

	//sort freq in less
	log.Println("sort freq")
	var freq_ []kv
	freq_ = Sort(&freq)

	// Count AB for every symbol
	log.Println("Create table")
	table := CreateTable(freq_, lenText)

	log.Println("print table")
	for key := range table {
		fmt.Println(string(table[key].s))
		fmt.Println(table[key].amount)
		fmt.Println(table[key].a)
		fmt.Println(table[key].b)
	}
}
