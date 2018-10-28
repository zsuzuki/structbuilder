{{- define "struct_base"}}
  //
  struct {{.Name}} {
{{- with .ChildStruct}}
    // child class
{{- range .}}
{{template "struct_base" .}}
{{- end}}
{{end}}
{{- with .BitField}}
    struct BitField {
{{- range .}}
      {{if .IsBool}}unsigned {{.Name}} : 1;
{{- else}}{{if .IsSigned}}signed  {{else}}unsigned{{end}} {{.Name}} : {{.Bits}};{{end}}
{{- end}}
    };
    BitField bit_field;
{{end}}
    // members
{{- range .Members}}
{{- if .Container}}
    {{.Container}} {{.Name}};
{{- else}}
    {{.Type}} {{.Name}};
{{- end}}
{{- end}}
{{- with .ReserveList}}
    // constructor
    {{.Name}}() {
{{- range .}}
      {{.Name}}.resize({{.Size}});{{end}}
    }
{{end}}
    // interface
{{- range .BitField}}
{{- if .IsBool}}
    //
    bool get{{.CapName}}() const { return bit_field.{{.Name}}; }
    void set{{.CapName}}(bool f) { bit_field.{{.Name}} = f; }
{{- else if .IsSigned}}
    //
    signed get{{.CapName}}() const { return bit_field.{{.Name}}; }
    void set{{.CapName}}(signed n) { bit_field.{{.Name}} = n; }
{{- else}}
    //
    unsigned get{{.CapName}}() const { return bit_field.{{.Name}}; }
    void set{{.CapName}}(unsigned n) { bit_field.{{.Name}} = n; }{{end}}
{{- end}}
{{- range .Members}}
{{- if .Container}}
    const {{.Type}}{{.Ref}} get{{.CapName}}(int idx) const { return {{.Name}}[idx]; }
    void set{{.CapName}}(int idx, {{.Type}}{{.Ref}} n) { {{.Name}}[idx] = n; }
    size_t get{{.CapName}}Size() const { return {{.Name}}.size(); }
    void append{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}}.emplace_back(n); }
    void resize{{.CapName}}(size_t sz) { {{.Name}}.resize(sz); }
{{- else}}
    const {{.Type}}{{.Ref}} get{{.CapName}}() const { return {{.Name}}; }
    void set{{.CapName}}({{.Type}}{{.Ref}} n) { {{.Name}} = n; }
{{- end}}
{{- end}}
  };
{{- end}}
