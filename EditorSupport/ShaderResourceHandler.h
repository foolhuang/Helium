//----------------------------------------------------------------------------------------------------------------------
// ShaderResourceHandler.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef HELIUM_EDITOR_SUPPORT_SHADER_RESOURCE_HANDLER_H
#define HELIUM_EDITOR_SUPPORT_SHADER_RESOURCE_HANDLER_H

#include "EditorSupport/EditorSupport.h"

#if HELIUM_TOOLS

#include "PcSupport/ResourceHandler.h"

#include "Graphics/Shader.h"

namespace Helium
{
    /// Resource handler for Shader resource types.
    class HELIUM_EDITOR_SUPPORT_API ShaderResourceHandler : public ResourceHandler
    {
        HELIUM_DECLARE_OBJECT( ShaderResourceHandler, ResourceHandler );

    public:
        /// @name Construction/Destruction
        //@{
        ShaderResourceHandler();
        virtual ~ShaderResourceHandler();
        //@}

        /// @name Resource Handling Support
        //@{
        virtual const GameObjectType* GetResourceType() const;
        virtual void GetSourceExtensions( const tchar_t* const*& rppExtensions, size_t& rExtensionCount ) const;

        virtual bool CacheResource(
            ObjectPreprocessor* pObjectPreprocessor, Resource* pResource, const String& rSourceFilePath );
        //@}

    private:
        /// @name Private Utility Functions
        //@{
        void ParseLine(
            GameObjectPath shaderPath, Shader::PersistentResourceData& rResourceData, const char* pLineStart,
            const char* pLineEnd );
        template< typename OptionType > bool ParseLineDuplicateOptionCheck(
            Name optionName, const DynamicArray< OptionType >& rOptions );
        //@}
    };
}

#endif  // HELIUM_TOOLS

#endif  // HELIUM_EDITOR_SUPPORT_SHADER_RESOURCE_HANDLER_H
