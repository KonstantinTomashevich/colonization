TokenUtils = {}

TokenUtils.TokenTypeToString = function (token)
    if token.type == Tokens.Operator then
        return "Operator"
    elseif token.type == Tokens.TypeOrName then
        return "Type Or Name"
    elseif token.type == Tokens.Command then
        return "Command"
    elseif token.type == Tokens.EndOfLine then
        return "End Of Line"
    elseif token.type == Tokens.EndOfFile then
        return "End Of File"
    else
        return "Unknown Token"
    end
end

TokenUtils.TokenToString = function (token)
    local tokenValue = token.value
    if tokenValue == nil then
        tokenValue = "nil"
    end
    return "\"" .. tokenValue .. "\"(" .. TokenUtils.TokenTypeToString (token) .. ")"
end
return TokenUtils
