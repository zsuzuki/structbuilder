-- test
print(string.format("Hello LUA(%s)", ExecuteFilename))
if InitializedTest then
    print("Initialized Struct by JSON")
else
    print("Not Initialized Struct")
end
print(string.format("Count: %d", gTest.count))
if #args > 0 then
    print("Args:")
    for ai, arg in ipairs(args) do
        print(string.format("    arg[%d]: %s", ai, arg))
    end
else
    print("no args")
end
print("Player:", gTest.child.name, "Age:", gTest.child.age)
if gTest.beer_type == Test.BeerType.Larger then
    print(string.format("Beer is Larger"))
    gTest.beer_type = Test.BeerType.Pilsner
else
    print(string.format("Beer is no Larger(type=%d)", gTest.beer_type))
    gTest.beer_type = Test.BeerType.Larger
end
print(string.format("BeerType[Lambic] is %d", Test.BeerType.Lambic))
print(string.format("Line: %d", #gTest.line))
for _, l in ipairs(gTest.line) do
    print(string.format("  %0.3f", l))
end
print("Note:")
for ni, n in ipairs(gTest.note) do
    print(string.format("  note[%d]: %3d,%2d", ni, n.page, n.line))
end
local pl_num = #gTest.entry_list
print(string.format("Top%d:", pl_num))
for r = 1, pl_num do
    print(string.format("  %2d = %d", r, gTest.ranking[r]))
end
-- local e = TestEntry.new()
-- e.name = "K. Yamada"
-- e.country = "JP"
-- table.insert(gTest.entry_list,e)
print(string.format("EntryList: %d players", #gTest.entry_list))
for ei, ev in ipairs(gTest.entry_list) do
    local m = string.format("    [%i]: %-16s(%s): %3d pts,%d wins", ei, ev.name, ev.country, ev.point, ev.wins)
    print(m)
end
test = Test.new()
test.copy(gTest)
if test == gTest then
    print(string.format("Eq %d/%d", test.child.age, gTest.child.age))
else
    print(string.format("Ne %d/%d", test.child.age, gTest.child.age))
end
test.child.age = test.child.age - 1
if test == gTest then
    print(string.format("Eq %d/%d", test.child.age, gTest.child.age))
else
    print(string.format("Ne %d/%d", test.child.age, gTest.child.age))
end
print "Good bye LUA"
