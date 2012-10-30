#pragma once

#include "SceneGraph/PivotTransform.h"

namespace Helium
{
    namespace SceneGraph
    {
        class Instance;

        class HELIUM_SCENE_GRAPH_API Instance HELIUM_ABSTRACT : public PivotTransform
        {
        public:
            REFLECT_DECLARE_ABSTRACT( Instance, PivotTransform );
            static void PopulateComposite( Reflect::Composite& comp );
            static void InitializeType();
            static void CleanupType();

            Instance();
            ~Instance();

        protected:
            bool  m_Solid;
            bool  m_SolidOverride;
            bool  m_Transparent;
            bool  m_TransparentOverride;
        };

        typedef std::vector< Instance* > V_InstanceDumbPtr;
    }
}