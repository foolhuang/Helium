//----------------------------------------------------------------------------------------------------------------------
// NullRendererInitialization.cpp
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#include "FrameworkPch.h"
#include "Framework/NullRendererInitialization.h"

using namespace Helium;

/// @copydoc RendererInitialization::Initialize()
bool NullRendererInitialization::Initialize()
{
    // No Renderer instance is created, so simply return that we have been successful.
    return true;
}
