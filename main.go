package main

import (
	"flag"
	"fmt"
	"os"
	"strings"

	"github.com/pelletier/go-toml"
)

// MemberInfo struct member's information
type MemberInfo struct {
	Name       string
	Type       string
	SizeMethod bool
	SizeType   string
	Container  bool
	RawAccess  bool
	Children   []MemberInfo
}

// StructInfo single struct information
type StructInfo struct {
	Name    string
	Version int64
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

func (m *MemberInfo) getName() string {
	if m.RawAccess {
		return m.Name
	}
	return "get" + strings.Title(m.Name) + "()"
}

// put members
func memberDump(structInfo StructInfo) error {
	members := structInfo.Member
	fmt.Printf("//\nsize_t get%sPackSize(const %s& s) {\n", structInfo.Name, structInfo.Name)
	fmt.Printf("  return 0;\n}\n")
	fmt.Printf("//\nvoid pack%s(Serializer& ser, %s& s) {\n", structInfo.Name, structInfo.Name)
	fmt.Printf("  ser.put<uint_t>(get%sPackSize());\n", structInfo.Name)
	fmt.Printf("  ser.put<uint16_t>(%d);\n", structInfo.Version)
	fmt.Printf("  for (auto &t : s.tlist) {\n")
	for _, m := range members {
		if m.Type == "struct" {
			fmt.Printf("    ser.putStruct(t.%s);\n", m.getName())
		} else if m.Container {
			fmt.Printf("    ser.putVector<%s>(t.%s);\n", m.Type, m.getName())
		} else {
			fmt.Printf("    ser.putBuffer<%s>(t.%s);\n", m.Type, m.getName())
		}
	}
	fmt.Printf("  }\n}\n//\n")
	fmt.Printf("void unpack%s(Serializer& ser, %s& s) {\n", structInfo.Name, structInfo.Name)
	fmt.Printf("  auto pack_size = ser.get<uint32_t>();\n")
	fmt.Printf("  auto version   = ser.get<uint16_t>();\n")
	fmt.Printf("  for (auto &t : s.tlist) {\n")
	for _, m := range members {
		if m.Type == "struct" {
			fmt.Printf("    ser.getStruct(t.%s);\n", m.getName())
		} else if m.Container {
			fmt.Printf("    ser.getVector<%s>(t.%s);\n", m.Type, m.getName())
		} else {
			fmt.Printf("    ser.getBuffer<%s>(t.%s);\n", m.Type, m.getName())
		}
	}
	fmt.Printf("  }\n}\n")
	return nil
}

// parse member
func memberParse(membersConfig []*toml.Tree) ([]MemberInfo, error) {
	var membersInfo []MemberInfo
	for _, m := range membersConfig {
		name := m.Get("name")
		if name == nil {
			return membersInfo, &myError{"not defined member name"}
		}
		typeName := m.Get("type")
		if typeName == nil {
			return membersInfo, &myError{"not defined member type"}
		}
		mInfo := MemberInfo{
			Name:     name.(string),
			Type:     typeName.(string),
			Children: nil,
		}
		sizeType := m.Get("size_type")
		if sizeType == nil {
			mInfo.SizeType = ""
		} else {
			mInfo.SizeType = sizeType.(string)
		}
		sizeMethod := m.Get("size_method")
		if sizeMethod == nil {
			mInfo.SizeMethod = false
		} else {
			mInfo.SizeMethod = sizeMethod.(bool)
		}
		container := m.Get("container")
		if container == nil {
			mInfo.Container = false
		} else {
			mInfo.Container = container.(bool)
		}
		rawAccess := m.Get("raw_access")
		if rawAccess == nil {
			mInfo.RawAccess = false
		} else {
			mInfo.RawAccess = rawAccess.(bool)
		}
		membersInfo = append(membersInfo, mInfo)
	}
	return membersInfo, nil
}

// setup world
func parseToml(tomlConfig *toml.Tree) (GlobalInfo, error) {
	var wInfo GlobalInfo
	wInfo.Include = []string{}
	wInfo.LocalInclude = []string{}
	wInfo.NameSpace = tomlConfig.Get("namespace").(string)
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
	sn := tomlConfig.Get("struct_name")
	if sn == nil {
		return wInfo, &myError{msg: "not defined struct name"}
	}
	membersConfig := tomlConfig.Get("member")
	if membersConfig == nil {
		return wInfo, &myError{msg: "not have members"}
	}

	var sInfo StructInfo
	sInfo.Name = sn.(string)
	ver := tomlConfig.Get("version")
	if ver != nil {
		sInfo.Version = ver.(int64)
	} else {
		sInfo.Version = 100
	}

	var err error
	sInfo.Member, err = memberParse(membersConfig.([]*toml.Tree))
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

	fmt.Printf("#pragma once\n\n")
	if len(wInfo.LocalInclude) > 0 {
		for _, inc := range wInfo.LocalInclude {
			fmt.Printf("#include \"%s\"\n", inc)
		}
		fmt.Printf("\n")
	}
	for _, inc := range wInfo.Include {
		fmt.Printf("#include <%s>\n", inc)
	}
	fmt.Printf("\n")

	if wInfo.HasNameSpace {
		fmt.Printf("namespace %s {\n", wInfo.NameSpace)
	}
	memberDump(wInfo.Struct)
	fmt.Printf("} // namespace %s\n", wInfo.NameSpace)
}
