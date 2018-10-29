#pragma once

#include <Core/MaterialData.h>
#include "AssetEntity.h"

class DefaultMaterialAssetEntity : public AssetEntity
{
  ENTITY_ID(ID_DEFAULT_MATERIAL)
    // =================================================================
    // Constructor / Destructor
    // =================================================================
public:
  DefaultMaterialAssetEntity(AssetInfo* info, MaterialData* data);
  ~DefaultMaterialAssetEntity();

  // =================================================================
  // Setter / Getter
  // =================================================================
public:
  GG_INLINE const MaterialData* GetData() const
  {
    return this->data_;
  }

  // =================================================================
  // Data Members
  // =================================================================
private:
  MaterialData* data_;

};
