#! /usr/bin/env lua
--
-- lua_tran.lua
-- Copyright (C) 2022 Shewer Lu <shewer@gmail.com>
--
-- Distributed under terms of the MIT license.
--
-- 重覆再利用 rime engine 各組件
--
-- 如 translator 受限於 speller 只能接受 ascii ，可以利用 translator 組件 產生 translation 再處理
--
-- 聯想輸入法 -- 
-- 1. 製作 中文 - 中文 字典 
-- 2. 以 lua_translator@translator 替代主字典
--
local M={}

function M.init(env)
  -- 主字典
  env.main_tran = Component.Translator(env.engine, "translator", "table_translator") -- namespace translator
  -- 聯想字典
  env.second_tran = Componont.Translator(env.engine, "translator", "table_translator@second_tran") -- namespace second_tran
end
function M.fini(env)
end


function M.func(input,seg,env)
  local active_input = input:sub(seg.starn +1 , seg._end )
  local tn=env.main_tran:query(input,seg)
  local tab = {}
  local save_cand
  -- 主字典
  for cand in  tn:iter() do 
    if cand.type ~= 'completion' then 
      yield(cand)
      table.insert(cand.text)
    else 
      save_cand = cand
      break
    end
  end
  -- 中文字查中文字  -- completion: true
  -- 聯想詞組
  for text in ipairs(tab) do 
    for cand in  env.second_tran:query(input,seg):iter() do 
      yield(cand)
    end
  end
  -- 主字典 completion cand 最後送出
  yield(save_cand)
  for cand in tn:iter() do 
    yield(cand)
  end
end


    
return M  


