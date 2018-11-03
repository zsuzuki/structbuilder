-- test
print(string.format("Hello LUA(%s)",ExecuteFilename))
print("Count:",gTest.count)
print("Player:",gTest.child.name,"Age:",gTest.child.age)
print("BeerType:",gTest.beer_type)
if gTest.beer_type == Test.BeerType.Larger
then
    print "is Larger"
    gTest.beer_type = Test.BeerType.Pilsner
else
    print "no Larger"
    gTest.beer_type = Test.BeerType.Larger
end
print(string.format("BeerType[Lambic] is %d",Test.BeerType.Lambic))
print("Line:",#gTest.line)
for li, l in ipairs(gTest.line) do
    print(string.format("  %0.3f",l))
end
print("Note:")
for ni, n in ipairs(gTest.note) do
    print(string.format("  note[%d]: %3d,%2d",ni,n.page,n.line))
end
local pl_num = #gTest.entry_list
print(string.format("Top%d:",pl_num))
for r = 1,pl_num do
    print(string.format("  %2d = %d",r,gTest.ranking[r]))
end
-- local e = TestEntry.new()
-- e.name = "K. Yamada"
-- e.country = "JP"
-- table.insert(gTest.entry_list,e)
print("EntryList:",#gTest.entry_list)
for ei, ev in ipairs(gTest.entry_list) do
    local m = string.format("    [%i]: %s(%s): %d pts,%d wins",ei,ev.name,ev.country,ev.point,ev.wins)
    print(m)
end
print("Args:")
for ai, arg in ipairs(args) do
    print(string.format("    arg[%d]: %s",ai,arg))
end
print "Good bye LUA"
