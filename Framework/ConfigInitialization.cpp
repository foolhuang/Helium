//----------------------------------------------------------------------------------------------------------------------
// ConfigInitialization.cpp
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#include "FrameworkPch.h"
#include "Framework/ConfigInitialization.h"

#include "Engine/Config.h"
#include "Engine/GameObjectLoader.h"

using namespace Helium;

/// Destructor.
ConfigInitialization::~ConfigInitialization()
{
}

/// Initialize application configuration settings.
///
/// @return  True if initialization was successful, false if not.
bool ConfigInitialization::Initialize()
{
    Config& rConfig = Config::GetStaticInstance();

    GameObjectLoader* pObjectLoader = GameObjectLoader::GetStaticInstance();
    HELIUM_ASSERT( pObjectLoader );

    HELIUM_TRACE( TraceLevels::Info, TXT( "Loading configuration settings.\n" ) );

    rConfig.BeginLoad();
    while( !rConfig.TryFinishLoad() )
    {
        pObjectLoader->Tick();
    }

    HELIUM_TRACE( TraceLevels::Debug, TXT( "Configuration settings loaded.\n" ) );

    return true;
}
