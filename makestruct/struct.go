package makestruct

import (
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
}

// Reserve is array size
type Reserve struct {
	Container string
	Name      string
	Size      int64
}

// StructInfo output struct information
type StructInfo struct {
	Name        string
	BitField    []BitField
	ChildStruct []StructInfo
	Members     []Member
	ReserveList []Reserve
}

// GlobalInfo is overall defined information
type GlobalInfo struct {
	NameSpace    string
	Include      []string
	LocalInclude []string
	TopStruct    StructInfo
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
func parseStruct(members []*toml.Tree) (StructInfo, error) {
	sInfo := StructInfo{
		BitField:    []BitField{},
		ChildStruct: []StructInfo{},
		Members:     []Member{},
		ReserveList: []Reserve{},
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
			isBool := typeStr == "bit-bool"
			bf := BitField{
				Name:     name.(string),
				CapName:  capitalize(name.(string), ""),
				Bits:     getInt(m, "bits", 1),
				IsBool:   isBool,
				IsSigned: !(isBool || typeStr == "bit-unsigned"),
				Offset:   getInt(m, "offset", 0),
				Scale:    getInt(m, "scale", 1),
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
				cS, err := parseStruct(ctype.([]*toml.Tree))
				if err != nil {
					return sInfo, err
				}
				cS.Name = typeStr
				sInfo.ChildStruct = append(sInfo.ChildStruct, cS)
				mm.Ref = "&"
			}
			sInfo.Members = append(sInfo.Members, mm)
		}
	}
	return sInfo, nil
}

// ParseToml setup serialize code information by toml
//
func ParseToml(tomlConfig *toml.Tree) (GlobalInfo, error) {
	gInfo := GlobalInfo{
		Include:      getStringList(tomlConfig, "include"),
		LocalInclude: getStringList(tomlConfig, "local_include"),
		NameSpace:    tomlConfig.Get("namespace").(string),
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

	topStruct, err := parseStruct(members.([]*toml.Tree))
	if err != nil {
		return gInfo, err
	}
	topStruct.Name = sn.(string)
	gInfo.TopStruct = topStruct
	return gInfo, nil
}
