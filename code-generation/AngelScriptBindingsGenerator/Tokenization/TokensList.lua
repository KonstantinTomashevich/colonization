if CreateNewClass == nil then
    CreateNewClass = require (scriptDirectory .. "BindingTypes/CreateNewClass")
end

TokensList = CreateNewClass ()
TokensList.Construct = function (self, tokens)
    self.currentIndex = 1
    self.tokens = tokens
end

TokensList.PreviousToken = function (self)
    self.currentIndex = self.currentIndex - 1
    return self:CurrentToken ()
end

TokensList.CurrentToken = function (self)
    return self.tokens [self.currentIndex]
end

TokensList.NextToken = function (self)
    self.currentIndex = self.currentIndex + 1
    return self:CurrentToken ()
end
return TokensList
