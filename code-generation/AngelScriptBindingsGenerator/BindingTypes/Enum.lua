if Class == nil then
    Class = require (scriptDirectory .. "ElementsTypes/Class")
end

Enum = Class ()
Enum.Construct = function (self, fileName, bindingAguments)
    self.fileName = fileName
    self.name = ""
    self.values = {}
    self.braceOpened = false
    self.arguments = bindingAguments

    self.enumWordReaded = false
    self.nameReaded = false
end

-- Process file line, returns true if binding processing finished.
Enum.ProcessFileLine = function (self, line, lineIndex)
    local commentPosition = line:find ("//")
    if commentPosition == nil then
        commentPosition = 999999
    end

    if not self.enumWordReaded then
        local enumPosition = line:find ("enum")
        if enumPosition ~= nil and enumPosition < commentPosition then
            self.enumWordReaded = true
            local readNamePosition = enumPosition + ("enum "):len ()
            local lastReadPosition = self:ReadName (line, readNamePosition, commentPosition)

            if self.enumWordReaded and self.nameReaded and self.braceOpened then
                return self:ReadLineAfterInitialization (line, lineIndex, lastReadPosition, commentPosition)
            else
                return false
            end
        end

    elseif self.enumWordReaded and not self.nameReaded then
        local readStartPosition = 1
        local char = line:sub (readStartPosition, readStartPosition)
        while char == " " and readStartPosition < commentPosition and readStartPosition < line:len () do
            readStartPosition = readStartPosition + 1
            char = line:sub (readStartPosition, readStartPosition)
        end

        local lastReadPosition = 1
        if readStartPosition < commentPosition and readStartPosition < line:len () then
            lastReadPosition = self:ReadName (line, readStartPosition, commentPosition)
        end

        if self.enumWordReaded and self.nameReaded and self.braceOpened then
            return self:ReadLineAfterInitialization (line, lineIndex, lastReadPosition, commentPosition)
        else
            return false
        end

    elseif self.braceOpened and (not self.enumWordReaded or not self.nameReaded) then
        print ("    Line " .. lineIndex .. ": Can't read Enum, \"{\" reached, but name not readed!")
        return true

    elseif self.enumWordReaded and self.nameReaded then
        return self:ReadLineAfterInitialization (line, lineIndex, 1, commentPosition)
    end
end

Enum.ReadName = function (self, line, readStartPosition, commentPosition)
    local readPosition = readStartPosition
    if readPosition < line:len () and (commentPosition == nil or readPosition < commentPosition) then
        local char = line:sub (readPosition, readPosition)
        while char ~= " " and char ~= "\n" and char ~= "{" and
            readPosition <= line:len () and readPosition < commentPosition do

            self.name = self.name .. char
            readPosition = readPosition + 1
            char = line:sub (readPosition, readPosition)
        end

        self.nameReaded = true
        while readPosition <= line:len () and readPosition < commentPosition do
            if char == "{" then
                self.braceOpened = true
                return readPosition
            end

            readPosition = readPosition + 1
            char = line:sub (readPosition, readPosition)
        end
        return readPosition
    end
end

Enum.ReadLineAfterInitialization = function (self, line, lineIndex, readStartPosition, commentPosition)
    local readingIndex = readStartPosition
    local char = line:sub (readingIndex, readingIndex)

    if not self.braceOpened then
        while char ~= "{" and readingIndex < commentPosition and readingIndex <= line:len () do
            char = line:sub (readingIndex, readingIndex)
            readingIndex = readingIndex + 1
        end

        if char == "{" then
            self.braceOpened = true
        else
            return false
        end
    end

    local currentlyReadingName = ""
    local readingName = true
    while readingIndex < commentPosition and readingIndex <= line:len () do
        char = line:sub (readingIndex, readingIndex)

        if readingName then
            if char ~= " " and char ~= "," and char ~= "=" and char ~= "{" and char ~= "}" then
                currentlyReadingName = currentlyReadingName .. char

            elseif char == "," or (currentlyReadingName == "" and char ~= "}") then
                if currentlyReadingName ~= "" then
                    table.insert (self.values, currentlyReadingName)
                end

                currentlyReadingName = ""
                readingName = true

            elseif char == "{" then
                print ("    Line " .. lineIndex .. ": Can't read Enum, second \"{\" reached!")
                return true

            elseif char == "}" then
                if currentlyReadingName ~= "" then
                    table.insert (self.values, currentlyReadingName)
                end
                return true

            else
                if currentlyReadingName ~= "" then
                    table.insert (self.values, currentlyReadingName)
                end

                currentlyReadingName = ""
                readingName = false
            end
        else
            if char == "," then
                readingName = true
            elseif char == "{" then
                print ("    Line " .. lineIndex .. ": Can't read Enum, second \"{\" reached!")
                return true

            elseif char == "}" then
                return true
            end
        end
        readingIndex = readingIndex + 1
    end

    if currentlyReadingName ~= "" then
        table.insert (self.values, currentlyReadingName)
    end
    return false
end

Enum.GetDataDestination = function ()
    return "enums"
end

Enum.GetTypeName = function ()
    return "Enum"
end

return Enum;
