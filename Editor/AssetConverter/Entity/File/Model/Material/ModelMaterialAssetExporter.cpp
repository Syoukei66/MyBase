#include "ModelMaterialAssetExporter.h"

// =================================================================
// Methods
// =================================================================
void ModelMaterialAssetExporter::ExportProcess(ModelMaterialAssetEntity* entity, const AssetConverterContext* context)
{
  CerealIO::Binary::Export(entity->GetAssetInfo()->GetOutputPath().c_str(), entity->GetData());
  Logger::ConvertAssetLog(entity->GetAssetInfo());
}
