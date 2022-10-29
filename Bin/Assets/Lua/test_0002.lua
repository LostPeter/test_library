print "***** Hello World From Lua *****"
    
    c_SayHello();

    local angle = 1.7
    local sin1 = c_CalculateSin(angle)
    print (string.format("sin(%g) = %.2f", angle, sin1))

    angle = 3.14
    local sin2, cos2 = c_CalculateSinCos(angle)
    print (string.format("sin(%g) = %.2f    cos(%g) = %.2f", angle, sin2, angle, cos2))

print "***** Hello World From Lua *****"