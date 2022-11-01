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
    print('Keystate is '..keystate.. "button is "..button)
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

return Controller
