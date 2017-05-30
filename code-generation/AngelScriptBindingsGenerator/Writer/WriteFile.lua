local IncludesWriter = Class ()
IncludesWriter.Construct = function (self, fileData)
    self.fileName = fileData.name
    self.bindables = fileData.bindables
end

IncludesWriter.Write = function (self, outputFile)
    outputFile:write (TemplatesUtils.ProcessTemplateString (Templates.IncludeFullPath,
                        {file = ConfigurationUtils.LocalFileNameToProjectIncludePath (self.fileName)}))

    local alreadyIncluded = {}
    for index, bindable in ipairs (self.bindables) do
        local requiredIncludes = bindable:GetRequiredBindingsIncludes ()
        for include, include in ipairs (requiredIncludes) do
            if alreadyIncluded [include] == nil then
                outputFile:write (TemplatesUtils.ProcessTemplateString (Templates.IncludeFullPath,
                                    {file = ConfigurationUtils.LocalFileNameToProjectBindingsIncludePath (include)}))
                alreadyIncluded [include] = 1
            end
        end
    end
    return true
end

local BodyWriter = Class ()
BodyWriter.Construct = function (self, fileData)
    self.fileName = fileData.name
    self.bindables = fileData.bindables
end

BodyWriter.Write = function (self, outputFile)
    return true
end

function WriteFile (fileData)
    local inputFileName = configuration.outputHppFileTemplate
    local outputFile = FileUtils.OpenFile (ConfigurationUtils.LocalFileNameToBindingsFilePath (fileData.name))

    local commands = {}
    commands ["WriteIncludes"] = IncludesWriter (fileData)
    commands ["WriteBody"] = BodyWriter (fileData)

    local result = TemplatesUtils.ProcessTemplateFile (inputFileName, outputFile, commands)
    outputFile:close ()
    return result
end
return WriteFile
