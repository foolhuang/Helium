#include "FileInterpreter.h"
#include "FileDialogButton.h"
#include "FileBrowserButton.h"

#include "Application/Inspect/InspectInit.h"
#include "Application/Inspect/Data/StringData.h"
#include "Application/Inspect/Script.h"
#include "Application/Inspect/Widgets/Button Controls/Action.h"
#include "Application/Inspect/Widgets/Text Controls/Value.h"
#include "Application/Inspect/DragDrop/ClipboardDataObject.h"
#include "Application/Inspect/DragDrop/ClipboardFileList.h"

#include "Pipeline/Asset/AssetFlags.h"
#include "Application/UI/FileDialog.h"
#include "Foundation/Log.h"
#include "Foundation/String/Wildcard.h"
#include "Foundation/String/Tokenize.h"

// Using
using namespace Reflect;
using namespace Inspect;

FileInterpreter::FileInterpreter (Container* container)
: ReflectFieldInterpreter (container)
, m_FileFilter( "" )
{

}

void FileInterpreter::InterpretField(const Field* field, const std::vector<Reflect::Element*>& instances, Container* parent)
{
    if (field->m_Flags & FieldFlags::Hide)
    {
        return;
    }

    //
    // Create the ui we are generating
    //

    std::vector< ContainerPtr > groups;

    ContainerPtr group = m_Container->GetCanvas()->Create<Container>(this);
    groups.push_back( group );

    bool pathField = field->m_SerializerID == Reflect::GetType< PointerSerializer >() && field->m_Flags & FieldFlags::Path;
    bool readOnly = ( field->m_Flags & FieldFlags::ReadOnly ) == FieldFlags::ReadOnly;

    DataChangingSignature::Delegate changingDelegate;

    FileDialogButtonPtr fileDialogButton;
    FileBrowserButtonPtr browserButton;

    //
    // Parse
    //
    std::string fieldUI;
    field->GetProperty( "UIScript", fieldUI );
    bool result = Script::Parse(fieldUI, this, parent->GetCanvas(), group, field->m_Flags);

    if (!result)
    {
        if ( pathField || field->m_SerializerID == Reflect::GetType<StringSerializer>() )
        {
            ContainerPtr valueGroup = m_Container->GetCanvas()->Create<Container>(this);
            ValuePtr value = m_Container->GetCanvas()->Create<Value>(this);
            value->SetJustification( Value::kRight );
            value->SetReadOnly( readOnly );
            valueGroup->AddControl( value );
            groups.push_back( valueGroup );

            if ( pathField || field->m_Flags & FieldFlags::FilePath ) 
            {
                if ( !readOnly )
                {
                    changingDelegate = DataChangingSignature::Delegate(this, &FileInterpreter::DataChanging);

                    // File dialog button
                    fileDialogButton = m_Container->GetCanvas()->Create<FileDialogButton>(this);

                    field->GetProperty( "FileFilter", m_FileFilter );

                    if ( !m_FileFilter.empty() )
                    {
                        fileDialogButton->SetFilter( m_FileFilter );
                    }
                    group->AddControl( fileDialogButton );

                    // File search button
                    browserButton = m_Container->GetCanvas()->Create<FileBrowserButton>(this);
                    if ( !m_FileFilter.empty() )
                    {
                        browserButton->SetFilter( m_FileFilter );
                    }
                    group->AddControl( browserButton );

                    Inspect::FilteredDropTarget* filteredDropTarget = new Inspect::FilteredDropTarget( m_FileFilter );
                    filteredDropTarget->AddDroppedListener( Inspect::FilteredDropTargetSignature::Delegate( this, &FileInterpreter::OnDrop ) );

                    m_Value = value;
                    m_Value->SetDropTarget( filteredDropTarget );
                }

                if ( instances.size() == 1 )
                {
                    // File edit button
                    ActionPtr editButton = m_Container->GetCanvas()->Create<Action>(this);
                    editButton->AddListener( ActionSignature::Delegate ( this, &FileInterpreter::Edit ) );
                    editButton->SetText("Edit");
                    group->AddControl( editButton );
                }

                if ( field->m_Flags & Asset::AssetFlags::RealTimeUpdateable )
                {
                    // File refresh button
                    ButtonPtr refreshButton = m_Container->GetCanvas()->Create<Button>(this);
                    refreshButton->SetText("Reload");
                    group->AddControl( refreshButton );
                }
            }
        }
        else
        {
            ValuePtr value = m_Container->GetCanvas()->Create<Value>( this );
            value->SetReadOnly( readOnly );
            group->AddControl( value );
        }
    }

    //
    // Setup label
    //

    LabelPtr label = NULL;

    {
        V_Control::const_iterator itr = group->GetControls().begin();
        V_Control::const_iterator end = group->GetControls().end();
        for( ; itr != end; ++itr )
        {
            Label* label = Reflect::ObjectCast<Label>( *itr );
            if (label)
            {
                break;
            }
        }
    }

    if (label == NULL)
    {
        label = group->GetCanvas()->Create<Label>(this);
        label->SetText( field->m_UIName );

        group->InsertControl(0, label);
    }

    //
    // Create type m_FinderSpecific data bound to this and additional instances
    //

    std::vector<Serializer*> ser;

    {
        std::vector<Reflect::Element*>::const_iterator itr = instances.begin();
        std::vector<Reflect::Element*>::const_iterator end = instances.end();
        for ( ; itr != end; ++itr )
        {
            SerializerPtr s = field->CreateSerializer();

            if (s->HasType(Reflect::GetType<ContainerSerializer>()))
            {
                return;
            }

            s->ConnectField(*itr, field);

            ser.push_back(s);

            m_Serializers.push_back(s);
        }
    }

    //
    // Create data and bind
    //

    Nocturnal::SmartPtr< MultiStringFormatter<Serializer> > data = new MultiStringFormatter<Serializer>( ser );

    if (changingDelegate.Valid())
    {
        data->AddChangingListener( changingDelegate );
    }

    {
        std::vector<ContainerPtr>::const_iterator itr = groups.begin();
        std::vector<ContainerPtr>::const_iterator end = groups.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->Bind( data );
        }
    }

    //
    // Set default
    //

    if (field->m_Default != NULL)
    {
        std::stringstream outStream;

        *field->m_Default >> outStream;

        group->SetDefault(outStream.str());
    }

    //
    // Close
    //

    {
        std::vector<ContainerPtr>::const_iterator itr = groups.begin();
        std::vector<ContainerPtr>::const_iterator end = groups.end();
        for ( ; itr != end; ++itr )
        {
            parent->AddControl(*itr);
        }
    }
}

bool FileInterpreter::DataChanging( DataChangingArgs& args )
{
    std::string text;
    Reflect::Serializer::GetValue( args.m_NewValue, text );

    if ( !text.empty() )
    {
        Nocturnal::Path path( text );

        if ( path.IsFile() )
        {
            return true;
        }

        std::string dir;
        if ( path.IsDirectory() )
        {
            dir = path.Get();
        }

        // case 1: the path is right but the file is wrong
        if ( dir.empty() )
        {
            char drive[MAX_PATH], folder[MAX_PATH], file[MAX_PATH], ext[MAX_PATH];
            _splitpath(text.c_str(), drive, folder, file, ext);
            std::ostringstream directory;
            directory << drive << folder;
            if ( Nocturnal::Path( directory.str() ).Exists() )
            {
                dir = directory.str();
            }
        }

        // case 2: get as close as we can to a valid directory
        if (dir.empty())
        {
            std::string temp;
            const char* token = strtok(&temp[0], "/");
            while (token)
            {
                temp = temp + token + "/";
                if ((strlen(token) == 2 && token[1] == ':') || Nocturnal::Path( temp ).Exists() )
                {
                    dir = dir + token + "/";
                    token = strtok(NULL, "/");
                }
                else
                {
                    token = NULL;
                }
            }
        }

        if (!dir.empty())
        {
            text = dir;
        }

        Nocturnal::FileDialog dialog ( m_Container->GetWindow(), wxFileSelectorPromptStr, text.c_str() );

        if ( !m_FileFilter.empty() )
        {
            dialog.SetFilter( m_FileFilter );
        }

        if ( dialog.ShowModal() == wxID_OK )
        {
            Reflect::Serializer::SetValue< std::string >( args.m_NewValue, dialog.GetPath().c_str() );
        }
    }

    return true;
}

void FileInterpreter::Edit( Button* button )
{
    StringData* data = static_cast<StringData*>(button->GetData().Ptr());

    std::string str;
    data->Get( str );

    if ( !str.empty() )
    {
        g_EditFilePath.Raise( EditFilePathArgs( str ) );
    }
}

void FileInterpreter::OnDrop( const Inspect::FilteredDropTargetArgs& args )
{
    if ( args.m_Paths.size() )
    {
        m_Value->SetText( args.m_Paths[ 0 ] );
    }
}