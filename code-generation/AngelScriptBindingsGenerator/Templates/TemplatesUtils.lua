TemplatesUtils = {}
TemplatesUtils.ProcessTemplateString = function (string, vars)
    local resultString = string
    for key, value in pairs (vars) do
        resultString = resultString:gsub ("${" .. key .. "}", value)
    end
    return resultString
end

TemplatesUtils.ProcessTemplateFile = function (inputFile, outputFile, commands)
    for line in inputFile:lines () do
        local isCommand = false
        for command, processor in pairs (commands) do
            if not isCommand then
                if line:find (configuration.bindingsGeneratorCommand .. " " .. command) ~= nil then
                    processor:Write (outputFile)
                    isCommand = true
                end
            end
        end

        if not isCommand then
            outputFile:write (line)
        end
    end
end
return TemplatesUtils
