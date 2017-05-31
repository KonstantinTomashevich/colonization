local TokenizeFile = require (scriptDirectory .. "Tokenization/TokenizeFile")
if TokensList == nil then
    TokensList = require (scriptDirectory .. "Tokenization/TokensList")
end
if Tokens == nil then
    Tokens = require (scriptDirectory .. "Tokenization/Tokens")
end
local currentlyProcessing = nil

function ReadFile (fileName)
    local file = io.open (ConfigurationUtils.LocalFileNameToFilePath (fileName), "r")
    local tokens = TokenizeFile (file)
    local tokensList = TokensList (tokens)
    local token = tokensList:CurrentToken ()

    while token ~= nil do
        if currentlyProcessing ~= nil then
            if not currentlyProcessing:Parse (tokensList) then
                currentlyProcessing = nil
                return false
            else
                -- For correct methods parsing, classes do self-insertion before methods parsing.
                if not currentlyProcessing:IsSelfInserted () then
                    table.insert (data [currentlyProcessing:GetDataDestination ()], currentlyProcessing)
                end
                currentlyProcessing = nil
            end
        elseif token.type == Tokens.Command then
            local command = nil
            local arguments = {}

            for part in token.value:gmatch ("%S+") do
                if command == nil and part ~= "" then
                    command = part
                elseif part ~= "" then
                    local argName = nil
                    local argValue = ""
                    part = part:gsub ("=", " ")
                    for argPart in part:gmatch ("%S+") do
                        if argName == nil then
                            argName = argPart
                        elseif argValue == "" then
                            argValue = argPart
                        else
                            argValue = argValue .. "=" .. argPart
                        end
                    end

                    while arguments [argName] ~= nil do
                        argName = argName .. "?"
                    end
                    arguments [argName] = argValue
                end
            end

            if bindables [command] ~= nil then
                currentlyProcessing = bindables [command] (fileName, arguments)
            else
                print ("Line " .. token.line .. ": Unknown command \"" .. command .. "\"!")
            end
        end
        token = tokensList:NextToken ()
    end

    file:close ()
    return true
end

return ReadFile
