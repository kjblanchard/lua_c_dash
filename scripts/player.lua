local gameobject = require("gameobject")
local controller = require("controller")

--This sets the prototype for the Lua player class.  Put static variables inside of it.  We will set all the variables in the new function
Player = {
    name = '',
    speed = 1,
}



--Creates a new player object
function NewPlayer(name)
    local player = Player
    player.name = name
    --Sets the metatable for the local player to reference the Player construct for lookups, which will have the functions and such
    setmetatable(player, Player)
    local ptr
    --Create the gameobject from C and attach it to this guy so that we can reference it from lua for future C function calls.
    player.gameobject, ptr = Create_gameobject(player, 10, 30, Player.start, Player.update)
    player.controller = CreateController(player)
    --player.gameobject.x(player.gameobject)
    player.gameobject:x()
    --Return the gameobject pointer to C, so that we can use it.
    return ptr
end

function Player:start()
    print('Starting player with name '..self.name)
    print(self.controller)
    self.controller:bind_button(0, 4, Player.move_left)
    self.controller:bind_button(1, 4, Player.move_left)
    self.controller:bind_button(0, 1, Player.move_up)
    self.controller:bind_button(1, 1, Player.move_up)
    self.controller:bind_button(0, 2, Player.move_right)
    self.controller:bind_button(1, 2, Player.move_right)
    self.controller:bind_button(0, 3, Player.move_down)
    self.controller:bind_button(1, 3, Player.move_down)
end

function Player:move_left()
    self.gameobject.add_x(self.gameobject, -self.speed)
end
function Player:move_right()
    self.gameobject.add_x(self.gameobject, self.speed)
end
function Player:move_up()
    self.gameobject.add_y(self.gameobject, -self.speed)
end
function Player:move_down()
    self.gameobject.add_y(self.gameobject, self.speed)
end




function Player:update()
    self.controller:update(self)
end


