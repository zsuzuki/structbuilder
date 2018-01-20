package main

import (
	"flag"
	"fmt"
	"os"

	"github.com/pelletier/go-toml"
)

// MemberInfo struct member's information
type MemberInfo struct {
	Name          string
	Type          string
	Array         int64
	Default       interface{}
	DefaultString string
	Bits          int64
	Comment       string
}

// StructInfo single struct information
type StructInfo struct {
	Name    string
	MaxSize int64
	Member  []MemberInfo
}

// GlobalInfo global context for header
// (namespace,include...)
type GlobalInfo struct {
	NameSpace    string
	Include      []string
	LocalInclude []string
	Struct       StructInfo
}

//
type myError struct {
	msg string
}

func (err *myError) Error() string {
	return err.msg
}

//
func getString(c *toml.Tree, n string) string {
	r := c.Get(n)
	if r == nil {
		return ""
	}
	return r.(string)
}
func getInt(c *toml.Tree, n string, d int64) int64 {
	r := c.Get(n)
	if r == nil {
		return d
	}
	return r.(int64)

}

// parse member
func memberParse(tomlConfig *toml.Tree) ([]MemberInfo, error) {
	var membersInfo []MemberInfo
	membersConfig := tomlConfig.Get("member")
	if membersConfig != nil {
		membersConfigList := membersConfig.([]*toml.Tree)
		for _, member := range membersConfigList {
			mName := member.Get("name")
			if mName == nil {
				return membersInfo, &myError{"not defined member name"}
			}
			mType := member.Get("type")
			if mType == nil {
				return membersInfo, &myError{"not defined member class-type"}
			}
			m := MemberInfo{
				Name:    mName.(string),
				Type:    mType.(string),
				Bits:    getInt(member, "bits", 0),
				Comment: getString(member, "comment"),
			}
			membersInfo = append(membersInfo, m)
			fmt.Println("    ", mType, mName, ";")
		}
	}
	return membersInfo, nil
}

// setup world
func parseToml(tomlConfig *toml.Tree) (GlobalInfo, error) {
	var wInfo GlobalInfo
	wInfo.Include = []string{}
	wInfo.LocalInclude = []string{}
	wInfo.NameSpace = getString(tomlConfig, "namespace")
	il := tomlConfig.Get("include")
	if il != nil {
		includeList := il.([]interface{})
		for _, i := range includeList {
			wInfo.Include = append(wInfo.Include, i.(string))
		}
	}
	lil := tomlConfig.Get("local_include")
	if lil != nil {
		localIncludeList := lil.([]interface{})
		for _, i := range localIncludeList {
			wInfo.LocalInclude = append(wInfo.LocalInclude, i.(string))
		}
	}

	// setup struct
	var sInfo StructInfo
	structConfig := tomlConfig.Get("struct").(*toml.Tree)
	sn := structConfig.Get("name")
	if sn == nil {
		return wInfo, &myError{msg: "not defined struct name"}
	}
	sInfo.Name = sn.(string)
	ms := structConfig.Get("maxsize")
	if ms != nil {
		sInfo.MaxSize = ms.(int64)
	} else {
		sInfo.MaxSize = 0
	}
	var err error
	sInfo.Member, err = memberParse(structConfig)
	wInfo.Struct = sInfo

	return wInfo, err
}

//
// application
//
func main() {
	headerFile := flag.String("header", "", "output header filename")
	cppFile := flag.String("cpp", "", "output c++ source filename")
	flag.Parse()
	fmt.Printf("output: %s %s\n", *headerFile, *cppFile)

	// file input
	if len(os.Args) < 2 {
		fmt.Println("no input toml file.")
		os.Exit(1)
	}
	intpuFile := os.Args[1]
	tomlConfig, ok := toml.LoadFile(intpuFile)
	if ok != nil {
		fmt.Println("TOML read error:", intpuFile)
		os.Exit(1)
	}

	wInfo, ok := parseToml(tomlConfig)
	if ok != nil {
		fmt.Println(ok.Error())
		os.Exit(1)
	}

	fmt.Println("namespace:", wInfo.NameSpace)
	fmt.Printf("struct %s {}\n", wInfo.Struct.Name)
}
