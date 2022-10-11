function Start ()
    print('Hello world from lua')
    GameObject()
end

function Update (x)
    print('Look, Im in the update from lua with ', x, 'And then it is cool boi')
end

function Draw (x, y)
    return (x^2 * math.sin(y)) / (1-x)
end
