require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

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
