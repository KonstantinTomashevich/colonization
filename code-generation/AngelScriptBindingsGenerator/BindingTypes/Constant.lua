if CreateNewClass == nil then
    CreateNewClass = require (scriptDirectory .. "BindingTypes/CreateNewClass")
end

if Tokens == nil then
    Tokens = require (scriptDirectory .. "Tokenization/Tokens")
end

Constant = CreateNewClass ()
Constant.Construct = function (self, fileName, bindingAguments)
    self.fileName = fileName
    self.type = ""
    self.name = ""
    self.arguments = bindingAguments
end

-- Return true if no errors.
Constant.Parse = function (self, tokensList)
    if not self:ReadType (tokensList) then
        return false

    elseif not self:ReadName (tokensList) then
        return false

    else
        return true
    end
end

Constant.ToString = function (self, indent)
    local string = indent .. self.type .. " " .. self.name .. " from file " .. self.fileName .. "\n"
    return string
end

Constant.ReadType = function (self, tokensList)
    local token = tokensList:CurrentToken ()
    while token.type == Tokens.EndOfLine and token ~= nil do
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("End of file reached while trying to read type while reading constant!")
        return false

    elseif token.type ~= Tokens.TypeOrName then
        print ("Line " .. token.line .. ": Expected constant type, but got \"" .. token.value .. "\"!")
        return false

    else
        self.type = token.value
        return true
    end
end

Constant.ReadName = function (self, tokensList)
    local token = tokensList:NextToken ()
    while token.type == Tokens.EndOfLine and token ~= nil do
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("End of file reached while trying to read type while reading constant!")
        return false

    elseif token.type ~= Tokens.TypeOrName then
        print ("Line " .. token.line .. ": Expected constant type, but got \"" .. token.value .. "\"!")
        return false

    else
        self.name = token.value
        return true
    end
end

Constant.GetDataDestination = function ()
    return "constants"
end

Constant.GetTypeName = function ()
    return "Constant"
end

return Constant;
