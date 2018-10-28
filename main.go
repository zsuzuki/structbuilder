package main

import (
	"flag"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"text/template"

	"./makestruct"
	"./serialize"

	"github.com/pelletier/go-toml"
)

var (
	enableFormat bool
)

// output file by template(default is Stdout)
func outputTemplateFile(data interface{}, name string, tempname []string) error {
	fl := []string{}
	for _, tn := range tempname {
		fl = append(fl, filepath.Join("templates", tn))
	}
	tmpl, err := template.ParseFiles(fl...)
	if err != nil {
		return err
	}
	var oFile *os.File
	format := false
	if len(name) > 0 {
		oFile, err = os.Create(name)
		if err != nil {
			return err
		}
		format = enableFormat
	} else {
		oFile = os.Stdout
	}
	tmpl.Execute(oFile, data)
	if format {
		exec.Command("clang-format", "-i", name).Run()
	}
	return nil
}

//
// application
//
func main() {
	ser := flag.Bool("s", false, "output serializer")
	cppFile := flag.String("cpp", "", "output c++ source filename")
	hppFile := flag.String("hpp", "", "output c++ header filename")
	flag.BoolVar(&enableFormat, "format", false, "use clang-format")
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
		// serializer
		serialize.SetIndent(*indentStep)
		wInfo, ok := serialize.ParseToml(tomlConfig)
		if ok != nil {
			fmt.Fprintln(os.Stderr, ok.Error())
			os.Exit(1)
		}
		// output - c++ serialize file
		err := outputTemplateFile(wInfo, *hppFile, []string{"serialize_hpp.tpl"})
		if err != nil {
			fmt.Println(err.Error())
			os.Exit(1)
		}
		err = outputTemplateFile(wInfo, *cppFile, []string{"serialize_cpp.tpl"})
		if err != nil {
			fmt.Println(err.Error())
			os.Exit(1)
		}
	} else {
		gInfo, err := makestruct.ParseToml(tomlConfig)
		if err != nil {
			fmt.Println(err.Error())
			os.Exit(1)
		}
		// output - c++ struct file
		err = outputTemplateFile(gInfo, *hppFile, []string{"struct_hpp.tpl", "struct_child.tpl"})
		if err != nil {
			fmt.Println(err.Error())
			os.Exit(1)
		}
	}
}
