#include "ModuleEntity.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

#define NOMINMAX
#include <Windows.h>

#include <ThirdParty/Cereal/cereal.hpp>
#include <ThirdParty/Cereal/archives/binary.hpp>
#include <ThirdParty/Cereal/types/string.hpp>
#include <ThirdParty/Cereal/types/vector.hpp>

static const std::string ENGINE_DIRECTORY_PATH = "Engine";
static const std::string INCLUDE_HEADER = "Include.h";
static const std::string INCLUDE_CACHE = "cache.IncludeBuilderCache";
static const std::string STDAFX_HEADER = "stdafx.h";

template <typename T>
bool SafeImport(const char* path, T* dest)
{
  using namespace std;
  ifstream ifs(path, ios::in | ios::binary);
  if (!ifs.is_open())
  {
    return false;
  }
  ::cereal::BinaryInputArchive i_archive(ifs);
  try
  {
    i_archive(*dest);
    return true;
  }
  catch (cereal::Exception e)
  {
  }
  return false;
}

template <typename T>
void Export(const char* path, const T* data)
{
  using namespace std;
  ofstream ofs(path, ios::out | ios::binary | ios::trunc);
  ::cereal::BinaryOutputArchive o_archive(ofs);
  o_archive(*data);
}

ModuleEntity::ModuleEntity(const std::string& path)
  : path_(path)
{
  this->AddIgnore(INCLUDE_HEADER);
  this->AddIgnore(STDAFX_HEADER);
}

void ModuleEntity::CreateHeaderProgram() const
{
  std::vector<std::string> targets;

  targets.push_back("/stdafx.h");

  this->Crawl("", &targets);

  std::string CACHE_PATH = ENGINE_DIRECTORY_PATH + "/" + this->path_ + "/" + INCLUDE_CACHE;
  std::string HEADER_PATH = ENGINE_DIRECTORY_PATH + "/" + this->path_ + "/" + INCLUDE_HEADER;

  bool need_build = false;

  // 無駄なヘッダービルドが行われないように事前にビルドしたヘッダーを記録しておく
  std::vector<std::string> cache = std::vector<std::string>();
  if (SafeImport(CACHE_PATH.c_str(), &cache))
  {
    if (targets.size() != cache.size())
    {
      need_build = true;
    }
    else
    {
      for (std::string target : targets)
      {
        bool builded = false;
        for (std::string cached_target : cache)
        {
          if (target == cached_target)
          {
            builded = true;
            break;
          }
        }
        if (!builded)
        {
          need_build = true;
          break;
        }
      }
    }
    if (!need_build)
    {
      if (!std::ifstream(HEADER_PATH))
      {
        need_build = true;
      }
    }
  }

  Export(CACHE_PATH.c_str(), &targets);

  if (!need_build)
  {
    return;
  }

  std::string header;
  header.append("#pragma once\n");
  header.append("// auto generated by IncludeBuilder\n");
  header.append("\n");
  if (targets.size() > 0)
  {
    for (const std::string& path : targets)
    {
      header.append("#include <" + this->path_ + path + ">\n");
    }
    header.append("\n");
  }
  if (this->specials_.size() > 0)
  {
    for (const std::string& path : this->specials_)
    {
      header.append("#include <" + path + ">\n");
    }
    header.append("\n");
  }
  std::ofstream ofs(HEADER_PATH);
  ofs << header;
  ofs.close();

  std::cout << "//==================================================" << std::endl;
  std::cout << "// " << this->path_ << "/Include.h" << std::endl;
  std::cout << "//==================================================" << std::endl;
  std::cout << header;
}

void ModuleEntity::Crawl(const std::string& path, std::vector<std::string>* dest) const
{
  HANDLE handle;
  WIN32_FIND_DATA data;

  std::string find_file = ENGINE_DIRECTORY_PATH + "/" + this->path_ + path + "/*";
  handle = FindFirstFile(find_file.c_str(), &data);
  if (handle == INVALID_HANDLE_VALUE)
  {
    std::cout << "error Directory not found" << std::endl;
    getchar();
    return;
  }

  //directory_pathで指定されたディレクトリ内のすべてのファイル/ディレクトリに対し処理を行う
  do
  {
    //親ディレクトリを無視
    if (
      (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
      (strcmp(data.cFileName, "..") == 0 || strcmp(data.cFileName, ".") == 0)
      )
    {
      continue;
    }

    std::string file_name = data.cFileName;

    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      Crawl(path + "/" + data.cFileName, dest);
      continue;
    }

    std::regex re("(.*[^\\.]+)\\.([^\\.]+$)");
    std::smatch result;
    if (!std::regex_match(file_name, result, re))
    {
      continue;
    }

    if (result[2] != "h")
    {
      continue;
    }

    if (this->ignores_.find(file_name) != this->ignores_.end())
    {
      continue;
    }

    (*dest).push_back(path + "/" + file_name);
  } while (FindNextFile(handle, &data));

  FindClose(handle);
}
