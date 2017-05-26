data = {}
data.enums = {}
data.constants = {}
data.freeFunctions = {}
data.classes = {}
data.subsystems = {}
data.filesToWriteList = {}

function GetBindablesOfFile (fileName)
    local fileBindingTypes = {}
    local toCheck = {"enums", "constants", "freeFunctions", "classes"}
    for index, toCheckItem in ipairs (toCheck) do
        for key, value in pairs (data [toCheckItem]) do
            if value.fileName == fileName then
                table.insert (fileBindingTypes, value)
            end
        end
    end
    return fileBindingTypes
end
return data, GetBindingTypesOfFile
