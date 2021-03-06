//----------------------------------------------------------------------------------------------------------------------
// FrameworkWin.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef HELIUM_FRAMEWORK_WIN_FRAMEWORK_WIN_H
#define HELIUM_FRAMEWORK_WIN_FRAMEWORK_WIN_H

#include "Platform/System.h"

#if HELIUM_SHARED
    #ifdef HELIUM_FRAMEWORK_WIN_EXPORTS
        #define HELIUM_FRAMEWORK_WIN_API HELIUM_API_EXPORT
    #else
        #define HELIUM_FRAMEWORK_WIN_API HELIUM_API_IMPORT
    #endif
#else
    #define HELIUM_FRAMEWORK_WIN_API
#endif

#endif  // HELIUM_FRAMEWORK_WIN_FRAMEWORK_WIN_H
