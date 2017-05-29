local IncludesWriter = Class ()
IncludesWriter.Construct = function (self, bindables)
    self.bindables = bindables
end

IncludesWriter.Write = function (self, outputFile)
    print ("Writing includes.")
    return true
end

local BodyWriter = Class ()
BodyWriter.Construct = function (self, bindables)
    self.bindables = bindables
end

BodyWriter.Write = function (self, outputFile)
    print ("Writing body.")
    return true
end

function WriteFile (fileData)
    local inputFileName = configuration.outputHppFileTemplate
    local outputFile = FileUtils.OpenFile (ConfigurationUtils.LocalFileNameToBindingsFilePath (fileData.name))

    local commands = {}
    commands ["WriteIncludes"] = IncludesWriter (fileData.bindables)
    commands ["WriteBody"] = BodyWriter (fileData.bindables)

    local result = TemplatesUtils.ProcessTemplateFile (inputFileName, outputFile, commands)
    outputFile:close ()
    return result
end
return WriteFile
