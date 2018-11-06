{{- define "serialize"}}
{{- if .BitField}}    put(bit_field);
{{end}}
{{- end}}
