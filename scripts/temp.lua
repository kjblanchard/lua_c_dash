local c_gameobject = require("GameObject")

ControllerButtons = {
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
    player.gameobject = c_gameobject.New(60,5, player)
    return player.gameobject
end

function Player:Update()

end

function Player:Start()
    print('Hello world from lua with the name ' .. self['name'] .. ' and the boi')
    print('The id is '..self.gameobject:Id())
    local x,y = self.gameobject:Location()
    print('Location is Y:'..y..' X: '..x)
end
