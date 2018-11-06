package main

import (
	"flag"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"text/template"

	"./makestruct"
	"./serialize"

	"github.com/pelletier/go-toml"
)

var (
	enableFormat bool
	wd           string
	myName       = ""
	objName      = []string{}
	tmplPushStr  = []string{}
	tmplFuncs    = template.FuncMap{
		"pushStr": func(str ...string) string {
			ns := ""
			for _, s := range str {
				ns += s
			}
			tmplPushStr = append(tmplPushStr, ns)
			return ""
		},
		"popStr": func() string {
			tmplPushStr = tmplPushStr[:len(tmplPushStr)-1]
			return ""
		},
		"getStr": func() string { return strings.Join(tmplPushStr, "") },
		"setMyName": func(n string) string {
			myName = n
			return ""
		},
		"clearMyName": func() string {
			myName = ""
			return ""
		},
		"myName": func() string {
			if myName == "" {
				return ""
			}
			return myName + "."
		},
		"pushObj": func(o string) string {
			objName = append(objName, o)
			return ""
		},
		"popObj": func() string {
			objName = objName[:len(objName)-1]
			return ""
		},
		"getObj": func() string {
			return objName[len(objName)-1]
		},
	}
)

// output file by template(default is Stdout)
func outputTemplateFile(data interface{}, name string, tempname []string) error {
	fl := []string{}
	for _, tn := range tempname {
		fl = append(fl, filepath.Join(wd, "templates", tn))
	}
	tmpl, err := template.New(filepath.Base(fl[0])).Funcs(tmplFuncs).ParseFiles(fl...)
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
	sjsonFile := flag.String("json", "", "output c++ source filename(json serializer)")
	serFile := flag.String("serialize", "", "output c++ source filename(serializer)")
	luaFile := flag.String("lua", "", "output lua interface c++ source filename")
	flag.BoolVar(&enableFormat, "format", false, "use clang-format")
	indentStep := flag.Int("indent", 4, "indent step")
	flag.Parse()
	fmt.Printf("output: %s %s %s %s %s\n", *hppFile, *cppFile, *sjsonFile, *serFile, *luaFile)

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
	wd = filepath.Dir(os.Args[0])

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
		gInfo, err := makestruct.ParseToml(tomlConfig, *hppFile, *serFile, *sjsonFile)
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
		if gInfo.TopStruct.SJson != "" {
			err = outputTemplateFile(gInfo, *sjsonFile, []string{"struct_json.tpl", "struct_json_child_out.tpl", "struct_json_child_in.tpl"})
			if err != nil {
				fmt.Println(err.Error())
				os.Exit(1)
			}
		}
		if gInfo.TopStruct.Serializer != "" {
			err = outputTemplateFile(gInfo, *serFile, []string{"struct_ser.tpl", "struct_ser_child_out.tpl", "struct_ser_child_in.tpl", "struct_ser_child_size.tpl"})
			if err != nil {
				fmt.Println(err.Error())
				os.Exit(1)
			}
		}
		if gInfo.UseLua {
			err = outputTemplateFile(gInfo, *luaFile, []string{"luasol.tpl", "luasol_child.tpl"})
			if err != nil {
				fmt.Println(err.Error())
				os.Exit(1)
			}
		}
	}
}
