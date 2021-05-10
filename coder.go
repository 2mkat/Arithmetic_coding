package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
)

// CalcFreq reads everything from ByteReader and returns byte frequencies.
func CountFreq(br io.ByteReader) (map[uint8]uint, uint) {
	freq := make(map[uint8]uint)
	var lenText uint

	v, err := br.ReadByte()
	for err == nil {
		freq[v]++
		lenText++
		v, err = br.ReadByte()
	}

	return freq, lenText
}

func CreateTable(map[uint8]uint, uint) map[uint8][]float64 {
	table := make(map[uint8][]float64)

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

	// Calculate byte frequencies
	log.Println("calculating frequencies")
	r := bufio.NewReader(inFile)
	freq, lenText := CountFreq(r)

	// Count AB for every symbol
	log.Println("calculating length")
	table := CreateTable(freq, lenText)

	fmt.Println(lenText)

}
