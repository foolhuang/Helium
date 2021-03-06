//----------------------------------------------------------------------------------------------------------------------
// RendererUtil.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef HELIUM_RENDERING_RENDERER_UTIL_H
#define HELIUM_RENDERING_RENDERER_UTIL_H

#include "Rendering/RendererTypes.h"

namespace Helium
{
    /// Renderer utility functions.
    class HELIUM_RENDERING_API RendererUtil
    {
    public:
        /// @name Pixel Format Support
        //@{
        static bool IsSrgbPixelFormat( ERendererPixelFormat format );
        static uint32_t PixelToBlockRowCount( uint32_t pixelRowCount, ERendererPixelFormat format );
        //@}

        /// @name Primitive Math
        //@{
        static uint32_t PrimitiveCountToIndexCount( ERendererPrimitiveType primitiveType, uint32_t primitiveCount );
        static uint32_t IndexCountToPrimitiveCount( ERendererPrimitiveType primitiveType, uint32_t indexCount );
        //@}
    };
}

#endif  // HELIUM_RENDERING_RENDERER_UTIL_H
