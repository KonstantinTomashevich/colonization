if CreateNewClass == nil then
    CreateNewClass = require (scriptDirectory .. "CreateNewClass")
end

if Tokens == nil then
    Tokens = require (scriptDirectory .. "Tokenization/Tokens")
end

if TokenToString == nil then
    TokenToString, TokenTypeToString = require (scriptDirectory .. "Tokenization/TokenToString")
end

Urho3DSubsystem = CreateNewClass ()
Urho3DSubsystem.Construct = function (self, fileName, bindingAguments)
    self.fileName = fileName
    self.name = ""
    self.arguments = bindingAguments
end

-- Return true if no errors.
Urho3DSubsystem.Parse = function (self, tokensList)
    tokensList.skipEndOfLineTokens = true
    local token = tokensList:CurrentOrNextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.Command then
        print ("Line " .. token.line .. ": Expected subsystem name as binder command, but got " .. TokenToString (token) .. "!")
        return false
    else
        self.name = token.value
        return true
    end
end

Urho3DSubsystem.ToString = function (self, indent)
    local string = indent .. self.name .. " from file " .. self.fileName .. "\n"
    return string
end

Urho3DSubsystem.GetDataDestination = function ()
    return "subsystems"
end

Urho3DSubsystem.GetTypeName = function ()
    return "Urho3DSubsystem"
end

return Urho3DSubsystem