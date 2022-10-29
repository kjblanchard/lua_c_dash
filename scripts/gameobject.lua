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

---Gets the X value of a gameobject.
---@return number X position
function GameObject:x()
    return prv.X(self.userdata)
end

---Adds to the current X position
---@param addition number Amount to move from the current X position.
function GameObject:add_x(addition)
    local current_x = prv.X(self.userdata)
    self:set_x(current_x + addition)
end

---Sets the X of a gameobject
---@param x number The new x position
function GameObject:set_x(x)
    prv.SetX(self.userdata,x)
end

---Gets the Y value of a gameobject
---@return number the Y value
function GameObject:y()
    return prv.Y(self.userdata)
end

---Sets the X value of a gameobject.
---@param y number the new y position
function GameObject:set_y(y)
    prv.SetY(self.userdata, y)
end

---Adds addition to the current y position
---@param addition number the amount to add
function GameObject:add_y(addition)
    local current_y = prv.Y(self.userdata)
    self:set_y(current_y + addition)
end

---Gets the current x and y position
---@return number the current x position
---@return number the current y position
function GameObject:location()
    return prv.Location()
end

---Sets the current location
---@param x number the new x position
---@param y number the new y position
function GameObject:set_location(x, y)
    local x_val = x or 0
    local y_val = y or 0
    prv.SetLocation(self.userdata, x_val,y_val)
end

---Adds to the gameobjects current location
---@param x_add number the x amount to add
---@param y_add number the y amount to add
function GameObject:add_location(x_add, y_add)
    local y = prv.Y(self.userdata)
    local x = prv.X(self.userdata)
    prv.SetLocation(self.userdata, x + x_add, y + y_add)
end

--The function that is called when a gameobject is started, this should be overrode if a gameobject needs to update.
function GameObject:start()
end

--The function that is called when a gameobject is updated every frame, this should be overrode if a gameobject needs to update.
function GameObject:update()
end

--global functions, like creating this object
---Creates a gameobject
---@param lua_obj table The lua object that is creating this, this is so that we can look this up in the table from C with just the pointer
---@param x number The x location of this gameobject
---@param y number The y location of this gameobject
---@param start function The start function for this gameobject, this is called when C starts the gameobject.
---@param update function The update function for this gameobject, this is called when C update  the gameobject ever frame
---@return table the created gameobject userdata
---@return any the pointer of this object, potentially not needed to be returned
function Create_gameobject(lua_obj,x, y, start, update)
    local gameobject  = {}
    setmetatable(gameobject,GameObject)
    local udata, ptr = prv.Create(x,y)
    gameobject.userdata = udata
    gameobjects[ptr] = lua_obj
    gameobject.start = start or GameObject.start
    gameobject.update = update or GameObject.update
    return gameobject, ptr
end

---Function that is meant to be called from C, which will start or update the gameobject.
---@param ptr userdata The C pointer for this, as we need to look it up.
function aux.start_gameobject(ptr)
    local gameobj = gameobjects[ptr]
    gameobj.gameobject.start(gameobj)
end

---Updates the gameobject
---@param ptr userdata The c pointer for this, as we need to look it up
function aux.update_gameobject(ptr)
    local gameobj = gameobjects[ptr]
    gameobj.gameobject.update(gameobj)
end


--Gameobject userdata metatable - Used by "create" c function, to add in the proper metatable if we want.
aux.metatable = umeta

return GameObject

