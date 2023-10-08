/*
 * opencc.h
 * Copyright (C) 2023 Shewer Lu <shewer@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef OPENCC_H
#define OPENCC_H

#include <opencc/Exception.hpp>
#include <opencc/Config.hpp> // Place OpenCC #includes here to avoid VS2015 compilation errors
#include <opencc/Converter.hpp>
#include <opencc/Conversion.hpp>
#include <opencc/ConversionChain.hpp>
#include <opencc/Dict.hpp>
#include <opencc/DictEntry.hpp>
#include <opencc/Common.hpp>

#if _MSC_VER
  // macro CONV( string of utf8) u use
  #if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
     #include <filesystem>
     using namespace ns = std::filesystem;
  #else
     #include <boost/filesystem.hpp>
     using namespace ns = boost::filesystem;
  #endif// __cplusplus
  #define CONV(utf8str) (opencc::UTF8UTIL::U16ToU8(ns::path(utf8str).wstring()))
#else
  #define CONV(utf8str) (utf8str)
#endif// _MSC_VER

#endif /* !OPENCC_H */
