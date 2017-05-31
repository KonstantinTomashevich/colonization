TypeUtils = {}
TypeUtils.BasicCXXToASType = function (type)
    local newType = type:gsub ("*", "@"):gsub ("unsigned ", "u"):gsub ("long", "int64"):
            gsub ("Urho3D::Vector ", "Array "):gsub ("Urho3D::Vector<", "Array<"):
            gsub ("Urho3D::PODVector ", "Array "):gsub ("Urho3D::PODVector<", "Array<")
    if newType:find ("Array ") ~= nil or newType:find ("Array<") ~= nil then
        newType = newType .. " @"
    end
    return newType
end

TypeUtils.CheckParsedClassesNames = function (type)
    for index, parsedClass in ipairs (data.classes) do
        if parsedClass.name ~= parsedClass.bindingName then
            type = type:gsub (parsedClass.name, parsedClass.bindingName)
        end
    end
    return type
end

TypeUtils.RemoveNamespaces = function (type)
    local newType = type
    local index = newType:find ("::")

    while index ~= nil do
        local toRemove = "::"
        local nextScanIndex = index - 1

        if nextScanIndex ~= 0 then
            local char = newType:sub (nextScanIndex, nextScanIndex)
            while char ~= " " and char ~= "," and char ~= "<" and char ~= ">" and
                char ~= "@" and char ~= "+" and char ~= "*" and char ~= "&" and nextScanIndex > 0 do
                toRemove = char .. toRemove
                nextScanIndex = nextScanIndex - 1
                char = newType:sub (nextScanIndex, nextScanIndex)
            end
        end
        newType = newType:gsub (toRemove, "")
        index = newType:find ("::")
    end

    return newType
end

TypeUtils.ConvertCXXToASType = function (type)
    return TypeUtils.RemoveNamespaces (TypeUtils.CheckParsedClassesNames (TypeUtils.BasicCXXToASType (type)))
end

TypeUtils.IsCXXArray = function (type)
    return type:find ("Vector ") ~= nil or type:find ("Vector<") ~= nil
end

TypeUtils.ASArrayTypeInCXX = function (type)
    return "Urho3D::CScriptArray *"
end

TypeUtils.GetArrayShortType = function (type)
    local typeWithoutNamespaces = TypeUtils.RemoveNamespaces (type):gsub ("const ", "")
    local index = 1
    local shortType = ""
    local char = ""

    while char ~= "<" and char ~= " " and index <= typeWithoutNamespaces:len () do
        shortType = shortType .. char
        char = typeWithoutNamespaces:sub (index, index)
        index = index + 1
    end
    return shortType
end

TypeUtils.GetArrayElementType = function (type)
    local index = type:find ("<") + 1
    local elementType = ""
    local char = ""

    while char ~= ">" and index <= type:len () do
        elementType = elementType .. char
        char = type:sub (index, index)
        index = index + 1
    end
    return elementType
end
return TypeUtils
