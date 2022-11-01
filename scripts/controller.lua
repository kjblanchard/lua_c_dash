local initialize_controller = require "Controller"
local aux,prv = {},{}
local Controller= {}
prv = initialize_controller(aux,prv)
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

Controller.KeyStates = {
    Pressed = 0,
    Held = 1,
    Released = 2
}

Controller.BindFunctions = {
    [Controller.ButtonNumbers.Up] = {},
    [Controller.ButtonNumbers.Right] = {},
    [Controller.ButtonNumbers.Down] = {},
    [Controller.ButtonNumbers.Left] = {},
    [Controller.ButtonNumbers.Y] = {},
    [Controller.ButtonNumbers.B] = {},
    [Controller.ButtonNumbers.A] = {},
    [Controller.ButtonNumbers.X] = {},
    [Controller.ButtonNumbers.Start] = {},
    [Controller.ButtonNumbers.Select] = {},
}

function Controller:update(lua_object)
    while true do
       local button, state = prv.HandleInput(self.userdata)
       if button == nil then
           print('Button is nil! should break')
           break
       end
       print('Button is '..button..' and state is '..state)
       self.BindFunctions[4](lua_object)


    end
end

---Calls into C to release the controller data, this should be called before this object is destroyed fully.
function Controller:destroy()
    prv.DestroyController(self.userdata)

end

---Create controller userdata, so that we can use it in lua and C
---@param lua_obj table The lua object that we are attaching this to (should be a gameobject)
---@return table a new controller table.
function CreateController(lua_obj)
    --TODO add in components in gameobject, which you would just add it in here.
    local controller  = {}
    setmetatable(controller,Controller)
    local udata = prv.CreateController()
    controller.userdata = udata
    return controller
end

return Controller
