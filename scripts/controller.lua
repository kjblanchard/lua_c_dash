local initialize = require "Controller"
local aux,prv = {},{}
local Controller= {}
prv = initialize(aux,prv)
--Userdata metatable.  Could be used to set the C userdata stuff in lua if we want.
local umeta = {}
--umeta.__gc = prv.del
--Use the GameObject to look things up.
Controller.__index = Controller

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

function Controller:ButtonPressed(button_number)
    local pressed = prv.CheckIfButtonPressed(self.userdata, button_number)
    return pressed
end

function Controller:ButtonReleased(button_number)
    local pressed = prv.CheckIfButtonReleased(self.userdata, button_number)
    return pressed
end

function Controller:ButtonHeld(button_number)
    local pressed = prv.CheckIfButtonHeld(self.userdata, button_number)
    return pressed
end

--global functions, like creating this object
function CreateController(lua_obj)
    --TODO add in components in gameobject, which you would just add it in here.
    local controller  = {}
    setmetatable(controller,Controller)
    local udata = prv.CreateController()
    controller.userdata = udata
    return controller
end

return Controller
