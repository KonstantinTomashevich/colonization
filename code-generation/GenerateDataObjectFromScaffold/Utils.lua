require (scriptDirectory .. "Globals")

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
    elseif IsArrayType (varType) then
        return "URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE"
    else
        return "URHO3D_MIXED_ACCESSOR_ATTRIBUTE"
    end
end

function VarCodeTypeToBindingsVarType (varType)
    local bindingsType = varType
    bindingsType = bindingsType:gsub ("unsigned ", "u")
    bindingsType = bindingsType:gsub ("long", "int64")
    bindingsType = bindingsType:gsub ("*", "@")

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

function IsArrayType (varType)
    return (varType:find ("Vector <") ~= nil or varType:find ("PODVector <") ~= nil)
end

function GetArrayValueType (arrayType)
    local index = arrayType:find ("<")
    if index ~= nil then
        index = index + 1
        local char = arrayType:sub (index, index)
        local valueType = ""
        while index <= arrayType:len () and char ~= ">" do
            valueType = valueType .. char
            index = index + 1
            char = arrayType:sub (index, index)
        end
        return valueType
    else
        return "VALUE_TYPE_DETECTION_ERROR"
    end
end

function GetArrayBindingsConversionFunction (arrayType)
    if arrayType:find ("PODVector") ~= nil then
        return "Urho3D::ArrayToPODVector"
    else
        return "Urho3D::ArrayToVector"
    end
end

function GetToArrayBindingsConversionFunction (arrayType)
    if arrayType:find ("@") ~= nil then
        return "Urho3D::VectorToHandleArray"
    else
        return "Urho3D::VectorToArray"
    end
end

function ConstructVarTemplateVars (var)
    local templateVars = {}
    templateVars ["    "] = _tab
    templateVars ["${asBindGenCommand}"] = _asBindGenCommand
    templateVars ["${className}"] = _className

    templateVars ["${var.type}"] = var.type
    templateVars ["${var.name}"] = var.name
    templateVars ["${var.default}"] = var.default
    templateVars ["${var.description}"] = var.description

    templateVars ["${var.shortName}"] = var.name:sub (1, var.name:len () - 1)
    templateVars ["${var.prettyName}"] = VarCodeNameToPrettyName (var.name)
    templateVars ["${var.setterArgType}"] = VarTypeToSetterArgType (var.type)
    templateVars ["${var.attributeDefault}"] = GetVarAttributeDefault (var.type, var.default)
    templateVars ["${var.registrationMacro}"] = GetRegistrationMacro (var.type)
    templateVars ["${var.bindingsType}"] = VarCodeTypeToBindingsVarType (var.type)

    if IsArrayType (var.type) then
        templateVars ["${var.arrayValueType}"] = GetArrayValueType (var.type)
        templateVars ["${var.arrayValueTypeWithoutPtr}"] = GetArrayValueType (var.type):gsub ("*", "")
        templateVars ["${var.arrayValueBindingsType}"] = VarCodeTypeToBindingsVarType (GetArrayValueType (var.type))
        templateVars ["${var.arrayConversionFunction}"] = GetArrayBindingsConversionFunction (var.type)
        templateVars ["${var.toArrayConversionFunction}"] = GetToArrayBindingsConversionFunction (var.type)
    end
    return templateVars
end

function ProcessTemplate (template, templateVars)
    for key, value in pairs (templateVars) do
        template = template:gsub (key, value)
    end
    return template
end
