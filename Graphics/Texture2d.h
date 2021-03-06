//----------------------------------------------------------------------------------------------------------------------
// Texture2d.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef HELIUM_GRAPHICS_TEXTURE_2D_H
#define HELIUM_GRAPHICS_TEXTURE_2D_H

#include "Graphics/Texture.h"

namespace Helium
{
    /// 2D texture resource.
    class HELIUM_GRAPHICS_API Texture2d : public Texture
    {
        HELIUM_DECLARE_OBJECT( Texture2d, Texture );

    public:
        /// @name Construction/Destruction
        //@{
        Texture2d();
        virtual ~Texture2d();
        //@}

        /// @name Serialization
        //@{
        virtual bool NeedsPrecacheResourceData() const;
        virtual bool BeginPrecacheResourceData();
        virtual bool TryFinishPrecacheResourceData();
        //@}

        /// @name Resource Serialization
        //@{
        virtual void SerializePersistentResourceData( Serializer& s );
        //@}

        /// @name Data Access
        //@{
        RTexture2d* GetRenderResource2d() const;
        //@}

    private:
        /// Async load IDs for cached texture data.
        DynamicArray< size_t > m_renderResourceLoadIds;
    };
}

#endif  // HELIUM_GRAPHICS_TEXTURE_2D_H
