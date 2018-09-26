#include "Logger.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "AssetInfo.h"
#include "FileUtil.h"

static void FileNameLog(const std::string& path, T_INT32 w)
{
  std::cout << std::left << std::setw(w) << path.substr(std::max(0, (T_INT32)path.length() - w), std::min((size_t)w, path.length()));
}

static void ExportAssetLog(const AssetInfo* info, const std::string& action)
{
  std::cout << std::left << std::setw(10) << action + ":";
  FileNameLog(info->GetURI().GetFullPath(), 60);
  std::cout << " => ";
  std::cout << FileUtil::CreateOutputPath(info->GetURI().GetFullPath()) << std::endl;
}

void Logger::SkipAssetLog(const AssetInfo* info)
{
  ExportAssetLog(info, "skip");
}

void Logger::ConvertAssetLog(const AssetInfo* info)
{
  ExportAssetLog(info, "convert");
}

void Logger::CopyAssetLog(const AssetInfo* info)
{  
  ExportAssetLog(info, "copy");
}
