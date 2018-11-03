-- test
print(string.format("Hello LUA(%s)",ExecuteFilename))
print("Count:",gTest.count)
print("Line2:",gTest.line[2],#gTest.line)
print("BeerType:",gTest.beer_type)
print("Child:",gTest.child.name,"Age:",gTest.child.age)
if gTest.beer_type == Test.BeerType.Larger
then
    print "is Larger"
    gTest.beer_type = Test.BeerType.Pilsner
else
    print "no Larger"
    gTest.beer_type = Test.BeerType.Larger
end
print("Note2:",gTest.note[2].page,"line=",gTest.note[2].line)
print "Top10:"
for r = 1,10 do
    print(r,"=",gTest.ranking[r])
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
print(Test.BeerType.Lambic)
for ai, arg in ipairs(args) do
    print(string.format("    arg[%d]: %s",ai,arg))
end
print "Good bye LUA"
