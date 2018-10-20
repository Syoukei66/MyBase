#include "CSVData.h"
#include <fstream>

// =================================================================
// Factory Method
// =================================================================
UniqueResource<rcCsvData> rcCsvData::CreateFromFile(const char* path)
{
  rcCsvData* ret = new rcCsvData(path);
  ret->Resource::Init();
  return UniqueResource<rcCsvData>(ret);
}

// =================================================================
// Constructor / Destructor
// =================================================================
rcCsvData::rcCsvData(const char* path)
{
  std::ifstream ifs(path);
  std::string str;
  while (getline(ifs, str))
  {
    const char& head = str[0];
    if (head == '/' || head == '\n')
    {
      continue;
    }
    CsvTokenizer* tokenizer = new CsvTokenizer(str);
    if (tokenizer->GetSize() == 0)
    {
      delete tokenizer;
      continue;
    }
    this->tokenizers_.emplace_back(tokenizer);
  }
  NATIVE_ASSERT(this->tokenizers_.size() > 0, "CSVファイルの読み込みに失敗しました");
}

rcCsvData::~rcCsvData()
{
  for (CsvTokenizer* tokenizer : this->tokenizers_)
  {
    delete tokenizer;
  }
}
