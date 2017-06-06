require (scriptDirectory .. "Globals")

function ReadTab (readedLine)
    local index = readedLine:find ("\"") + 1
    _tab = ""
    while readedLine:sub (index, index) == " " do
        _tab = _tab ..  " "
        index = index + 1
    end
    Log ("Tab setted: \"" .. _tab .. "\".")
end

function ReadClassName (readedLine)
    local index = readedLine:find (":") + 1
    _className = ""
    while index <= readedLine:len () do
        local char = readedLine:sub (index, index)
        if char ~= " " then
            _className = _className .. char
        end
        index = index + 1
    end
    Log ("Class name setted: \"" .. _className .. "\".")
end
