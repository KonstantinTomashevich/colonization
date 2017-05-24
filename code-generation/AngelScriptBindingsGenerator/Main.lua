scriptDirectory = ""
local PrintConfiguration = require (scriptDirectory .. "PrintConfiguration")
local ReadFile = require (scriptDirectory .. "ReadFile")

Class = require (scriptDirectory .. "BindingTypes/CreateNewClass")
data, GetBindingTypesOfFile = require (scriptDirectory .. "Data")

bindingTypes = {}
bindingTypes ["Enum"] = require (scriptDirectory .. "BindingTypes/Enum")
bindingTypes ["Constant"] = require (scriptDirectory .. "BindingTypes/Constant")
bindingTypes ["Function"] = require (scriptDirectory .. "BindingTypes/Function")
bindingTypes ["Constructor"] = function (fileName, bindingAguments) return bindingTypes ["Function"] (fileName, bindingAguments, true) end
bindingTypes ["Urho3DSubsystem"] = require (scriptDirectory .. "BindingTypes/Urho3DSubsystem")
bindingTypes ["Class"] = require (scriptDirectory .. "BindingTypes/Class")

local configurationFile = arg [1]:gsub (".lua", "")
configuration = require (configurationFile)
PrintConfiguration ()

print ("Reading files...")
local filesCount = #configuration.files
for index, fileName in ipairs (configuration.files) do
    print ("[" .. (index * 100.0 / filesCount) .. "%] " .. fileName)
    if not ReadFile (fileName) then
        print ("Error while reading and parsing file!")
        return
    end
end
print ("\n")

local toPrint = {}
table.insert (toPrint, {name = "Enums:", key = "enums"})
table.insert (toPrint, {name = "Constants:", key = "constants"})
table.insert (toPrint, {name = "Free Functions:", key = "freeFunctions"})
table.insert (toPrint, {name = "Classes:", key = "classes"})
table.insert (toPrint, {name = "Subsystems:", key = "subsystems"})
for key, value in pairs (toPrint) do
    print (value.name)
    for index, value in pairs (data [value.key]) do
        print (value:ToString ("    "))
    end
    print ("")
end
