local c_gameobject = require("GameObject")

--This sets the prototype for the Lua player class.  Put static variables inside of it.  We will set all the variables in the new function
Player = {
    name = '',
    gameobject = {
        Id = function() end,
        Location = function() end,
    },

}

function NewPlayer(name)
    local player = {}
    player.name = name
    --Sets the metatable for the local player to reference the Player construct for lookups, which will have the functions and such
    setmetatable(player, Player)
    --Create the gameobject from C and attach it to this guy so that we can reference it from lua for future C function calls.
    player.gameobject = c_gameobject.New(10,33)
    return player
end

function Player:Start()
    print('Hello world from lua with the name ' .. self['name'] .. ' and the boi')
    print('The id is '..self.gameobject:Id())
    local x,y = self.gameobject:Location()
    print('Location is Y:'..y..' X: '..x)
end
