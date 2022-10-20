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
    local x = prv.X(self.userdata)
    print("X value in lua is "..x)
    return x
end

function GameObject:location()
    return prv.Location()
end

function GameObject:start()
    return 0
end

--global functions
function Create_gameobject(x, y, start,lua_obj)
    local gameobject  = {}
    setmetatable(gameobject,GameObject)
    local udata, ptr = prv.Create(x,y)
    gameobject.userdata = udata
    gameobject.ptr = ptr
    gameobjects[ptr] = lua_obj
    gameobject.start = start or GameObject.start
    return gameobject, ptr
end

function aux.start_gameobject(ptr)
    local gameobj = gameobjects[ptr]
    gameobj.gameobject.start(gameobj)
end


--Gameobject userdata metatable - Used by "create" c function
aux.metatable = umeta

return GameObject

