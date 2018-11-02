-- test
print "Hello LUA"
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
print("EntryList:",#gTest.entry_list)
for ei, ev in ipairs(gTest.entry_list) do
    local m = string.format( "    [%i]: %s(%s)",ei,ev.name,ev.country)
    print(m)
end
-- local e = TestEntry.new()
-- e.name = "B. Glow"
-- e.country = "UK"
-- table.insert(gTest.entry_list,e)
print(args[1])
print(Test.BeerType.Lambic)
print "Good bye LUA"
