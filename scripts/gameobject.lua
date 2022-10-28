local initialize = require "GameObject"
--Aux is the C "auxiliary" library, where we call these from C
local aux,prv = {},{}
local GameObject = {
    userdata = {},
    ptr = {},
    start = function () end
}

prv = initialize(aux,prv)
--Userdata metatable.  Could be used to set the C userdata stuff in lua if we want.
local umeta = {}
--umeta.__gc = prv.del
local gameobjects = setmetatable({},{__mode = "v"})
--Use the GameObject to look things up.
GameObject.__index = GameObject

--Gets the X value of a gameobject.
function GameObject:x()
    local x = prv.X(self.userdata)
    return x
end

function GameObject:add_x(addition)
    local current_x = prv.X(self.userdata)
    self:set_x(current_x + addition)
end

--Sets the X value of a gameobject.
function GameObject:set_x(x)
    prv.SetX(self.userdata,x)
end

--Gets the Y value of a gameobject
function GameObject:y()
    local y = prv.Y(self.userdata)
    return y
end

--Sets the X value of a gameobject.
function GameObject:set_y(y)
    prv.SetY(self.userdata, y)
end

function GameObject:add_y(addition)
    local current_y = prv.Y(self.userdata)
    self:set_y(current_y + addition)
end

function GameObject:location()
    local x,y =  prv.Location()
    return x,y
end

function GameObject:set_location(x,y)
    local x_val = x or 0
    local y_val = y or 0
    prv.SetLocation(self.userdata, x_val,y_val)
end

--The function that is called when a gameobject is started, this should be overrode if a gameobject needs to update.
function GameObject:start()
end

--The function that is called when a gameobject is updated every frame, this should be overrode if a gameobject needs to update.
function GameObject:update()
end

--global functions, like creating this object
function Create_gameobject(lua_obj,x, y, start, update)
    local gameobject  = {}
    setmetatable(gameobject,GameObject)
    local udata, ptr = prv.Create(x,y)
    gameobject.userdata = udata
    gameobject.ptr = ptr
    gameobjects[ptr] = lua_obj
    gameobject.start = start or GameObject.start
    gameobject.update = update or GameObject.update
    return gameobject, ptr
end

--Function that is meant to be called from C, which will start or update the gameobject.
function aux.start_gameobject(ptr)
    local gameobj = gameobjects[ptr]
    gameobj.gameobject.start(gameobj)
end

function aux.update_gameobject(ptr)
    local gameobj = gameobjects[ptr]
    gameobj.gameobject.update(gameobj)
end


--Gameobject userdata metatable - Used by "create" c function, to add in the proper metatable if we want.
aux.metatable = umeta

return GameObject

