/*
 * lua_ext_gears.h
 * Copyright (C) 2022 Shewer Lu <shewer@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LUA_EXT_GEARS_H
#define LUA_EXT_GEARS_H
#include <rime/common.h>
#include <rime/ticket.h>
// LuaMemory
#include <rime/gear/memory.h>
#include <rime/dict/dictionary.h>
#include <rime/dict/user_dictionary.h>
// LuaTableTranslator
#include <rime/gear/table_translator.h>
#include <rime/gear/unity_table_encoder.h> //table_translator

// LuaScriptTranslator
#include <rime/gear/script_translator.h>
#include <rime/dict/corrector.h>   // StriptTranslator
#include <rime/gear/poet.h>

//#include <rime/gear/script_translator.h>
//#include <rime/dict/corrector.h>   // StriptTranslator
//#include <rime/gear/table_translator.h>
//#include <rime/gear/unity_table_encoder.h> //table_translator
//#include <rime/gear/poet.h>


namespace rime {
  class LuaMemory : public Memory {
    an<LuaObj> memorize_callback;
    Lua *lua_;
    public:
    using Memory::Memory;
    DictEntryIterator iter;
    UserDictEntryIterator uter;

    LuaMemory(Lua *lua, const Ticket& ticket)
      : lua_(lua), Memory(ticket) {}

    virtual bool Memorize(const CommitEntry&);

    void memorize(an<LuaObj> func) {
      memorize_callback = func;
    }

    void clearDict() {
      iter = DictEntryIterator();
    }
    void clearUser() {
      uter = UserDictEntryIterator();
    }
  };

//START_ATTR_
//sed  sed -n -e'/\/\/START_ATTR_/,/\/\/END_ATTR_/p' src/lua_ext_gears.h | gcc -E -
#define ATTR_(v_name, type) \
  GET_(v_name, type);\
  SET_(v_name, type)
#define GET_(v_name, type) \
  type v_name() { return v_name ## _ ;}
#define SET_(v_name, type) \
  void set_ ## v_name(const type v) { v_name ## _ = v ;}

  class LuaTableTranslator : public TableTranslator {
    public:
      LuaTableTranslator(const Ticket &ticket) : TableTranslator(ticket) {};

      ATTR_(enable_charset_filter, bool);
      ATTR_(enable_encoder, bool);
      ATTR_(enable_sentence, bool);
      ATTR_(sentence_over_completion, bool);
      ATTR_(encode_commit_history, bool);
      ATTR_(max_phrase_length, int);
      ATTR_(max_homographs, int);
  };

  class LuaScriptTranslator : public ScriptTranslator {
    public:
      LuaScriptTranslator(const Ticket &ticket) : ScriptTranslator(ticket) {};

      ATTR_(max_homophones, int);
      ATTR_(spelling_hints, int);
      ATTR_(always_show_comments, bool);
      ATTR_(enable_correction, bool);
  };
}
#undef ATTR_
#undef GET_
#undef SET_
//END_ATTR_
#endif /* !LUA_EXT_GEARS_H */
