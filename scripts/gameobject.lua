local initialize = require "GameObject"
local aux,prv = {},{}
local GameObject = {
    userdata = {},
    ptr = {},
    start = function () end
}

prv = initialize(aux,prv)
local umeta = {}
--umeta.__gc = prv.del
local gameobjects = setmetatable({},{__mode = "v"})
GameObject.__index = GameObject

function GameObject:x()
    return prv.X(self.userdata)
end

function GameObject:location()
    return prv.Location()
end

function GameObject:start()
    return 0
end

--global functions
function Create_gameobject(x, y, start)
    print('here')
    local gameobject  = {}
    setmetatable(gameobject,GameObject)
    print('here2')
    local udata, ptr = prv.Create(x,y)
    print('here3')
    gameobject.userdata = udata
    gameobject.ptr = ptr
    gameobjects[ptr] = gameobject
    gameobject.start = start or GameObject.start
    return gameobject, ptr
end

function aux.start_gameobject(ptr)
    local gameobject = gameobjects[ptr]
    gameobject.start()
end


--Gameobject userdata metatable - Used by "create" c function
aux.metatable = umeta

return GameObject

