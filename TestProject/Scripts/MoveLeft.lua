function update(entity, deltaTime)
    local x, y = GetPosition(entity)
    local speed = 100

    x = x - (speed * deltaTime)

    SetPosition(entity, x, y)
end