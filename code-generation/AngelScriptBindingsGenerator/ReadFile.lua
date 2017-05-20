local data = require (scriptDirectory .. "Data")
local currentlyProcessing = nil

function ReadFile (fileName)
    local file = io.open (configuration.pathPrefix .. "/" .. configuration.projectDir .. "/" .. fileName, "r")
    local lineIndex = 1
    for line in file:lines () do
        if currentlyProcessing ~= nil then
            if currentlyProcessing:Process (line, lineIndex) then
                table.insert (data [currentlyProcessing:GetDataDestination ()], currentlyProcessing)
                currentlyProcessing = nil
            end
        else
            local index = line:find (configuration.bindingsGeneratorCommand)
            if index ~= nil then
                index = index + configuration.bindingsGeneratorCommand:len () + 1
                local commandLine = line:sub (index, line:len ())

                local command = nil
                local arguments = {}
                for element in commandLine:gmatch ("%S+") do
                    if command == nil then
                        command = element
                    else
                        table.insert (arguments, element)
                    end
                end

                local bindingType = bindingTypes [command]
                if bindingType ~= nil then
                    currentlyProcessing = bindingType (fileName, arguments)
                else
                    print ("    Line " .. lineIndex .. ": unknown command \"" .. command .. "\".")
                end
            end
        end
        lineIndex = lineIndex + 1
    end

    if currentlyProcessing ~= nil then
        print ("    Processing of " .. currentlyProcessing:GetTypeName () .. " isn't finished, end of file reached!")
        table.insert (data [currentlyProcessing.GetDataDestination ()], currentlyProcessing)
    end
    file:close ()
end

return ReadFile
