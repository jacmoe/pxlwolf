print("Script.lua loaded!")

RandomStuff()

local Value = GetRandomNumber(123)
print(Value)

local struct1 = MyStruct:new()
local struct2 = MyStruct:new(1, 2)

print(struct1.x .. " x " .. struct1.y)
print(struct2.x .. " x " .. struct2.y)

struct1.x = 12
struct1.y = 14

print(struct1.x .. " x " .. struct1.y)

struct1:Print()
struct1:SetX(100)
struct1:Print()

local x = struct1:GetX()
print("X = " .. x)

DoStuffToStruct(struct2)
struct2:Print()

function LuaFunction()
    print("LuaFunction() called")
end

function LuaFunction2(x)
    print(x)
    return x
end

function LuaFunction3(myStruct)
    myStruct.x = 111
    myStruct.y = 222
end
