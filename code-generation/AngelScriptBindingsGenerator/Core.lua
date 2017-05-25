Core = {}
Core.LoadCoreScripts = function ()
    PrintConfiguration = require (scriptDirectory .. "PrintConfiguration")
    ReadFile = require (scriptDirectory .. "ReadFile")

    Class = require (scriptDirectory .. "CreateNewClass")
    data, GetBindingTypesOfFile = require (scriptDirectory .. "Data")
end

Core.LoadBindables = function ()
    bindables = {}
    bindables ["Enum"] = require (scriptDirectory .. "Bindables/Enum")
    bindables ["Constant"] = require (scriptDirectory .. "Bindables/Constant")
    bindables ["Function"] = require (scriptDirectory .. "Bindables/Function")
    bindables ["Constructor"] = function (fileName, bindingAguments) return bindables ["Function"] (fileName, bindingAguments, true) end
    bindables ["Urho3DSubsystem"] = require (scriptDirectory .. "Bindables/Urho3DSubsystem")
    bindables ["Class"] = require (scriptDirectory .. "Bindables/Class")
end

Core.LoadConfiguration = function (scriptPath)
    local configurationFile = scriptPath
    configuration = require (configurationFile)
    PrintConfiguration ()
end

Core.ParseInputFiles = function ()
    print ("Parsing files...")
    local filesCount = #configuration.files
    for index, fileName in ipairs (configuration.files) do
        print ("[" .. (index * 100.0 / filesCount) .. "%] " .. fileName)
        if not ReadFile (fileName) then
            print ("Error while reading and parsing file!")
            return
        end
    end
    print ("\n")
end

Core.PrintParsedBindables  = function ()
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
end
return Core
