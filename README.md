# structbuilder
C++ struct builder by toml(by Golang)

# package

used toml package:
https://github.com/pelletier/go-toml

# usage

```shell
$ structbuilder [-cpp output-c++source] [-hpp output-c++header] .toml-file
```

# toml format<serializer>

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
