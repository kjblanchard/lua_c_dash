local gameobject = require("gameobject")

--This sets the prototype for the Lua player class.  Put static variables inside of it.  We will set all the variables in the new function
Player = {
    name = '',
}

function NewPlayer(name)
    local player = {}
    player.name = name
    --Sets the metatable for the local player to reference the Player construct for lookups, which will have the functions and such
    setmetatable(player, Player)
    --Create the gameobject from C and attach it to this guy so that we can reference it from lua for future C function calls.
    player.gameobject, ptr = Create_gameobject(10,30,Player.Start)
    --player.gameobject.x(player.gameobject)
    player.gameobject:x()

    --Return the gameobject pointer to C, so that we can use it.
    return ptr
end

function Player:Start()
    gameobject_x = self:x()
    print('Start x is '..gameobject_x..' and the name is '..self.name)
end


