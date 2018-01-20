# structbuilder
C++ struct builder by toml(by Golang)

# package

https://github.com/pelletier/go-toml

# usage

```shell
$ structbuilder app.toml
```

# toml format

```toml
namspace = "myapp"
include = ["cstdint","myapp.h"]
local_include = ["local.h"]

[struct]
name="parameter"
maxsize=32

[[struct.member]]
name = "state"
type = "uint8_t"
comment = "current working status"
default = 0
[[struct.member]]
name = "progress"
type = "float"
default = 0.0
[[struct.member]]
name = "country"
type = "char"
array = 32
default = "japan"
comment = "work in country"
```
