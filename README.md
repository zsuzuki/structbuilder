# structbuilder
C++ struct builder by toml(by Golang)

# package

used toml package:[go-toml](https://github.com/pelletier/go-toml)

used json serializer:[json](https://github.com/nlohmann/json)

target lua utility:[sol2](https://github.com/ThePhD/sol2)

# usage

```shell
$ structbuilder [-format] [-s] [-cpp output-c++source] [-hpp output-c++header] [-json output-json-serializer-c++-source] [-lua output-lua-interface] .toml-file
```
- "-format" use clang-format for output-files
- "-s" select serializer format
- "-cpp" name of output c++ source file(for serializer format only)
- "-hpp" name of header c++
- "-json" output json serializer source
- "-lua" output lua interface source

# toml format

## struct format

struct [struct.hpp](cpp/struct.hpp)
```toml
namespace = "Sample"
local_include = ["serializer.hpp"]
include = ["cstdint","vector","string","array","nlohmann/json.hpp"]
struct_name = "Test"
comment = """
Test class
"""

serializer_json = "nlohmann::json"
serializer = "Serializer"
lua = true

[[member]]
name = "index"
type = "bit-unsigned"
bits = 5
[[member]]
name = "beer_type"
type = "bit-enum"
cast = "BeerType"
bits = 5
enum = ["Ales","Larger","Pilsner","Lambic","IPA"]
[[member]]
name = "generation"
type = "bit-signed"
bits = 3
[[member]]
name = "enabled"
type = "bit-bool"
[[member]]
name = "count"
type = "int"
[[member]]
name = "max_speed"
type = "uint32_t"
[[member]]
name = "note"
type = "Note"
container = "std::array"
reserve = 4
    [[member.Note]]
    name = "page"
    type = "int"
    [[member.Note]]
    name = "line"
    type = "int"
```

## serializer format

serialize [test.hpp](cpp/test.hpp) 

```toml
namespace = "Sample"
local_include = ["test.hpp", "serializer.hpp"]
# include = ["serializer.hpp"]
version = 1001 # m.n.oo
unsupport = 999 # 0.9.99

struct_name = "Test"

[[member]]
name = "child_list"
var_name = "t"
size_type = "uint8_t"
raw_access = true

[[member.child]]
name = "field"
type = "struct"
raw_access = true

[[member.child]]
name = "message"
type = "char"
size_type = "uint8_t"
#raw_access = true

[[member.child]]
name = "ranking"
type = "uint16_t"
size_type = "uint8_t"
container = true
raw_access = true
```
