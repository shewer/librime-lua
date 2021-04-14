#! /usr/bin/env lua
--
-- table_translator.lua
-- Copyright (C) 2021 Shewer Lu <shewer@gmail.com>
--
-- Distributed under terms of the MIT license.
--



-- local p= Projection() --  new obj
-- p:load( ConfigList )  -- load from ConfigList 
-- p:append(string)      -- append from  string



local function test_code(env)
	print("==== test TranslatorOptions ")
	print("----tran: ",env.translator,env.translator.lookup)
	print("----tran: ",env.translator,env.translator.memory.dict)
--	print("----tran: ",env.translator,env.translator.memory.dict.lookup_words)
	print("tag: " ,env.translator.option.tag)
	print("enable_completion: " ,env.translator.option.enable_completion )
	env.translator.option.enable_completion=false
end 

local function wildword(str, ch, wildch,tab)
	tab = tab or {}
	local pat=  ("([%s]*)([%s])(.*)"):format(wildch,ch) 
	local lstr,cc ,rstr= str:match(pat)
	-- print( ("pat:  --%s-- lstr:--%s-- ch:--%s-- rstr: --%s-- wildch: --%s-- tab %s"):format(pat,lstr,cc,rstr,wildch,tab))
	if not lstr then 
		--print("insert: " ,str )
		table.insert(tab,str)
		return
	end 
	for i =1,#wildch do 
		wildword( lstr .. wildch:sub(i,i) .. rstr , ch, wildch , tab)
	end 
	--print("table size:", #tab)
	return #tab >0 and tab  or  nil
end 

local function init_func(env)
	--  create Ticket
	local ticket=Ticket(env.engine, env.name_space) -- (engine , path)
	assert(ticket, ticket) 
	print( "load TableTran : cagjie5, namespace :", env.name_space )
	-- create TableTranslator obj
	env.translator= TableTranslator( ticket ) 
	env.translator.option.tag="abc"
	env.translator.option.enable_completion=true
	test_code(env)--  
	env.wild="abcdefghijklmnopqrstuvwxyz;"
	env.wildch="?"
end 

local function func(input,seg,env)
    --   lookup(input, seg.start,seg_end,limit,enable_user_dict)

	local pj2= env.translator.option.preedit_formatter  -- get Projection obj
	assert(pj2,pj2)
	env.engine.context.preedit= pj2:apply(input)
    --print("pj2 tet ---->") ;print( pj2:apply(input) )
	local inp_tab=wildword(input,env.wildch,env.wild)
	--print("---->>>input:",  input, pj2:apply(input),"inp_tab", inp_tab  )
	if inp_tab then 
		--print("---in wildword : tran ", tran )
		local tab={}
		for _,inp in next , inp_tab do 
			local tran=env.translator:lookup(inp,seg.start,seg._end,10,true)
			if tran then 
				for cand in tran:iter() do 
					if utf8.len(cand.text) < 2 then 
					   cand.comment= pj2:apply(inp) 
					   table.insert(tab,cand)
				   end 
				end 
			end
		end 
		table.sort(tab, function(a,b) return a.quality > b.quality end )
		print("cand count:" , #tab ) 
		for _,cand in next, tab do 
			yield(cand)
		end 
	else
		print("---in normal : tran " )
		local tran=env.translator:query(input,seg)
		print("----", tran)
		for cand in tran:iter() do 
			cand.comment= pj2:apply(input) 
			yield(cand)
		end 
	end 
end


return { init=init_func, func=func } 
---   rime.lua    
--   table_tran= require("table_translator")()
--
--
--   schema.yaml
--   insert to /engine/translators
--   lua_translator@tab_tran
--

