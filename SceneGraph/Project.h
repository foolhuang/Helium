#pragma once

#include "Foundation/Attribute.h"
#include "Application/DocumentManager.h"

#include "Reflect/Structure.h"
#include "Reflect/Data/DataDeduction.h"

#include "SceneGraph/API.h"

namespace Helium
{
    class HELIUM_SCENE_GRAPH_API Project : public Reflect::Object
    {
    public:
        Project( const FilePath& path = TXT( "" ) );
        virtual ~Project();

        void ConnectDocument( Document* document );
        void DisconnectDocument( const Document* document );

        // Document and DocumentManager Events
        void OnDocumentOpened( const DocumentEventArgs& args );
        void OnDocumenClosed( const DocumentEventArgs& args );

        bool Serialize() const;

        const std::set< FilePath >& Paths()
        {
            return m_Paths;
        }

        FilePath GetTrackerDB() const;

        void AddPath( const FilePath& path );
        void RemovePath( const FilePath& path );

    public:
        Helium::Attribute< FilePath >    a_Path;
        Helium::Event< const FilePath& > e_PathAdded;
        Helium::Event< const FilePath& > e_PathRemoved;

        mutable DocumentObjectChangedSignature::Event e_HasChanged;

    protected:
        std::set< FilePath > m_Paths;

        void OnDocumentSave( const DocumentEventArgs& args );
        void OnDocumentPathChanged( const DocumentPathChangedArgs& args );
        void OnChildDocumentPathChanged( const DocumentPathChangedArgs& args );

    public:
        REFLECT_DECLARE_OBJECT( Project, Reflect::Object );
        static void PopulateComposite( Reflect::Composite& comp );
    };

    typedef Helium::StrongPtr<Project> ProjectPtr;
}