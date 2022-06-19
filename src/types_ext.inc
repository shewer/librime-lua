/*
 * types_ext.cc
 * Copyright (C) 2022 Shewer Lu <shewer@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef _TYPE_EXT_INC
#define _TYPE_EXT_INC 
#include <rime/common.h>
using namespace rime;
#include <rime/dict/level_db.h> 

namespace ProcessorReg{ 
  typedef Processor T;
  static const luaL_Reg funcs[] = {
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"press_key_event",WRAPMEM(T::ProcessKeyEvent)},
    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };

}
namespace SegmentorReg{

  typedef Segmentor T;

  static const luaL_Reg funcs[] = {
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"proceed",WRAPMEM(T::Proceed)},
    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };
}
namespace TranslatorReg{

  typedef Translator T;

  static const luaL_Reg funcs[] = {
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"query",WRAPMEM(T::Query)},
    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };
}
namespace FilterReg{

  typedef Filter T;
  an<Translation> apply(an<T> f, an<Translation> t){
    return f->Apply( t, nullptr );
  }

  static const luaL_Reg funcs[] = {
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"apply",WRAP(apply)},
    {"applies_to_segment",WRAPMEM(T::AppliesToSegment)},
    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };
}

// leveldb 
namespace DbAccessorReg{
  typedef DbAccessor T;

  // return key , value or nil
  int raw_peek(lua_State* L){
    int n = lua_gettop(L);
    if (1 > n ) 
      return 0;
    auto a = LuaType<an<T>>::todata(L, 1);
    string key,value;
    if (a->GetNextRecord(&key,&value)) {
      LuaType<string>::pushdata(L,key);
      LuaType<string>::pushdata(L,value);
      return 2;
    }else {
      return 0;
    }
  };

  // return raw_peek, self
  // for key,value in peek, self do print(key,value) end
  int raw_iter(lua_State* L){
    int n = lua_gettop(L);
    if (1 > n)
      return 0;
    lua_pushcfunction(L, raw_peek);
    lua_insert(L, 1);
    //lua_rotate(L, 2, 1);
    lua_settop(L, 2);
    return 2;
  }

  static const luaL_Reg funcs[] = {
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"reset", WRAPMEM(T::Reset)},
    {"jump", WRAPMEM(T::Jump)},
    {"exhanusted", WRAPMEM(T::exhausted)},
    {"peek",raw_peek},
    {"iter",raw_iter},
    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };
}
namespace LevelDbReg{
  typedef LevelDb T;
  typedef DbAccessor A;

  // 
  an<T> make(const string& file_name, const string& db_name, const string& db_type){
    return New<LevelDb>(file_name,db_name,db_type);
  }
  string fetch(an<T> t, const string& key){
    string res;
    return ( t->Fetch(key,&res) )? res : nullptr ; 
  }

  static const luaL_Reg funcs[] = {
    {"LevelDb", WRAP(make)},
    { NULL, NULL },
  };

  static const luaL_Reg methods[] = {
    {"open", WRAPMEM(T::Open)},
    {"open_read_only", WRAPMEM(T::OpenReadOnly)},
    {"close", WRAPMEM(T::Close)},
    {"query", WRAPMEM(T::Query)},
    {"fetch", WRAP(fetch)},
    {"update", WRAPMEM(T::Update)},
    {"erase", WRAPMEM(T::Erase)},

    { NULL, NULL },
  };

  static const luaL_Reg vars_get[] = {
    { NULL, NULL },
  };

  static const luaL_Reg vars_set[] = {
    { NULL, NULL },
  };
}

template <typename O>
an<O> create(Ticket & ticket){
  if (auto c = O::Require(ticket.klass)) {
    return (an<O>) c->Create(ticket);
  }
  else {
    LOG(ERROR) << "error creating " << typeid(O).name() << ": '" << ticket.klass << "'";
    return {};
  }
}

namespace ComponentReg{

  typedef Processor P;
  typedef Segmentor S;
  typedef Translator T;
  typedef Filter F;

  an<P> create_processor(Engine *e, string ns, string prescription){
    Ticket t = Ticket(e, ns, prescription);
    return create<P>(t);
  }
  an<S> create_segmentor(Engine *e, string ns, string prescription){
    Ticket t = Ticket(e, ns, prescription);
    return create<S>(t);
  }
  an<T> create_translator(Engine *e, string ns, string prescription){
    Ticket t = Ticket(e, ns, prescription);
    return create<T>(t);
  }
  an<F> create_filter(Engine *e, string ns, string prescription){
    Ticket t = Ticket(e, ns, prescription);
    return create<F>(t);
  }
  

  static const luaL_Reg funcs[] = {
    {"Processor",  WRAP(create_processor)},
    {"Segment"   , WRAP(create_segmentor)},
    {"Translator", WRAP(create_translator)},
    {"Filter", WRAP(create_filter)},
    { NULL, NULL },
  };

  void init(lua_State *L) {
    lua_createtable(L, 0, 0);
    luaL_setfuncs(L, funcs, 0);
    lua_setglobal(L, "Component");
  }
}

#define EXPORT_TYPES_EXT(L) \
{\
  EXPORT(ProcessorReg, L);\
  EXPORT(SegmentorReg, L);\
  EXPORT(TranslatorReg, L);\
  EXPORT(FilterReg, L);\
  EXPORT(DbAccessorReg, L);\
  EXPORT(LevelDbReg, L);\
  ComponentReg::init(L);\
}

#endif /* ifndef _TYPE_EXT_INC */


