Class = CreateNewClass ()
Class.Construct = function (self, fileName, bindingAguments)
    self.fileName = fileName
    self.name = ""
    self.publicBases = {}
    self.constructors = {}
    self.methods = {}

    self.bindingName = ""
    self.bindingPublicBases = {}
    self.arguments = bindingAguments
    self.openedBraces = 0
end

-- Return true if no errors.
Class.Parse = function (self, tokensList)
    tokensList.skipEndOfLineTokens = true
    return (self:SkipUntilClassKeyword (tokensList) and self:ReadName (tokensList) and
        self:ReadBases (tokensList) and self:PreInsertSelfToData () and self:ReadContent (tokensList))
end

Class.ToString = function (self, indent)
    local string = indent .. self.bindingName
    if self.name ~= self.bindingName then
        string = string .. " (from " .. self.name .. ")"
    end
    string = string .. " from file " .. self.fileName .. "\n" .. indent ..
        "    " .. "inherits "

    for key, value in pairs (self.bindingPublicBases) do
        local base = DataUtils.GetNamedValueOfTable (data.classes, value)
        if base ~= nil then
            if base.bindingName == value then
                string = string .. value .. ", "
            else
                string = string .. base.bindingName .. " (from " .. value .. ") "
            end
        else
            string = string .. value .. " (external) "
        end
    end

    string = string .. "\n" .. indent .. "{\n" .. indent .. "constructors:\n"
    for key, value in pairs (self.constructors) do
        string = string .. value:ToString (indent .. "    ") .. "\n"
    end

    string = string .. "\n" .. indent .. "methods:\n"
    for key, value in pairs (self.methods) do
        string = string .. value:ToString (indent .. "    ") .. "\n"
    end
    string = string .. indent .. "}\n"
    return string
end

Class.ApplyArguments = function (self)
    self.bindingName = TypeUtils.ConvertCXXToASType (self.name)
    for index, value in ipairs (self.publicBases) do
        self.bindingPublicBases [value] = TypeUtils.ConvertCXXToASType (value)
    end

    for key, value in pairs (self.arguments) do
        key = key:gsub ("?", "")
        if key == "OverrideName" then
            self.bindingName = value

        elseif key:find ("ExcludeBase_") == 1 then
            local toExclude = key:sub (("ExcludeBase_"):len () + 1, key:len ())
            self.bindingPublicBases [toExclude] = nil
        end
    end
end

Class.SkipUntilClassKeyword = function (self, tokensList)
    local token = tokensList:CurrentOrNextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.TypeOrName and token.value ~= "class" then
        print ("Line " .. token.line .. ": Expected \"class\", but got \"" .. TokenToString (token) .. "\"!")
        return false
    else
        return true
    end
end

Class.ReadName = function (self, tokensList)
    local token = tokensList:NextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    elseif token.type ~= Tokens.TypeOrName then
        print ("Line " .. token.line .. ": Expected class name, but got \"" .. TokenToString (token) .. "\"!")
        return false
    else
        self.name = token.value;
        -- Set binding name there, because it affects names of constructors and static functions.
        if self.arguments ["OverrideName"] ~= nil then
            self.bindingName = self.arguments ["OverrideName"]
        else
            self.bindingName = self.name
        end
        return true
    end
end

Class.ReadBases = function (self, tokensList)
    local token = tokensList:NextToken ()
    if token == nil then
        print ("Fatal error, token is nil!")
        return false

    elseif token.type ~= Tokens.Operator then
        print ("Line " .. token.line .. ": Expected \":\" or \"{\", but got \"" .. TokenToString (token) .. "\"!")
        return false

    elseif token.value == "{" then
        -- There are no bases.
        return true

    elseif token.value == ":" then
        token = tokensList:NextToken ();
        local isNextBasePublic = false
        while token ~= nil and (token.type ~= Tokens.Operator and token.value ~= "{") do
            if token.type == Tokens.Operator and token.value == "," and isNextBasePublic then
                print ("Line " .. token.line .. ": Expected public base class name, but got \"" .. TokenToString (token) .. "\"!")
                return false

            elseif token.type == Tokens.TypeOrName then
                if token.value == "public" then
                    isNextBasePublic = true
                elseif isNextBasePublic then
                    table.insert (self.publicBases, token.value)
                    isNextBasePublic = false
                end
            end
            token = tokensList:NextToken ()
        end

        if token == nil then
            print ("Fatal error, token is nil!")
            return false
        else
            self.openedBraces = 1
            return true
        end

    else
        print ("Line " .. token.line .. ": Expected \":\" or \"{\", but got \"" .. TokenToString (token) .. "\"!")
        return false
    end
end

Class.PreInsertSelfToData = function (self)
    table.insert (data [self:GetDataDestination ()], self)
    return true
end

Class.ReadContent = function (self, tokensList)
    local token = tokensList:NextToken ()
    local currentChildReader = nil
    while token ~= nil and self.openedBraces > 0 do
        if currentChildReader ~= nil then
            if not currentChildReader:Parse (tokensList) then
                return false
            elseif currentChildReader:GetTypeName () ~= "Function" then
                print ("Line " .. token.line .. ": Class can contain only functions and constructors, but got \"" ..
                    currentChildReader:GetTypeName () .. "\"!")
                return false

            elseif currentChildReader.isConstructor then
                currentChildReader.ownerClassName = self.name
                currentChildReader.name = self.bindingName
                currentChildReader:ApplyArguments ()
                table.insert (self.constructors, currentChildReader)
                currentChildReader = nil

            else
                currentChildReader.ownerClassName = self.name
                if currentChildReader.isStatic then
                    currentChildReader.name = self.bindingName .. currentChildReader.name
                    currentChildReader:ApplyArguments ()
                    table.insert (data [currentChildReader:GetDataDestination ()], currentChildReader)
                else
                    currentChildReader:ApplyArguments ()
                    table.insert (self.methods, currentChildReader)
                end
                currentChildReader = nil
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
                currentChildReader = bindables [command] (self.fileName, arguments)
            else
                print ("Line " .. token.line .. ": Unknown command \"" .. command .. "\"!")
            end
        end

        token = tokensList:CurrentOrNextToken ()
        if token.type == Tokens.Operator then
            if token.value == "{" then
                self.openedBraces = self.openedBraces + 1
            elseif token.value == "}" then
                self.openedBraces = self.openedBraces - 1
            end
        end
        token = tokensList:NextToken ()
    end

    if token == nil then
        print ("Fatal error, token is nil!")
        return false
    else
        return true
    end
end

Class.GetDataDestination = function ()
    return "classes"
end

Class.GetTypeName = function ()
    return "Class"
end

return Class
