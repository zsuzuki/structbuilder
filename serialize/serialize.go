package serialize

import (
	"strings"

	"github.com/pelletier/go-toml"
)

var (
	indentStep int
)

// MemberInfo struct member's information
type MemberInfo struct {
	Name      string
	Type      string
	VarName   string
	SizeType  string
	Container bool
	RawAccess bool
	Children  []MemberInfo
	// for output
	DispName     string
	DispNameSet  string
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
	Unsupport   int64
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

// SetIndent set indent step in source code
func SetIndent(step int) {
	indentStep = step
}

func capitalize(str string, prefix string) string {
	nl := strings.Split(str, "_")
	newStr := prefix
	for _, n := range nl {
		newStr += strings.Title(n)
	}
	return newStr
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
		return "set"
	}()

	return capitalize(m.Name, prefix+opName) + func() string {
		if isRead {
			return "()"

		}
		return ""
	}()
}

func (m *MemberInfo) getSizeName(parentName string, isRead bool) string {
	prefix := parentName
	if prefix != "" {
		prefix += "."
	}
	if m.RawAccess {
		return "sizeof(" + prefix + m.Name + ")"
	}
	opName := func() string {
		if isRead {
			return "get"
		}
		return "set"
	}()
	return capitalize(m.Name, prefix+opName) + "Size()"
}

func dumpPutMember(m MemberInfo, parentName string) []MemberInfo {
	dispMembers := []MemberInfo{}
	myName := m.getName(parentName, true)
	mInfo := m
	mInfo.DispName = myName
	mInfo.DispNameSet = m.getName(parentName, false)
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
			typeName = m.Get("var_name")
			if typeName == nil {
				return membersInfo, &myError{"not defined member type"}
			}
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

		children := m.Get("child")
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

// ParseToml setup serialize code information by toml
func ParseToml(tomlConfig *toml.Tree) (GlobalInfo, error) {
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

	sInfo := StructInfo{
		Name:  sn.(string),
		Brank: strings.Repeat(" ", indentStep),
	}
	ver := tomlConfig.Get("version")
	if ver != nil {
		sInfo.Version = ver.(int64)
	} else {
		sInfo.Version = 100
	}
	unsup := tomlConfig.Get("unsupport")
	if unsup != nil {
		sInfo.Unsupport = unsup.(int64)
	} else {
		sInfo.Unsupport = 0
	}

	var err error
	sInfo.Member, err = memberParse(membersConfig.([]*toml.Tree), indentStep)
	if err == nil {
		sInfo.DispMembers, err = memberDump(sInfo)
	}
	wInfo.Struct = sInfo

	return wInfo, err
}
