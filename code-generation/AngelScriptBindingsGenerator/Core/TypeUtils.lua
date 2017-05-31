TypeUtils = {}
TypeUtils.BasicCXXToASType = function (type)
    return type:gsub ("*", "@"):gsub ("unsigned ", "u"):gsub ("long", "int64")
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
return TypeUtils
