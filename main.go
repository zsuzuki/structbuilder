package main

import (
	"flag"
	"fmt"
	"os"
	"text/template"

	"./serialize"

	"github.com/pelletier/go-toml"
)

//
// application
//
func main() {
	ser := flag.Bool("s", true, "output serializer")
	cppFile := flag.String("cpp", "", "output c++ source filename")
	hppFile := flag.String("hpp", "", "output c++ header filename")
	indentStep := flag.Int("indent", 4, "indent step")
	flag.Parse()
	fmt.Printf("output: %s %s\n", *hppFile, *cppFile)

	// file input
	fArgs := flag.Args()
	if len(fArgs) < 1 {
		fmt.Fprintln(os.Stderr, "no input toml file.")
		os.Exit(1)
	}
	intpuFile := fArgs[0]
	tomlConfig, ok := toml.LoadFile(intpuFile)
	if ok != nil {
		fmt.Fprintln(os.Stderr, "TOML read error:", intpuFile)
		os.Exit(1)
	}

	if *ser {
		//
		serialize.SetIndent(*indentStep)
		wInfo, ok := serialize.ParseToml(tomlConfig)
		if ok != nil {
			fmt.Fprintln(os.Stderr, ok.Error())
			os.Exit(1)
		}
		// output - c++ header
		hpptmpl, err := template.ParseFiles("output_hpp.tpl")
		if err != nil {
			fmt.Fprintln(os.Stderr, err.Error())
		}
		var hFile *os.File
		if len(*hppFile) > 0 {
			hFile, err = os.Create(*hppFile)
			if err != nil {
				fmt.Fprintln(os.Stderr, err.Error())
			}
		} else {
			hFile = os.Stdout
		}
		hpptmpl.Execute(hFile, wInfo)

		// output - c++ source
		cpptmpl, err := template.ParseFiles("output_cpp.tpl")
		if err != nil {
			fmt.Fprintln(os.Stderr, err.Error())
		}
		var oFile *os.File
		if len(*cppFile) > 0 {
			oFile, err = os.Create(*cppFile)
			if err != nil {
				fmt.Fprintln(os.Stderr, err.Error())
			}
		} else {
			oFile = os.Stdout
		}
		cpptmpl.Execute(oFile, wInfo)
	}
}
