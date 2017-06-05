FileUtils = {}
FileUtils.RemoveBeginPointsInPath = function (path)
    local newPath = path
    while newPath:sub (1, 1) == "." or newPath:sub (1, 1) == "/" do
        newPath = newPath:sub (2, newPath:len ())
    end
    return newPath
end

FileUtils.OpenFile = function (fileName)
    local file = io.open (fileName, "w+")
    if file == nil then
        local dir = fileName:match (".*/")
        dir = FileUtils.RemoveBeginPointsInPath (dir)

        if os.execute ("mkdir " .. dir) == 1 then
            dir = dir:gsub ("/", "\\")
            os.execute ("mkdir " .. dir)
        end

        file = io.open (fileName, "w+")
        return file
    else
        return file
    end
end
return FileUtils
