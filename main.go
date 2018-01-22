package main

import (
	"flag"
	"fmt"
	"os"
	"strconv"
	"strings"

	"github.com/pelletier/go-toml"
)

const (
	intSize     = 4
	pointerSize = 8
)

var (
	sizeMap = map[string]int64{
		"int":                intSize,
		"unsigned int":       intSize,
		"size_t":             pointerSize,
		"char":               1,
		"unsigned char":      1,
		"short":              2,
		"unsigned short":     2,
		"long":               intSize,
		"unsigned long":      intSize,
		"long long":          8,
		"unsigned long long": 8,
		"float":              4,
		"double":             8,
		"int8_t":             1,
		"uint8_t":            1,
		"int16_t":            2,
		"uint16_t":           2,
		"int32_t":            4,
		"uint32_t":           4,
		"int64_t":            8,
		"uint64_t":           8,
	}
)

// MemberInfo struct member's information
type MemberInfo struct {
	Name    string
	Type    string
	Array   int64
	Default string
	Bits    int64
	Comment string
	Offset  int64
	Size    int64
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
	HasNameSpace bool
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
func getFloat(c *toml.Tree, n string, d float64) float64 {
	r := c.Get(n)
	if r == nil {
		return d
	}
	return r.(float64)
}

//
func checkFloatType(typeString string) bool {
	switch typeString {
	case "float", "double":
		return true
	}
	return false
}

//
func checkStringType(typeString string, arraySize int64) bool {
	switch typeString {
	case "std::string":
		return true
	case "char", "unsigned char":
		if arraySize > 1 {
			return true
		}
	}
	if strings.Index(typeString, "char*") >= 0 {
		return true
	}
	if strings.Index(typeString, "char *") >= 0 {
		return true
	}
	return false
}

// parse member
func memberParse(tomlConfig *toml.Tree) ([]MemberInfo, error) {
	var membersInfo []MemberInfo
	var offset int64
	offset = 0
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
				Array:   getInt(member, "array", 1),
				Comment: getString(member, "comment"),
			}
			baseSize, foundType := sizeMap[m.Type]
			if foundType == false {
				if strings.Index(m.Type, "*") >= 0 {
					baseSize = pointerSize
				} else {
					baseSize = intSize
				}
			}
			alignSize := baseSize - (offset % baseSize)
			if baseSize != alignSize {
				m.Offset = offset + alignSize
			} else {
				m.Offset = offset
			}
			m.Size = baseSize * m.Array
			offset = m.Offset + m.Size
			if checkStringType(m.Type, m.Array) {
				m.Default = getString(member, "default")
			} else if checkFloatType(m.Type) {
				m.Default = strconv.FormatFloat(getFloat(member, "default", 0), 'e', -1, 64)
			} else {
				m.Default = strconv.Itoa(int(getInt(member, "default", 0)))
			}
			membersInfo = append(membersInfo, m)
			fmt.Println("    ", mType, mName, "; //", m.Offset, ":", m.Size, "=", m.Default)
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
	if wInfo.NameSpace == "" {
		wInfo.HasNameSpace = false
	} else {
		wInfo.HasNameSpace = true
	}

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

	if wInfo.HasNameSpace {
		fmt.Println("namespace:", wInfo.NameSpace)
	}
	fmt.Printf("struct %s\n", wInfo.Struct.Name)
}
