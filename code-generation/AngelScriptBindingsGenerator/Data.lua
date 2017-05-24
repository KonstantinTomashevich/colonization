data = {}
data.enums = {}
data.constants = {}
data.freeFunctions = {}
data.classes = {}

function GetBindingTypesOfFile (fileName)
    local fileBindingTypes = {}
    for key, value in pairs (data.enums) do
        if value.fileName == fileName then
            table.insert (fileBindingTypes, value)
        end
    end

    for key, value in pairs (data.constants) do
        if value.fileName == fileName then
            table.insert (fileBindingTypes, value)
        end
    end

    for key, value in pairs (data.freeFunctions) do
        if value.fileName == fileName then
            table.insert (fileBindingTypes, value)
        end
    end

    for key, value in pairs (data.classes) do
        if value.fileName == fileName then
            table.insert (fileBindingTypes, value)
        end
    end
    return fileBindingTypes
end
return data, GetBindingTypesOfFile
