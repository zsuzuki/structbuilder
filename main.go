package main

import (
	"flag"
	"fmt"
	"os"
	"strings"
	"text/template"

	"github.com/pelletier/go-toml"
)

var (
	indentStep int
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
	// for output
	DispName     string
	Brank        string
	SetterName   string
	GetterName   string
	SizeName     string
	BracketClose bool
}

// StructInfo single struct information
type StructInfo struct {
	Name        string
	Version     int64
	Brank       string
	Member      []MemberInfo
	DispMembers []MemberInfo
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

func (m *MemberInfo) getName(parentName string, isRead bool) string {
	prefix := parentName
	if prefix != "" {
		prefix += "."
	}
	if m.RawAccess {
		return prefix + m.Name
	}
	opName := func() string {
		if isRead {
			return "get"
		}
		return "put"
	}()
	return prefix + opName + strings.Title(m.Name) + "()"
}

func (m *MemberInfo) getSizeName(parentName string, isRead bool) string {
	prefix := parentName
	if prefix != "" {
		prefix += "."
	}
	if m.SizeMethod == false {
		return "sizeof(" + prefix + m.Name + ")"
	}
	opName := func() string {
		if isRead {
			return "get"
		}
		return "put"
	}()
	return prefix + opName + strings.Title(m.Name) + "Size()"
}

func dumpPutMember(m MemberInfo, parentName string) []MemberInfo {
	dispMembers := []MemberInfo{}
	myName := m.getName(parentName, true)
	mInfo := m
	mInfo.DispName = myName
	mInfo.SizeName = m.getSizeName(parentName, true)
	dispMembers = append(dispMembers, mInfo)

	if m.Children != nil {
		for _, ch := range m.Children {
			ml := dumpPutMember(ch, m.Type)
			dispMembers = append(dispMembers, ml...)
		}
		var bClose MemberInfo
		bClose.BracketClose = true
		bClose.Brank = m.Brank
		dispMembers = append(dispMembers, bClose)
	}

	return dispMembers
}

// put members
func memberDump(structInfo StructInfo) ([]MemberInfo, error) {
	members := structInfo.Member
	dispMembers := []MemberInfo{}
	for _, m := range members {
		rm := dumpPutMember(m, "s")
		dispMembers = append(dispMembers, rm...)
	}
	return dispMembers, nil
}

// parse member
func memberParse(membersConfig []*toml.Tree, brank int) ([]MemberInfo, error) {
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
			Brank:    strings.Repeat(" ", brank),
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

		children := m.Get("member")
		if children != nil {
			var err error
			mInfo.Children, err = memberParse(children.([]*toml.Tree), brank+indentStep)
			if err != nil {
				return membersInfo, err
			}
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
	sInfo.Member, err = memberParse(membersConfig.([]*toml.Tree), indentStep)
	if err == nil {
		sInfo.DispMembers, err = memberDump(sInfo)
	}
	wInfo.Struct = sInfo

	return wInfo, err
}

//
// application
//
func main() {
	headerFile := flag.String("header", "", "output header filename")
	cppFile := flag.String("cpp", "", "output c++ source filename")
	flag.IntVar(&indentStep, "indent", 4, "indent step")
	flag.Parse()
	fmt.Printf("output: %s %s\n", *headerFile, *cppFile)

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

	wInfo, ok := parseToml(tomlConfig)
	if ok != nil {
		fmt.Fprintln(os.Stderr, ok.Error())
		os.Exit(1)
	}
	tmpl, err := template.ParseFiles("output.tpl")
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
	tmpl.Execute(oFile, wInfo)
}
