require (scriptDirectory .. "Globals")
require (scriptDirectory .. "Utils")

function ProcessBindingsLine (readedLine)
    if readedLine:find ("//@Insert getters and setters bindings") ~= nil then
        WriteBindingsGettersAndSetters ()
    elseif readedLine:find ("//@Insert arrays getters and setters") ~= nil then
        WriteArraysGettersAndSetters ()
    else
        _bindingsFile:write (readedLine .. "\n")
    end
end

function WriteBindingsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        local shortenedName = var.name:sub (1, var.name:len () - 1)
        if IsArrayType (var.type) then
            local prettyName = VarCodeNameToPrettyName (var.name)
            local bindingsArrayValueType = VarCodeTypeToBindingsVarType (GetArrayValueType (var.type))

            _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                                 "Array <" .. bindingsArrayValueType .. "> @get_" .. shortenedName .. "() const\", " ..
                                 "asFUNCTION (" .. _className .. "_Get" .. prettyName .. "), asCALL_CDECL_OBJFIRST);\n")

            _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                                 "void set_" .. shortenedName .. "(" .. "Array <" .. bindingsArrayValueType .. "> @" .. shortenedName ..
                                 ")\", asFUNCTION (" .. _className .. "_Set" .. prettyName .. "), asCALL_CDECL_OBJFIRST);\n\n")
        else
            _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                                 VarCodeTypeToBindingsVarType (var.type) .. " get_" .. shortenedName .. " () const\", " ..
                                 "asMETHOD (" .. _className .. ", Get" .. VarCodeNameToPrettyName (var.name) ..
                                 "), asCALL_THISCALL);\n")
            _bindingsFile:write (_tab .. "engine->RegisterObjectMethod (\"" .. _className .. "\", \"" ..
                                 "void set_" .. shortenedName ..
                                 " (" .. VarCodeTypeToBindingsVarType (var.type) .. " " .. shortenedName .. ")\", " ..
                                 "asMETHOD (" .. _className .. ", Set" .. VarCodeNameToPrettyName (var.name) ..
                                 "), asCALL_THISCALL);\n\n")
        end
    end
end

function WriteArraysGettersAndSetters ()
    for index, var in ipairs (_vars) do
        if IsArrayType (var.type) then
            local prettyName = VarCodeNameToPrettyName (var.name)
            local arrayValueType = GetArrayValueType (var.type)
            _bindingsFile:write ("Urho3D::CScriptArray *" .. _className .. "_Get" .. prettyName ..
                                 " (" .. _className .. " *object)\n{\n" ..
                                 _tab .. var.type .. " array = object->Get" .. prettyName .. " ();\n" ..
                                 _tab .. "return Urho3D::VectorToArray <" .. arrayValueType .. "> (array, \"Array <" ..
                                 VarCodeTypeToBindingsVarType (arrayValueType) .. ">\");\n" ..
                                 "}\n\n")

            _bindingsFile:write ("void " .. _className .. "_Set" .. prettyName .. " (" .. _className .. " *object, " ..
                                 "Urho3D::CScriptArray *array)\n{\n" ..
                                 _tab .. "object->Set" .. prettyName .. " (" .. GetArrayBindingsConversionFunction (var.type) ..
                                 " <" .. arrayValueType .. "> (array));\n}\n\n")
        end
    end
end
