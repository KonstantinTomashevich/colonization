Core = {}
Core.LoadCoreScripts = function ()
    ConfigurationUtils = require (scriptDirectory .. "Core/ConfigurationUtils")
    DataUtils = require (scriptDirectory .. "Core/DataUtils")
    TemplatesUtils = require (scriptDirectory .. "Templates/TemplatesUtils")
    TypeUtils = require (scriptDirectory .. "Core/TypeUtils")
    FileUtils = require (scriptDirectory .. "Core/FileUtils")

    Templates = require (scriptDirectory .. "Templates/Templates")
    Class = require (scriptDirectory .. "Core/CreateNewClass")
    ReadFile = require (scriptDirectory .. "Core/ReadFile")
    WriteFile = require (scriptDirectory .. "Writer/WriteFile")
    WriteMainCpp = require (scriptDirectory .. "Writer/WriteMainCpp")
    WriteMainHpp = require (scriptDirectory .. "Writer/WriteMainHpp")

    data = require (scriptDirectory .. "Core/Data")
    return true
end

Core.LoadBindables = function ()
    bindables = {}
    bindables ["Enum"] = require (scriptDirectory .. "Bindables/Enum")
    bindables ["Constant"] = require (scriptDirectory .. "Bindables/Constant")
    bindables ["Function"] = require (scriptDirectory .. "Bindables/Function")
    bindables ["Constructor"] = function (fileName, bindingAguments) return bindables ["Function"] (fileName, bindingAguments, true) end
    bindables ["Urho3DSubsystem"] = require (scriptDirectory .. "Bindables/Urho3DSubsystem")
    bindables ["Class"] = require (scriptDirectory .. "Bindables/Class")
    return true
end

Core.LoadConfiguration = function (scriptPath)
    local configurationFile = scriptPath
    configuration = require (configurationFile)
    ConfigurationUtils.PrintConfiguration ()
    return true
end

Core.ParseInputFiles = function ()
    print ("### Parsing files...")
    local filesCount = #configuration.files
    for index, fileName in ipairs (configuration.files) do
        print ("    [" .. (index * 100.0 / filesCount) .. "%] " .. fileName)
        if not ReadFile (fileName) then
            print ("Error while reading and parsing file!")
            return false
        end
    end
    print ("\n")
    return true
end

Core.ApplyArguments = function ()
    print ("### Applying arguments...")
    local toApply = {"enums", "constants", "freeFunctions", "classes", "subsystems"}
    for itemIndex, toApplyItem in ipairs (toApply) do
        for index, value in ipairs (data [toApplyItem]) do
            value:ApplyArguments ()
        end
    end
    return true
end

Core.PrintParsedBindables = function ()
    print ()
    local toPrint = {}
    table.insert (toPrint, {name = "### Enums:", key = "enums"})
    table.insert (toPrint, {name = "### Constants:", key = "constants"})
    table.insert (toPrint, {name = "### Free Functions:", key = "freeFunctions"})
    table.insert (toPrint, {name = "### Classes:", key = "classes"})
    table.insert (toPrint, {name = "### Subsystems:", key = "subsystems"})
    for key, value in pairs (toPrint) do
        print (value.name)
        for index, value in pairs (data [value.key]) do
            print (value:ToString ("    "))
        end
        print ("")
    end
    return true
end

Core.CreateAndPrintFilesToWriteList = function ()
    print ("### Files will be generated:\n    " ..
        ConfigurationUtils.LocalFileNameToBindingsFilePath (configuration.bindingsFileName .. ".cpp") .. "\n    " ..
        ConfigurationUtils.LocalFileNameToBindingsFilePath (configuration.bindingsFileName .. ".hpp"))

    data.filesToWriteList = {}
    for index, fileName in ipairs (configuration.files) do
        local fileBindables = DataUtils.GetBindablesOfFile (fileName)
        if #fileBindables > 0 then
            table.insert (data.filesToWriteList, {name = fileName, bindables = fileBindables})
            print ("    " .. ConfigurationUtils.LocalFileNameToBindingsFilePath (fileName))
        end
    end
    print ("")
    return true
end

Core.WriteBindings = function ()
    print ("### Writing bindings...")
    local filesCount = #data.filesToWriteList + 2

    print ("    [" .. (1 * 100.0 / filesCount) .. "%] " ..
        ConfigurationUtils.LocalFileNameToBindingsFilePath (configuration.bindingsFileName .. ".cpp"))
    if not WriteMainCpp () then
        return false
    end

    print ("    [" .. (2 * 100.0 / filesCount) .. "%] " ..
        ConfigurationUtils.LocalFileNameToBindingsFilePath (configuration.bindingsFileName .. ".hpp"))
    if not WriteMainHpp () then
        return false
    end

    for index, fileData in ipairs (data.filesToWriteList) do
        print ("    [" .. ((index + 2) * 100.0 / filesCount) .. "%] " .. fileData.name)
        if not WriteFile (fileData) then
            return false
        end
    end
    return true
end
return Core
