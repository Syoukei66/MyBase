#include <stdio.h>

#include "ShaderCompiler.h"
#include "ShaderTest.h"

class TestCompiler : public HLSLCompiler
{
public:
  virtual void ConvertHLSL(const ShaderData& data, std::string* str) override
  {

  }
  virtual void CompileVertexShader(const std::string& str, std::vector<unsigned char>* dest) override
  {
    Log::Info(str.c_str());
  }
  virtual void CompilePixelShader(const std::string& str, std::vector<unsigned char>* dest) override
  {
    Log::Info(str.c_str());
  }
};

int main()
{
  TestCompiler test = TestCompiler();
  std::string str = FileUtil::TextFile_Read("Test.shader");
  ShaderData data = ShaderData();
  try
  {
    ShaderCompiler(str).Compile(&test, &data);
    printf(ShaderTest::ToString(data).c_str());
  }
  catch (ParseException e)
  {
    printf(e.message.c_str());
  }
  printf("\n終了するには何かキーを押してください");
  getchar();
  return 0;
}
