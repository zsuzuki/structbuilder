package makestruct

import (
	"path/filepath"
	"strings"

	"github.com/pelletier/go-toml"
)

//
type myError struct {
	msg string
}

func (err *myError) Error() string {
	return err.msg
}

func capitalize(str string, prefix string) string {
	nl := strings.Split(str, "_")
	newStr := prefix
	for _, n := range nl {
		newStr += strings.Title(n)
	}
	return newStr
}

// BitField is defined bit field on struct
type BitField struct {
	Name     string
	CapName  string
	Bits     int64
	IsBool   bool
	IsSigned bool
	Offset   int64
	Scale    int64
	Cast     string
}

// Member is struct member
type Member struct {
	Name      string
	CapName   string
	Type      string
	Container string
	Ref       string
	IsStatic  bool
	Size      int64
	HasChild  bool
	Child     *StructInfo
}

// Reserve is array size
type Reserve struct {
	Container string
	Name      string
	Size      int64
}

// EnumInfo is defined enumerate list
type EnumInfo struct {
	StructName string
	Name       string
	List       []string
}

// StructInfo output struct information
type StructInfo struct {
	Name        string
	IsClass     bool
	BitField    []BitField
	ChildStruct []StructInfo
	Members     []Member
	ReserveList []Reserve
	EnumList    []EnumInfo
	Serializer  string
	SJson       string
	UseLua      bool
}

// GlobalInfo is overall defined information
type GlobalInfo struct {
	NameSpace     string
	Include       []string
	LocalInclude  []string
	HeaderNameB   string
	HeaderGlobalB bool
	HeaderNameJ   string
	HeaderGlobalJ bool
	UseLua        bool
	TopStruct     StructInfo
}

// build string list from toml attribute
func getStringList(tomlConfig *toml.Tree, attr string) []string {
	al := tomlConfig.Get(attr)
	result := []string{}
	if al != nil {
		attrList := al.([]interface{})
		for _, i := range attrList {
			result = append(result, i.(string))
		}
	}
	return result
}

//
func getInt(tomlConfig *toml.Tree, attr string, number int64) int64 {
	n := tomlConfig.Get(attr)
	if n != nil {
		number = n.(int64)
	}
	return number
}

// parse struct
//
func parseStruct(members []*toml.Tree, sname string) (StructInfo, error) {
	sInfo := StructInfo{
		Name:        sname,
		BitField:    []BitField{},
		ChildStruct: []StructInfo{},
		Members:     []Member{},
		ReserveList: []Reserve{},
		IsClass:     false,
		UseLua:      false,
	}
	for _, m := range members {
		name := m.Get("name")
		if name == nil {
			return sInfo, &myError{msg: "member name is not defined"}
		}
		mtype := m.Get("type")
		if mtype == nil {
			return sInfo, &myError{msg: "member type is not defined"}
		}
		typeStr := mtype.(string)
		if strings.HasPrefix(typeStr, "bit-") {
			// bit field
			castType := ""
			isBool := typeStr == "bit-bool"
			if typeStr == "bit-enum" {
				typeStr = "bit-unsigned"
				cast := m.Get("cast")
				if cast != nil {
					castType = cast.(string)
					enumInfo := EnumInfo{
						StructName: sname,
						Name:       castType,
						List:       getStringList(m, "enum"),
					}
					sInfo.EnumList = append(sInfo.EnumList, enumInfo)
				}
			}
			bf := BitField{
				Name:     name.(string),
				CapName:  capitalize(name.(string), ""),
				Bits:     getInt(m, "bits", 1),
				IsBool:   isBool,
				IsSigned: !(isBool || typeStr == "bit-unsigned"),
				Offset:   getInt(m, "offset", 0),
				Scale:    getInt(m, "scale", 1),
				Cast:     castType,
			}
			sInfo.BitField = append(sInfo.BitField, bf)
		} else {
			mm := Member{
				Name:      name.(string),
				CapName:   capitalize(name.(string), ""),
				Type:      typeStr,
				Container: "",
				Ref:       "",
				IsStatic:  false,
				Size:      1,
				HasChild:  false,
				Child:     nil,
			}
			container := m.Get("container")
			if container != nil {
				isStatic := func() bool {
					switch container.(string) {
					case "std::vector":
						return false
					default:
					}
					return true
				}()
				mm.IsStatic = isStatic
				mm.Container = container.(string)
				rs := m.Get("reserve")
				if rs != nil {
					if isStatic {
						mm.Size = rs.(int64)
					} else {
						// for vector
						res := Reserve{
							Container: mm.Container,
							Name:      name.(string),
							Size:      rs.(int64),
						}
						sInfo.ReserveList = append(sInfo.ReserveList, res)
					}
				} else if isStatic {
					return sInfo, &myError{msg: "not defined size: " + name.(string)}
				}
			}
			ctype := m.Get(typeStr)
			if ctype != nil {
				// child is new struct
				cS, err := parseStruct(ctype.([]*toml.Tree), typeStr)
				if err != nil {
					return sInfo, err
				}
				sInfo.ChildStruct = append(sInfo.ChildStruct, cS)
				mm.Ref = "&"
				mm.HasChild = true
				mm.Child = &cS
			}
			sInfo.Members = append(sInfo.Members, mm)
		}
	}
	return sInfo, nil
}

//
func checkRelPath(basePath string, targetPath string) (string, bool) {
	if filepath.HasPrefix(basePath, targetPath) {
		p := basePath[len(targetPath):]
		if strings.HasPrefix(p, "/") {
			p = p[1:]
		}
		return p, true
	}
	return "", false
}

// ParseToml setup serialize code information by toml
//
func ParseToml(tomlConfig *toml.Tree, hpp string, bser string, json string) (GlobalInfo, error) {
	gInfo := GlobalInfo{
		Include:       getStringList(tomlConfig, "include"),
		LocalInclude:  getStringList(tomlConfig, "local_include"),
		NameSpace:     tomlConfig.Get("namespace").(string),
		HeaderNameB:   "",
		HeaderNameJ:   "",
		HeaderGlobalB: false,
		HeaderGlobalJ: false,
		UseLua:        false,
	}
	fullHpp, _ := filepath.Abs(hpp)
	hppPath := filepath.Dir(fullHpp)
	if bser != "" {
		fullBinSer, _ := filepath.Abs(bser)
		bserPath := filepath.Dir(fullBinSer)
		bP, ex := checkRelPath(hppPath, bserPath)
		if ex {
			gInfo.HeaderNameB = filepath.Join(bP, filepath.Base(hpp))
		} else {
			gInfo.HeaderNameB = filepath.Base(hpp)
		}
		gInfo.HeaderGlobalB = !ex
	}
	if json != "" {
		fullPathJs, _ := filepath.Abs(json)
		jsonPath := filepath.Dir(fullPathJs)
		jP, ex := checkRelPath(hppPath, jsonPath)
		if ex {
			gInfo.HeaderNameJ = filepath.Join(jP, filepath.Base(hpp))
		} else {
			gInfo.HeaderNameJ = filepath.Base(hpp)
		}
		gInfo.HeaderGlobalJ = !ex
	}

	// top level struct
	sn := tomlConfig.Get("struct_name")
	if sn == nil {
		return gInfo, &myError{msg: "not defined struct name"}
	}
	members := tomlConfig.Get("member")
	if members == nil {
		return gInfo, &myError{msg: "not have members"}
	}

	topStruct, err := parseStruct(members.([]*toml.Tree), sn.(string))
	if err != nil {
		return gInfo, err
	}
	ser := tomlConfig.Get("serializer")
	if ser != nil {
		topStruct.Serializer = ser.(string)
	}
	serJ := tomlConfig.Get("serializer_json")
	if serJ != nil {
		topStruct.SJson = serJ.(string)
	}
	lua := tomlConfig.Get("lua")
	if lua != nil {
		gInfo.UseLua = lua.(bool)
	}
	topStruct.UseLua = gInfo.UseLua
	topStruct.IsClass = true
	gInfo.TopStruct = topStruct
	return gInfo, nil
}
