require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

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
