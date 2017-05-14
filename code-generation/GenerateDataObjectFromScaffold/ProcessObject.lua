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
        code = code .. _tab .. var.name .. " (" .. var.default .. "),\n"
    end
    code = code:sub (1, code:len () - 2)
    _objectFile:write (code .. "\n")
end

function WriteObjectAttributesRegistration ()
    for index, var in ipairs (_vars) do
        local prettyName = VarCodeNameToPrettyName (var.name)
        if IsArrayType (var.type) then
            _objectFile:write (_tab .. "URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE (\"" ..
                               var.description .. "\", Get" .. prettyName .. "Attribute, Set" ..
                               prettyName .. "Attribute, Urho3D::VariantVector, " ..
                               "Urho3D::Variant::emptyVariantVector, " ..
                               var.name:sub (1, var.name:len () - 1) .. "StructureElementsNames, Urho3D::AM_DEFAULT);\n")
        else
            _objectFile:write (_tab .. GetRegistrationMacro (var.type) .. " (\"" ..
                            var.description .. "\", Get" .. prettyName ..
                            ", Set" .. prettyName .. ", " .. var.type ..
                            ", " .. GetVarAttributeDefault (var.type, var.default) ..
                            ", Urho3D::AM_DEFAULT);\n")
        end
    end
end

function WriteObjectVarsGettersAndSetters ()
    for index, var in ipairs (_vars) do
        _objectFile:write (var.type .. " " .. _className .. "::Get" .. VarCodeNameToPrettyName (var.name) ..
                        " () const\n" .. "{\n" .. _tab .. "return " .. var.name .. ";\n}\n\n")
        _objectFile:write ("void " .. _className .. "::Set" .. VarCodeNameToPrettyName (var.name) ..
                        " (" .. VarTypeToSetterArgType (var.type) ..
                        var.name:sub (1, var.name:len () - 1) .. ")\n{\n" ..
                        _tab .. var.name .. " = " .. var.name:sub (1, var.name:len () - 1) ..
                        ";\n}\n\n")

        if IsArrayType (var.type) then
            local shortenedName = var.name:sub (1, var.name:len () - 1)
            local prettyName = VarCodeNameToPrettyName (var.name)

            _objectFile:write ("Urho3D::VariantVector " .. _className .. "::Get" .. prettyName .. "Attribute () const\n{\n" ..
                               _tab .. "Urho3D::VariantVector variantVector;\n" ..
                               _tab .. "variantVector.Push (Urho3D::Variant (" .. var.name .. ".Size ()));\n" ..
                               _tab .. "for (int index = 0; index < " .. var.name .. ".Size (); index++)\n" ..
                               _tab .. "{\n" ..
                               _tab .. _tab .. "variantVector.Push (Urho3D::Variant (" .. var.name .. ".At (index)));\n" ..
                               _tab .. "}\n" ..
                               _tab .. "return variantVector;\n}\n\n")

            _objectFile:write ("void " .. _className .. "::Set" .. prettyName ..
                               "Attribute (const Urho3D::VariantVector &" .. shortenedName .. ")\n{\n" ..
                               _tab .. var.name .. ".Clear ();\n" ..
                               _tab .. "if (!" .. shortenedName .. ".Empty ())\n" .. _tab .. "{\n" ..
                               _tab .. _tab .. "int requestedSize = " .. shortenedName .. ".At (0).GetInt ();\n" ..
                               _tab .. _tab .. "if (requestedSize > 0)\n" .. _tab .. _tab .. "{\n" ..
                               _tab .. _tab .. _tab .. "for (int index = 0; index < requestedSize; index++)\n" ..
                               _tab .. _tab .. _tab .. "{\n" ..
                               _tab .. _tab .. _tab .. _tab .. "if (index + 1 < " .. shortenedName .. ".Size ())\n" ..
                               _tab .. _tab .. _tab .. _tab .. "{\n" ..
                               _tab .. _tab .. _tab .. _tab .. _tab .. var.name .. ".Push (" .. shortenedName .. ".At (index + 1).Get" .. VarCodeTypeToBindingsVarType (GetArrayValueType (var.type)) .. " ());\n" ..
                               _tab .. _tab .. _tab .. _tab .. "}\n" ..
                               _tab .. _tab .. _tab .. _tab .. "else\n" ..
                               _tab .. _tab .. _tab .. _tab .. "{\n" ..
                               _tab .. _tab .. _tab .. _tab .. _tab .. var.name .. ".Push (" .. GetArrayValueType (var.type) .. " ());\n"..
                               _tab .. _tab .. _tab .. _tab .. "}\n" ..
                               _tab .. _tab .. _tab .. "}\n" ..
                               _tab .. _tab .. "}\n" ..
                               _tab .. "}\n" ..
                               "}\n\n")
        end
    end
end
