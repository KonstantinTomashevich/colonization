require (scriptDirectory .. "Globals")

function ReadVar (readedLine)
    local varData = {}
    local index = 1
    local reading = ""
    local readProcess = "type"

    while index <= readedLine:len () do
        local char = readedLine:sub (index, index)
        local nextChar;
        if index + 1 <= readedLine:len () then
            nextChar = readedLine:sub (index + 1, index + 1)
        else
            nextChar = " "
        end

        local previousChar;
        if index > 1 then
            previousChar = readedLine:sub (index - 1, index - 1)
        else
            previousChar = " "
        end

        if readProcess == "type" then
            if char ~= " " or reading == "unsigned" or
                nextChar == "," or nextChar == "<" or nextChar == ">" or previousChar == "," then
                reading = reading .. char
            else
                varData ["type"] = reading
                reading = ""
                readProcess = "name"
            end

        elseif readProcess == "name" then
            if char ~= " " and char ~= "=" then
                reading = reading .. char
            elseif char == "=" then
                varData ["name"] = reading
                reading = ""
                readProcess = "default"
            end

        elseif readProcess == "default" then
            if char ~= "$" then
                reading = reading .. char
            else
                varData ["default"] = reading
                reading = ""
                readProcess = "description"
            end

        else
            reading = reading .. char
        end
        index = index + 1
    end

    varData ["description"] = reading
    table.insert (_vars, varData)

    print ("New var:\n    type: " .. varData ["type"] .. "\n" ..
        "    name: " .. varData ["name"] .. "\n" ..
        "    default: " .. varData ["default"] .. "\n" ..
        "    description: " .. varData ["description"])
end
