#include "StaticModelAssetConverterFactory.h"

#include "StaticModelAssetEntity.h"
#include "StaticModelAssetImporter.h"
#include "StaticModelAssetExporter.h"
#include <ProgramGenerator/AssetProgramGenerator.h>
#include <Converter/AssetConverter.h>

// =================================================================
// Methods
// =================================================================
IAssetConverter* StaticModelAssetConverterFactory::Create(AssetConverterContext* context) const
{
  StaticModelAssetImporter* importer = new StaticModelAssetImporter(this->target_extensions_);
  StaticModelAssetExporter* exporter = new StaticModelAssetExporter();
  AssetProgramGenerator<StaticModelAssetEntity>* program_generator = new AssetProgramGenerator<StaticModelAssetEntity>(1, 1);
  program_generator->AddAsset("StaticModel", "rcStaticModel");
  return new AssetConverter<StaticModelAssetEntity>(importer, exporter, program_generator);
}
