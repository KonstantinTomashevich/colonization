FileUtils = {}
FileUtils.OpenFile = function (fileName)
    local file = io.open (fileName, "w+")
    if file == nil then
        local dir = fileName:match (".*/")
        while dir:sub (1, 1) == "." or dir:sub (1, 1) == "/" do
            dir = dir:sub (2, dir:len ())
        end

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
