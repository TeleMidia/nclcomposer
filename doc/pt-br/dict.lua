local dict = {
  ["nclcomposer"]     = "NCL Composer",
  ["structural_view"] = "Visão Estrutural",
  ["layout_view"]     = "Visão de Leiaute",
  ["outline_view"]    = "Visão de Outline",
  ["properties_view"] = "Visão de Propriedades",
  ["textual_view"]    = "Visão Textual",
  ["rules_view"]      = "Visão de Regras",
  ["validator"]       = "Visão de Validação",
  ["default_views"]   = "Visão Estrutural, Visão de Leiaute, Visão de Outline, Visão Proprieades, Visão de Regras, Visão de Validaćão"
}

local f = assert(io.open(arg[1], "r"))
local t = f:read("*all")

--Replace strings
x = string.gsub(t, "%\{([%w_]+)%}", dict)

print (x)
f:close()
