require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

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
