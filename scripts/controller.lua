local initialize_controller = require "Controller"
local aux,prv = {},{}
local Controller= {}
prv = initialize_controller(aux,prv)
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
---The different states the key can be in
Controller.KeyStates = {
    Pressed = 0,
    Held = 1,
    Released = 2
}
---The currently bound functions for each keypress type
Controller.BindFunctions = {
    [Controller.KeyStates.Pressed] = {},
    [Controller.KeyStates.Held] = {},
    [Controller.KeyStates.Released] = {},
}
---The base bind function, probably shouldn't be called directly
---@param keystate integer The KeyState that the button is in
---@param button integer The button being pressed
---@param func function the function to call
function Controller:bind_button(keystate, button, func)
    local button_table = self.BindFunctions[keystate][button]
    if button_table == nil then
        self.BindFunctions[keystate][button] = {}
    end
    table.insert(self.BindFunctions[keystate][button],func)
end
---Updates the controller.  This will get all of the controller events, and then call the binded functions for the button.  This should be called every frame
---@param lua_object table The lua object that is being updated
function Controller:update(lua_object)
    while true do
       local button, state = prv.HandleInput(self.userdata)
       if button == nil then
           break
       end
       local func = self.BindFunctions[state][button]
       if func ~= nil then
           for i,val in ipairs(func) do
               val(lua_object)
           end
       end
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
---Binds a function to a button when it is pressed or held.
---@param button integer A controller button from Controller.ButtonNumbers
---@param func function A function that is going to be called when the button is down or held
function Controller:bind_button_down(button, func)
    self:bind_button(Controller.KeyStates.Held, button, func )
    self:bind_button(Controller.KeyStates.Pressed, button, func )
end
---Binds a function when a button is pressed
---@param button integer A controller button from Controller.ButtonNumbers
---@param func function a function that is going to be called when the button is pressed
function Controller:bind_button_pressed(button, func)
    self:bind_button(Controller.KeyStates.Pressed, button, func )
end
---Binds a function when a button is released
---@param button integer A controller button from Controller.ButtonNumbers
---@param func function a function that is going to be called when the button is released
function Controller:bind_button_released(button, func)
    self:bind_button(Controller.KeyStates.Released, button, func )
end
---Binds a function when a button is held
---@param button integer A controller button from Controller.ButtonNumbers
---@param func function a function that is going to be called when the button is held
function Controller:bind_button_held(button, func)
    self:bind_button(Controller.KeyStates.Held, button, func )
end
-------------------------------------------------------------------------------------------
--Helper functions for lua objects
-------------------------------------------------------------------------------------------
function Controller:left_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Left,func) end
function Controller:up_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Up,func) end
function Controller:right_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Right,func) end
function Controller:down_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Down,func) end
function Controller:a_button_down(func) self:bind_button_down(Controller.ButtonNumbers.A,func) end
function Controller:b_button_down(func) self:bind_button_down(Controller.ButtonNumbers.B,func) end
function Controller:x_button_down(func) self:bind_button_down(Controller.ButtonNumbers.X,func) end
function Controller:y_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Y,func) end
function Controller:start_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Start,func) end
function Controller:select_button_down(func) self:bind_button_down(Controller.ButtonNumbers.Select,func) end
function Controller:left_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Left,func) end
function Controller:up_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Up,func) end
function Controller:right_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Right,func) end
function Controller:down_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Down,func) end
function Controller:a_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.A,func) end
function Controller:b_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.B,func) end
function Controller:x_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.X,func) end
function Controller:y_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Y,func) end
function Controller:start_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Start,func) end
function Controller:select_button_pressed(func) self:bind_button_pressed(Controller.ButtonNumbers.Select,func) end
function Controller:left_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Left,func) end
function Controller:up_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Up,func) end
function Controller:right_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Right,func) end
function Controller:down_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Down,func) end
function Controller:a_button_released(func) self:bind_button_released(Controller.ButtonNumbers.A,func) end
function Controller:b_button_released(func) self:bind_button_released(Controller.ButtonNumbers.B,func) end
function Controller:x_button_released(func) self:bind_button_released(Controller.ButtonNumbers.X,func) end
function Controller:y_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Y,func) end
function Controller:start_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Start,func) end
function Controller:select_button_released(func) self:bind_button_released(Controller.ButtonNumbers.Select,func) end
function Controller:left_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Left,func) end
function Controller:up_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Up,func) end
function Controller:right_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Right,func) end
function Controller:down_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Down,func) end
function Controller:a_button_held(func) self:bind_button_held(Controller.ButtonNumbers.A,func) end
function Controller:b_button_held(func) self:bind_button_held(Controller.ButtonNumbers.B,func) end
function Controller:x_button_held(func) self:bind_button_held(Controller.ButtonNumbers.X,func) end
function Controller:y_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Y,func) end
function Controller:start_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Start,func) end
function Controller:select_button_held(func) self:bind_button_held(Controller.ButtonNumbers.Select,func) end
--
--End Helper functions
--
return Controller
