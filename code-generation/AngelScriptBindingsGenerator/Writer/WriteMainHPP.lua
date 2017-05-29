local IncludesWriter = Class ()
IncludesWriter.Construct = function (self)

end

IncludesWriter.Write = function (self, outputFile)
    print ("Writing main hpp includes.")
    return true
end

local BodyWriter = Class ()
BodyWriter.Construct = function (self)

end

BodyWriter.Write = function (self, outputFile)
    print ("Writing main hpp body.")
    return true
end

function WriteMainHPP ()
    local inputFileName = configuration.outputCppFileTemplate
    local fileName = ConfigurationUtils.LocalFileNameToBindingsFilePath (configuration.bindingsFileName .. ".hpp")
    local outputFile = FileUtils.OpenFile (fileName)

    local commands = {}
    commands ["WriteIncludes"] = IncludesWriter ()
    commands ["WriteBody"] = BodyWriter ()

    local result = TemplatesUtils.ProcessTemplateFile (inputFileName, outputFile, commands)
    outputFile:close ()
    return result
end
return WriteMainHPP
