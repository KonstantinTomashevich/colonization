FileUtils = {}
FileUtils.RemoveBeginPointsInPath = function (path)
    local newPath = path
    while newPath:sub (1, 1) == "." or newPath:sub (1, 1) == "/" do
        newPath = newPath:sub (2, newPath:len ())
    end
    return newPath
end

FileUtils.IsWindowsCmd = function ()
    return (os.execute ("cls") == 0)
end

FileUtils.MakePath = function (path)
    local chunkTemplate = ""
    if FileUtils.IsWindowsCmd () then
        chunkTemplate = "(cd ${dir} || mkdir ${dir} && cd ${dir})"
    else
        chunkTemplate = "cd ${dir} || mkdir ${dir} && cd ${dir}"
    end

    local command = ""
    local isFirst = true
    for dir in path:gmatch ("%a+/") do
        if isFirst then
            isFirst = false
        else
            command = command .. " && "
        end
        command = command .. chunkTemplate:gsub ("${dir}", dir:sub (1, dir:len () - 1))
    end
    os.execute (command)
end

FileUtils.CreateNewFile = function (fileName)
    local file = io.open (fileName, "w+")
    if file == nil then
        local dir = FileUtils.RemoveBeginPointsInPath (fileName:match (".*/"))
        FileUtils.MakePath (dir)
        file = io.open (fileName, "w+")
    end
    return file
end
return FileUtils
