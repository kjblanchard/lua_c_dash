local initialize = require "GameObject"
local aux,prv = {},{}
local GameObject = {}
prv = initialize(aux,prv)
local umeta = {}
--umeta.__gc = prv.del
local gameobjects = setmetatable({},{__mode = "v"})
GameObject.__index = GameObject

function GameObject:x()
    return prv.X()
end

function GameObject:location()
    return prv.Location()
end

function GameObject:start()
    return 0
end

function GameObject:ptr()
    return self.ptr
end

--global functions
function Create_gameobject(x, y, start)
    for key,value in pairs(prv) do
        print (key)
    end

    local udata, ptr = prv.Create(x,y)
    local gameobject = setmetatable({},GameObject)
    gameobject.__udata = udata
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

