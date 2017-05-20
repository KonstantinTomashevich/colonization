scriptDirectory = ""
local PrintConfiguration = require (scriptDirectory .. "PrintConfiguration")
local ReadFile = require (scriptDirectory .. "ReadFile")

Class = require (scriptDirectory .. "BindingTypes/Class")
data = require (scriptDirectory .. "Data")

bindingTypes = {}
bindingTypes ["Enum"] = require (scriptDirectory .. "BindingTypes/Enum")

local configurationFile = arg [1]:gsub (".lua", "")
configuration = require (configurationFile)
PrintConfiguration ()

print ("Reading files...")
local filesCount = #configuration.files
for index, fileName in ipairs (configuration.files) do
    print ("[" .. (index * 100.0 / filesCount) .. "%] " .. fileName)
    ReadFile (fileName)
end
print ("\n")
