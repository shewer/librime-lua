#! /usr/bin/env lua
--
-- leveldb.lua
-- Copyright (C) 2022 Shewer Lu <shewer@gmail.com>
--
-- Distributed under terms of the MIT license.
--
-- lua_translator@english
--
--每個 leveldb 不能重覆開啓 ，所以在同一方案中如有共用 同一個db 設定在全域中
-- leveldb 初始化請在offline 狀態下，利用leveldb工具建立 如 luarocks lua-leveldb 
-- 此範例是將 ecdict.csv --> <word>\t<csvline>  導入 leveldb  key: word   value: csvline
--
-- shell 
-----------------------------------------
-- >luarock install lua-leveldb --local
--
-----------------------------------------
-- import.lua  
-----------------------------------------
--[[
leveldb = require 'lualeveldb'
local opt = leveldb.options()
local db = leveldb.open(opt, 'ecdict') 
for line in io.open('ecdict.txt') do 
  local word, data = line:match("^(.*)\t(.*)$) 
  db:put(word,data)
end
db:close()
--]]
-----------------------------------------
--
--
-- 開啓 關閉 要橙檢查 開啓狀態
--
-- 要調用大資料庫可行用 leveldb 可降低內存使用量
-- key value  字串格式自由度 比opencc優 
--
-- rime.lua 
_levedb={}

function create_leveldb( filename) 
  _leveldb[filename] = _leveldb[filename] or LevelDb(filename, 'ecdict','dict')
  return _leveldb[filename]
end
--- 
local M ={}
function M.init(env)

  local filename= "ecdict"
  env.levedb= create_leveldb(filename) 
  if not env.leveldb.loaded then 
    env:open_read_only()
  end

end
function M.fini(env)
  if env.leveldb.loaded then
    env.ldb:close()
  end
end

function M.func(input,seq,env)
  if not seg.tags:has("english") then return end
  --  queyr( prefix_str):iter() 
  for k,v in env.ldb:query(input):iter() do 
    yield( Candidate('english', seg.start, seg._end, k, v ) )
  end
end

return M
