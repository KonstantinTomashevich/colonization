if CreateNewClass == nil then
    CreateNewClass = require (scriptDirectory .. "BindingTypes/CreateNewClass")
end

if Tokens == nil then
    Tokens = require (scriptDirectory .. "Tokenization/Tokens")
end

Function = CreateNewClass ()
Function.Construct = function (self, fileName, bindingAguments, isConstructor)
    self.fileName = fileName
    self.ownerClassName = nil
    self.returnType = ""
    self.name = ""
    self.callArguments = {}

    self.isConst = false
    self.isStatic = false
    if isConstructor ~= nil then
        self.isConstructor = isConstructor
    else
        self.isConstructor = false
    end
    self.arguments = bindingAguments
end

-- Return true if no errors.
Function.Parse = function (self, tokensList)
    tokensList.skipEndOfLineTokens = true
    return ((self.isConstructor or self:ReadReturnType (tokensList)) and
        self:ReadName (tokensList) and self:ReadCallArguments (tokensList) and self:ReadIsConst (tokensList))
end

Function.ToString = function (self, indent)
    local string = indent .. self.returnType .. " " .. self.name .. " from file " .. self.fileName .. "\n" .. indent .. "(\n"
    for key, value in pairs (self.callArguments) do
        string = string .. indent .. "    " .. value.type .. " " .. value.name
        if value.default ~= nil then
            string = string .. " = " .. value.default
        end
        string = string .. "\n"
    end

    string = string .. indent .. ")"
    if self.isConst then
        string = string .. " const"
    end
    string = string .. "\n"
    return string
end

Function.ReadReturnType = function (self, tokensList)
    local token = tokensList:CurrentOrNextToken ()
    if token.type == Tokens.TypeOrName and token.value == "static" then
        self.isStatic = true
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.TypeOrName then
        print ("Line " .. token.line .. ": Expected function return type, but got \"" .. token.value .. "\"!")
        return false
    else
        self.returnType = token.value
        return true
    end
end

Function.ReadName = function (self, tokensList)
    local token = nil
    if (self.isConstructor) then
        token = tokensList:CurrentOrNextToken ()
    else
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.TypeOrName then
        print ("Line " .. token.line .. ": Expected function name, but got \"" .. token.value .. "\"!")
        return false
    else
        self.name = token.value
        return true
    end
end

Function.ReadCallArguments = function (self, tokensList)
    if not self:SkipUntilArgumentsListBegin (tokensList) then
        return false
    end

    local whatNextNameTokenWillBe = "type"
    local token = tokensList:NextToken ()
    local currentArg = nil

    while token ~= nil and not (token.type == Tokens.Operator and token.value == ")") do
        if currentArg == nil then
            currentArg = {}
        end

        if token.type == Tokens.TypeOrName then
            if whatNextNameTokenWillBe == "type" then
                currentArg.type = token.value
                whatNextNameTokenWillBe = "name"

            elseif whatNextNameTokenWillBe == "name" then
                currentArg.name = token.value
                whatNextNameTokenWillBe = ""

            elseif whatNextNameTokenWillBe == "default" then
                currentArg.default = token.value
                whatNextNameTokenWillBe = ""
            end

        elseif token.type == Tokens.Operator and token.value == "=" then
            if currentArg.type == nil or currentArg.name == nil then
                print ("Line " .. token.line .. ": Call argument type or name is not readed, but got \"=\"!")
                return false
            else
                whatNextNameTokenWillBe = "default"
            end

        elseif token.type == Tokens.Operator and token.value == "," then
            if whatNextNameTokenWillBe == "default" then
                print ("Line " .. token.line .. ": Expected call argument default, but got \",\"!")
                return false

            elseif currentArg.type == nil or currentArg.name == nil then
                print ("Line " .. token.line .. ": Call argument type or name is not readed, but got \",\"!")
                return false

            else
                whatNextNameTokenWillBe = "type"
                table.insert (self.callArguments, currentArg)
                currentArg = nil
            end

        else
            print ("Line " .. token.line .. ": Unexpected token \"" .. token.value .. "\" while reading function call arguments!")
            return false
        end
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("End of file reached while trying to read call arguments while reading function!")
        return false
    elseif currentArg ~= nil then
        if currentArg.type == nil or currentArg.name == nil then
            print ("Line " .. token.line .. ": Call argument type or name is not readed, but got \")\"!")
            return false
        else
            table.insert (self.callArguments, currentArg)
            currentArg = nil
            return true
        end
    else
        return true
    end
end

Function.ReadIsConst = function (self, tokensList)
    local token = tokensList:NextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type == Tokens.Operator and (token.value == ";" or token.value == "{") then
        return true
    elseif token.type == Tokens.TypeOrName then
        if token.value == "const" then
            self.isConst = true
            return true
        else
            print ("Line " .. token.line .. ": Expected function declaration end, but got \"" .. token.value .. "\"!")
            return false
        end
    end
end

Function.SkipUntilArgumentsListBegin = function (self, tokensList)
    local token = tokensList:NextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.Operator and token.value ~= "(" then
        print ("Line " .. token.line .. ": Expected \"(\", but got \"" .. token.value .. "\"!")
        return false
    else
        return true
    end
end

Function.GetDataDestination = function ()
    return "freeFunctions"
end

Function.GetTypeName = function ()
    return "Function"
end

return Function;
