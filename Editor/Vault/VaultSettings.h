#pragma once

#include "Foundation/Container/OrderedSet.h"

#include "SceneGraph/SettingsManager.h"

#include "Editor/Vault/VaultMenuIDs.h"
#include "Editor/Vault/VaultSearchQuery.h"
#include "Editor/Settings/WindowSettings.h"

namespace Helium
{
    namespace Editor
    {
        class VaultSettings : public Settings
        {
        public:
            REFLECT_DECLARE_OBJECT( VaultSettings, Reflect::Object );
            static void PopulateComposite( Reflect::Composite& comp );

            VaultSettings( VaultViewMode viewVaultMode = VaultViewMode::Details, uint32_t thumbnailSize = VaultThumbnailsSizes::Medium );
            ~VaultSettings();

        public:
            VaultViewMode         m_VaultViewMode;
            uint32_t                   m_ThumbnailSize;
        };

        typedef Helium::SmartPtr< VaultSettings > VaultSettingsPtr;
    }
}