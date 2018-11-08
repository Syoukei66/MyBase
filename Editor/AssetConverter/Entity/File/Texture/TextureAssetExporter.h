#pragma once

#include <Entity/AssetExporter.h>
#include "TextureAssetEntity.h"

class TextureAssetExporter : public AssetExporter<TextureAssetEntity>
{
  // =================================================================
  // Methods
  // =================================================================
protected:
  void ExportProcess(TextureAssetEntity* entity, const AssetConverterContext* context) override;

};
