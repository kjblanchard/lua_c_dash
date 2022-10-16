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

InitialMoveSpeed = 5
MoveSpeed = 1

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
    player.gameobject = c_gameobject.New(60,5, player)
    return player.gameobject
end

function Player:Start()
    local x,y = self.gameobject:Location()
    print('Lua Start function.. Location Y: '..y..' X: '..x)
end

function Player:Update()
    if self.gameobject:ButtonPressed(ControllerButtons.Right) then
        Player.MoveX(self,InitialMoveSpeed)
    elseif self.gameobject:ButtonHeld(ControllerButtons.Right) then
        Player.MoveX(self,MoveSpeed)
    end
    if self.gameobject:ButtonPressed(ControllerButtons.Up) then
        Player.MoveY(self,-InitialMoveSpeed)
    elseif self.gameobject:ButtonHeld(ControllerButtons.Up) then
        Player.MoveY(self,-MoveSpeed)
    end
    if self.gameobject:ButtonPressed(ControllerButtons.Down) then
        Player.MoveY(self,InitialMoveSpeed)
    elseif self.gameobject:ButtonHeld(ControllerButtons.Down) then
        Player.MoveY(self,MoveSpeed)
    end
    if self.gameobject:ButtonPressed(ControllerButtons.Left) then
        Player.MoveX(self,-InitialMoveSpeed)
    elseif self.gameobject:ButtonHeld(ControllerButtons.Left) then
        Player.MoveX(self,-MoveSpeed)
    end

end

function Player:MoveX(move)
        local new_x = self.gameobject:X() + move
        self.gameobject:SetX(new_x)
end

function Player:MoveY(move)
        local new_y = self.gameobject:Y() + move
        self.gameobject:SetY(new_y)
end

