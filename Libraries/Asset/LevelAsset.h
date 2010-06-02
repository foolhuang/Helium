#pragma once

#include "API.h"
#include "AssetClass.h"


#include "TUID/TUID.h"
#include "Reflect/Registry.h"

#include "Finder/AssetSpecs.h"
#include "Finder/ExtensionSpecs.h"

namespace Asset
{
  class ASSET_API LevelAsset : public AssetClass
  {
  public:
    float       m_NearClipDist;
    float       m_FarClipDist;
    float       m_SpatialGridSize;
    bool        m_OcclTestDownwardColl;
    bool        m_OcclTestDownwardVis;
    float       m_OcclVisDistAdjust;
    bool        m_Multiplayer;
    bool        m_AutoBuildDefaultRegion;
    bool        m_IncludeGlobalReqs;

    float       m_DecalGeomMem;

    Math::Vector3 m_ViewerStartingPosition;
    Math::Quaternion m_ViewerStartingRotation;

    bool        m_IsLevelFoliageHeavy;

    LevelAsset()
      : m_NearClipDist( 0.06f )
      , m_FarClipDist( 1800.0f )
      , m_SpatialGridSize( 2.0f )
      , m_IsLevelFoliageHeavy(false)
      , m_OcclTestDownwardColl( true )
      , m_OcclTestDownwardVis( true )
      , m_OcclVisDistAdjust( 0.0f )
      , m_Multiplayer( false )
      , m_AutoBuildDefaultRegion( true )
      , m_IncludeGlobalReqs( true )
      , m_DecalGeomMem( 1024.0f )
    {
    }

    virtual bool ProcessComponent(Reflect::ElementPtr element, const std::string& fieldName) NOC_OVERRIDE;

    virtual bool ValidateCompatible( const Attribute::AttributePtr& attr, std::string& error ) const NOC_OVERRIDE;

    virtual bool IsBuildable() const NOC_OVERRIDE;
    virtual bool IsViewable() const NOC_OVERRIDE;

    void MakeDefault();

    REFLECT_DECLARE_CLASS( LevelAsset, AssetClass );

    static void EnumerateClass( Reflect::Compositor<LevelAsset>& comp );
  };

  typedef Nocturnal::SmartPtr< LevelAsset > LevelAssetPtr;
  typedef std::vector< LevelAsset > V_LevelAsset;
}