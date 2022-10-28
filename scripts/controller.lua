local initialize = require "Controller"
local aux,prv = {},{}
local Controller= {}
prv = initialize(aux,prv)
--Userdata metatable.  Could be used to set the C userdata stuff in lua if we want.
local umeta = {}
--umeta.__gc = prv.del
--Use the GameObject to look things up.
Controller.__index = Controller

--The button numbers mapped to a C enum, so that we can easily reference them.
Controller.ButtonNumbers = {
    Up = 1,
    Right = 2,
    Down = 3,
    Left = 4,
    Y = 5,
    B = 6,
    A = 7,
    X = 8,
    Start = 9,
    Select = 10
}

---Checks to see if a button is pressed
---@param button_number integer integer corresponding to a button pressed
---@return boolean if the button is pressed
function Controller:ButtonPressed(button_number)
    return prv.CheckIfButtonPressed(self.userdata, button_number)
end

---Checks to see if a button is released
---@param button_number integer integer corresponding to a button pressed
---@return boolean if the button is held
function Controller:ButtonReleased(button_number)
    return prv.CheckIfButtonReleased(self.userdata, button_number)
end

---Checks to see if a button is held
---@param button_number integer Int corresponding to a c enum to button pressed
---@return boolean if the button is held
function Controller:ButtonHeld(button_number)
    return prv.CheckIfButtonHeld(self.userdata, button_number)
end

---Checks to see if a button is held or pressed
---@param button_number integer The button to check for
---@return boolean if the button is held or pressed
function Controller:ButtonDown(button_number)
    local pressed = self:ButtonPressed(button_number)
    if pressed
        then
        return pressed
    end
    return self:ButtonHeld(button_number)
end

--Create a controller userdata, should be used inside of a lua object so that it can be properly deleted
function CreateController(lua_obj)
    --TODO add in components in gameobject, which you would just add it in here.
    local controller  = {}
    setmetatable(controller,Controller)
    local udata = prv.CreateController()
    controller.userdata = udata
    return controller
end

return Controller
