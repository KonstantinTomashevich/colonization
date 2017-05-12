-- TODO: Implement arrays support.

function ReadTab (readedLine)
    local index = readedLine:find ("\"") + 1
    _tab = ""
    while readedLine:sub (index, index) == " " do
        _tab = _tab ..  " "
        index = index + 1
    end
    print ("Tab setted: \"" .. _tab .. "\".")
end

function ReadClassName (readedLine)
    local index = readedLine:find (":") + 1
    _className = ""
    while index <= readedLine:len () do
        local char = readedLine:sub (index, index)
        if char ~= " " then
            _className = _className .. char
        end
        index = index + 1
    end
    print ("Class name setted: \"" .. _className .. "\".")
end

function UpdateReadMode (readedLine, fallbackToConfiguration)
    if readedLine:find ("vars") == 1 then
        _readMode = "Vars"
        print ("New read mode: " .. _readMode .. ".")
    elseif readedLine:find ("header") == 1 then
        _readMode = "Header"
        print ("New read mode: " .. _readMode .. ".")
    elseif readedLine:find ("object") == 1 then
        _readMode = "Object"
        print ("New read mode: " .. _readMode .. ".")
    elseif readedLine:find ("bindings") == 1 then
        _readMode = "Bindings"
        print ("New read mode: " .. _readMode .. ".")
    elseif (fallbackToConfiguration) then
        _readMode = "Configuration"
    end
end

function ReadVar (readedLine)
    local varData = {}
    local index = 1
    local reading = ""
    local readProcess = "type"

    while index <= readedLine:len () do
        local char = readedLine:sub (index, index)
        local nextChar;
        if index + 1 <= readedLine:len () then
            nextChar = readedLine:sub (index + 1, index + 1)
        else
            nextChar = " "
        end

        local previousChar;
        if index > 1 then
            previousChar = readedLine:sub (index - 1, index - 1)
        else
            previousChar = " "
        end

        if readProcess == "type" then
            if char ~= " " or reading == "unsigned" or
                nextChar == "," or nextChar == "<" or nextChar == ">" or previousChar == "," then
                reading = reading .. char
            else
                varData ["type"] = reading
                reading = ""
                readProcess = "name"
            end

        elseif readProcess == "name" then
            if char ~= " " and char ~= "=" then
                reading = reading .. char
            elseif char == "=" then
                varData ["name"] = reading
                reading = ""
                readProcess = "default"
            end

        elseif readProcess == "default" then
            if char ~= "$" then
                reading = reading .. char
            else
                varData ["default"] = reading
                reading = ""
                readProcess = "description"
            end

        else
            reading = reading .. char
        end
        index = index + 1
    end

    varData ["description"] = reading
    table.insert (_vars, varData)

    print ("New var:\n    type: " .. varData ["type"] .. "\n" ..
        "    name: " .. varData ["name"] .. "\n" ..
        "    default: " .. varData ["default"] .. "\n" ..
        "    description: " .. varData ["description"])
end

function ProcessHeaderLine (readedLine)
    if readedLine:find ("//@Insert var list") ~= nil then
        WriteHeaderVarsList ()
    elseif readedLine:find ("//@Insert vars getters and setters") ~= nil then
        WriteHeaderVarsGettersAndSetters ()
    else
        _headerFile:write (readedLine .. "\n")
    end
end

function WriteHeaderVarsList ()
    for index, var in ipairs (_vars) do
        _headerFile:write (_tab .. var ["type"] .. " " .. var ["name"] .. ";\n");
    end
end

function WriteHeaderVarsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        _headerFile:write (_tab .. var ["type"] .. " Get" .. VarCodeNameToPrettyName (var ["name"]) .. " () const;\n")
        _headerFile:write (_tab .. "void Set" .. VarCodeNameToPrettyName (var ["name"]) ..
                        " (" .. VarTypeToSetterArgType (var ["type"]) ..
                        var ["name"]:sub (1, var ["name"]:len () - 1) .. ");\n\n")
    end
end

function VarCodeNameToPrettyName (varName)
    return varName:sub (1, 1):upper () .. varName:sub (2, varName:len () - 1);
end

function VarTypeToSetterArgType (varType)
    if _elementalTypes [varType] ~= nil then
        return varType .. " "
    else
        return "const " .. varType .. " &"
    end
end

function ProcessObjectLine (readedLine)
    if readedLine:find ("//@Insert vars initialization") ~= nil then
        WriteObjectVarsInitialization ()
    elseif readedLine:find ("//@Insert attributes registration") ~= nil then
        WriteObjectAttributesRegistration ()
    elseif readedLine:find ("//@Insert vars getters and setters")~= nil then
        WriteObjectVarsGettersAndSetters ()
    else
        _objectFile:write (readedLine .. "\n")
    end
end

function WriteObjectVarsInitialization ()
    local code = ""
    for index, var in ipairs (_vars) do
        code = code .. _tab .. var ["name"] .. " (" .. var ["default"] .. "),\n"
    end
    code = code:sub (1, code:len () - 2)
    _objectFile:write (code .. "\n")
end

function WriteObjectAttributesRegistration ()
    for index, var in ipairs (_vars) do
        local prettyName = VarCodeNameToPrettyName (var ["name"])
        _objectFile:write (_tab .. GetRegistrationMacro (var ["type"]) .. " (\"" ..
                        var ["description"] .. "\", Get" .. prettyName ..
                        ", Set" .. prettyName .. ", " .. var ["type"] ..
                        ", " .. GetVarAttributeDefault (var ["type"], var ["default"]) ..
                        ", Urho3D::AM_DEFAULT);\n")
    end
end

function WriteObjectVarsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        _objectFile:write (var ["type"] .. " " .. _className .. "::Get" .. VarCodeNameToPrettyName (var ["name"]) ..
                        " () const\n" .. "{\n" .. _tab .. "return " .. var ["name"] .. ";\n}\n\n")
        _objectFile:write ("void " .. _className .. "::Set" .. VarCodeNameToPrettyName (var ["name"]) ..
                        " (" .. VarTypeToSetterArgType (var ["type"]) ..
                        var ["name"]:sub (1, var ["name"]:len () - 1) .. ")\n{\n" ..
                        _tab .. var ["name"] .. " = " .. var ["name"]:sub (1, var ["name"]:len () - 1) ..
                        ";\n}\n\n")
    end
end

function GetVarAttributeDefault (varType, varDefault)
    if _elementalTypes [varType] ~= nil then
        return varDefault
    else
        return varType .. " (" .. varDefault .. ")"
    end
end

function GetRegistrationMacro (varType)
    if _elementalTypes [varType] ~= nil then
        return "URHO3D_ACCESSOR_ATTRIBUTE"
    else
        return "URHO3D_MIXED_ACCESSOR_ATTRIBUTE"
    end
end

function ProcessBindingsLine (readedLine)
    if readedLine:find ("//@Insert getters and setters bindings") ~= nil then
        WriteBindingsGettersAndSetters ()
    else
        _bindingsFile:write (readedLine .. "\n")
    end
end

function WriteBindingsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                             VarCodeTypeToBindingsVarType (var ["type"]) .. " get_" ..
                             var ["name"]:sub (1, var ["name"]:len () - 1) .. " () const\", " ..
                             "asMETHOD (" .. _className .. ", Get" .. VarCodeNameToPrettyName (var ["name"]) ..
                             "), asCALL_THISCALL);\n")
        _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                             "void set_" .. var ["name"]:sub (1, var ["name"]:len () - 1) ..
                             " (" .. VarCodeTypeToBindingsVarType (var ["type"]) .. " " ..
                             var ["name"]:sub (1, var ["name"]:len () - 1).. ")\", " ..
                             "asMETHOD (" .. _className .. ", Set" .. VarCodeNameToPrettyName (var ["name"]) ..
                             "), asCALL_THISCALL);\n\n")
    end
end

function VarCodeTypeToBindingsVarType (varType)
    local bindingsType = varType
    bindingsType = bindingsType:gsub ("unsigned ", "u")
    bindingsType = bindingsType:gsub ("long", "int64")

    local index = bindingsType:find ("::")
    if index == nil then
        return bindingsType
    else
        while index ~= nil do
            local toRemove = "::"
            local nextScanIndex = index - 1

            if nextScanIndex ~= 0 then
                local char = bindingsType:sub (nextScanIndex, nextScanIndex)
                while char ~= " " and char ~= "," and char ~= "<" and char ~= ">" and nextScanIndex ~= 0 do
                    toRemove = char .. toRemove
                    nextScanIndex = nextScanIndex - 1
                    char = bindingsType:sub (nextScanIndex, nextScanIndex)
                end
            end
            bindingsType = bindingsType:gsub (toRemove, "")
            index = bindingsType:find ("::")
        end
        return bindingsType
    end
end

_inputFileName = arg [1]
_outputHeaderFileName = arg [2]
_outputObjectFileName = arg [3]
_outputBindingsObjectFileName = arg [4]
if _inputFileName == nil or _outputHeaderFileName == nil or _outputObjectFileName == nil or _outputBindingsObjectFileName == nil then
    print ("Incorrect input arguments!\nExpected arguments:\n1th -- input file name,\n"..
        "2th -- output header file name,\n3th -- output C++ object file name,\n" ..
        "4th -- output bindings C++ file name!");
    return 1
end

print ("Input: " .. _inputFileName .. "\nOutput Header: " .. _outputHeaderFileName .. "\nOutput Object: " .. _outputObjectFileName)
print ("WARNING: Currently, arrays (Urho3D::PODVector and Urho3D::Vector) NOT supported!")

--- Defaults.
_tab = "    "
_className = "Default_className"

--- Elemental types for correct setters generation.
_elementalTypes = {}
_elementalTypes ["float"] = 1
_elementalTypes ["double"] = 1
_elementalTypes ["bool"] = 1
_elementalTypes ["int"] = 1
_elementalTypes ["unsigned"] = 1
_elementalTypes ["long"] = 1
_elementalTypes ["unsigned int"] = 1
_elementalTypes ["unsigned long"] = 1

--- Current read mode.
_readMode = "Configuration"

--- Readed vars list.
_vars = {}

--- Files
_headerFile = io.open (_outputHeaderFileName, "w+")
_objectFile = io.open (_outputObjectFileName, "w+")
_bindingsFile = io.open (_outputBindingsObjectFileName, "w+")

for readedLine in io.lines (_inputFileName) do
    if _readMode == "Configuration" then

        if readedLine:find ("tab:") == 1 then
            ReadTab (readedLine)
        elseif readedLine:find ("className:") == 1 then
            ReadClassName (readedLine)
        else
            UpdateReadMode (readedLine, true)
        end

    elseif _readMode == "Vars" then
        UpdateReadMode (readedLine, false)
        if _readMode == "Vars" and readedLine:len () > 10 then
            ReadVar (readedLine)
        end

    elseif _readMode == "Header" then
        UpdateReadMode (readedLine, false)
        if _readMode == "Header" then
            ProcessHeaderLine (readedLine)
        end

    elseif _readMode == "Object" then
        UpdateReadMode (readedLine, false)
        if _readMode == "Object" then
            ProcessObjectLine (readedLine)
        end

    elseif _readMode == "Bindings" then
        UpdateReadMode (readedLine, false)
        if _readMode == "Bindings" then
            ProcessBindingsLine (readedLine)
        end
    end
end

_headerFile:close ()
_objectFile:close ()
_bindingsFile:close ()
