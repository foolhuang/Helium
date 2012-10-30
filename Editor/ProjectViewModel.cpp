#include "EditorPch.h"

#include "ProjectViewModel.h"

#include "Editor/App.h"
#include "Editor/FileIconsTable.h"
#include "Editor/Settings/EditorSettings.h"

#include "Foundation/Flags.h"
#include "Foundation/String/Units.h"

REFLECT_DEFINE_ENUMERATION( Helium::Editor::ProjectMenuID );

using namespace Helium;
using namespace Helium::Editor;

const tchar_t* ProjectMenuID::s_Labels[COUNT] =
{
    TXT( "Filename" ),
    TXT( "Full Path" ),
    TXT( "Relative Path" ),
};

const wxArtID ProjectViewModel::DefaultFileIcon = ArtIDs::FileSystem::File;

///////////////////////////////////////////////////////////////////////////////
ProjectViewModelNode::ProjectViewModelNode( ProjectViewModel* model, ProjectViewModelNode* parent, const Helium::Path& path, const Document* document, const bool isContainer, const bool isActive )
: m_Model( model )
, m_ParentNode( parent )
, m_Path( path )
, m_Document( NULL )
, m_IsContainer( isContainer )
, m_IsActive( isActive )
{
    // projects shouldn't have references absolute paths in them
    HELIUM_ASSERT( m_ParentNode == NULL || !m_Path.IsAbsolute() );

    if ( document )
    {
        ConnectDocument( document );
    }
}

ProjectViewModelNode::~ProjectViewModelNode()
{
    m_ParentNode = NULL;

    DisconnectDocument();

    m_ChildNodes.clear();
}

ProjectViewModelNode* ProjectViewModelNode::GetParent()
{
    return m_ParentNode;
}

S_ProjectViewModelNodeChildren& ProjectViewModelNode::GetChildren()
{
    return m_ChildNodes;
}

bool ProjectViewModelNode::IsContainer() const
{
#pragma TODO ( "OR the file is a scene file, reflect file with manifest" )
    return ( m_IsContainer || m_ChildNodes.size() > 0 || m_Path.IsDirectory() ) ? true : false;
}

void ProjectViewModelNode::SetPath( const Helium::Path& path )
{
    if ( _tcsicmp( m_Path.c_str(), path.c_str() ) != 0 )
    {
        m_Path = path;
    }
}

const Helium::Path& ProjectViewModelNode::GetPath()
{
    return m_Path;
}

tstring ProjectViewModelNode::GetName() const
{
    if ( m_Path.IsDirectory() )
    {
        return m_Path.Get();
    }
    else
    {
        if ( wxGetApp().GetSettingsManager()->GetSettings< EditorSettings >()->GetShowFileExtensionsInProjectView() )
        {
            return m_Path.Filename();
        }
        else
        {
            return m_Path.Basename();
        }
    }
}

tstring ProjectViewModelNode::GetDetails() const
{
    return tstring( TXT( "" ) );
}

tstring ProjectViewModelNode::GetFileSize() const
{
    if ( m_Path.IsDirectory() )
    {
        return tstring( TXT( "" ) );
    }
    else if ( m_Path.IsFile() )
    {
        uint64_t size = m_Path.Size();
        return BytesToString( size );
    }

    return tstring( TXT( "" ) );
}

const Document* ProjectViewModelNode::GetDocument() const
{
    return m_Document;
}

uint32_t ProjectViewModelNode::GetDocumentStatus() const
{
    if ( m_Document )
    {
        return m_Document->GetStatus();
    }
    return DocumentStatus::Default;
}

void ProjectViewModelNode::ConnectDocument( const Document* document)
{
    if ( m_Document && m_Document != document )
    {
        DisconnectDocument();
    }

    m_Document = document;
    if ( m_Document )
    {
        m_Document->e_Saved.AddMethod( this, &ProjectViewModelNode::DocumentSaved );
        m_Document->e_Closed.AddMethod( this, &ProjectViewModelNode::DocumentClosed );
        m_Document->e_Changing.AddMethod( this, &ProjectViewModelNode::DocumentChanging );
        m_Document->e_Changed.AddMethod( this, &ProjectViewModelNode::DocumentChanged );
        m_Document->e_ModifiedOnDiskStateChanged.AddMethod( this, &ProjectViewModelNode::DocumentModifiedOnDiskStateChanged );
        m_Document->e_PathChanged.AddMethod( this, &ProjectViewModelNode::DocumentPathChanged );
    }
}

void ProjectViewModelNode::DisconnectDocument()
{
    if ( m_Document )
    {
        m_Document->e_Saved.RemoveMethod( this, &ProjectViewModelNode::DocumentSaved );
        m_Document->e_Closed.RemoveMethod( this, &ProjectViewModelNode::DocumentClosed );
        m_Document->e_Changing.RemoveMethod( this, &ProjectViewModelNode::DocumentChanging );
        m_Document->e_Changed.RemoveMethod( this, &ProjectViewModelNode::DocumentChanged );
        m_Document->e_ModifiedOnDiskStateChanged.RemoveMethod( this, &ProjectViewModelNode::DocumentModifiedOnDiskStateChanged );
        m_Document->e_PathChanged.RemoveMethod( this, &ProjectViewModelNode::DocumentPathChanged );
        m_Document = NULL;
    }
}

void ProjectViewModelNode::DocumentSaved( const DocumentEventArgs& args )
{
#pragma TODO( "Remove the icon dirty overlay and text format changes" )
}

void ProjectViewModelNode::DocumentClosed( const DocumentEventArgs& args )
{
    DisconnectDocument();
}

void ProjectViewModelNode::DocumentChanging( const DocumentEventArgs& args )
{
#pragma TODO( "Add the icon dirty overlay and change text format" )
}

void ProjectViewModelNode::DocumentChanged( const DocumentEventArgs& args )
{
#pragma TODO( "Add the icon dirty overlay" )
    m_Model->ItemChanged( wxDataViewItem( (void*)this ) );
}

void ProjectViewModelNode::DocumentModifiedOnDiskStateChanged( const DocumentEventArgs& args )
{
#pragma TODO( "Add the icon dirty overlay and change text format" )
}

void ProjectViewModelNode::DocumentPathChanged( const DocumentPathChangedArgs& args )
{
    if ( m_ParentNode )
    {
        m_Path = args.m_Document->GetPath().GetRelativePath( m_ParentNode->GetPath() );

        m_Model->ItemChanged( wxDataViewItem( (void*)this ) );
    }
    else
    {
        m_Path = args.m_Document->GetPath();
    }
}

///////////////////////////////////////////////////////////////////////////////
ProjectViewModel::ProjectViewModel( DocumentManager* documentManager )
: m_DocumentManager( documentManager )
, m_Project( NULL )
, m_RootNode( NULL )
{
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "bin" ), ArtIDs::MimeTypes::Binary ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "dat" ),ArtIDs::MimeTypes::Binary ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "hrb" ), ArtIDs::MimeTypes::ReflectBinary ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "HeliumEntity" ), ArtIDs::MimeTypes::Entity ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "HeliumProject" ), ArtIDs::MimeTypes::Project ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "HeliumScene" ), ArtIDs::MimeTypes::Scene ) );
    m_FileIconExtensionLookup.insert( M_FileIconExtensionLookup::value_type( TXT( "txt" ), ArtIDs::MimeTypes::Text ) );
}

ProjectViewModel::~ProjectViewModel()
{
    CloseProject();

    m_FileIconExtensionLookup.clear();
}

wxDataViewColumn* ProjectViewModel::CreateColumn( uint32_t id )
{
    switch( id )
    {
    default:
        {
            return NULL;
        }
        break;

    case ProjectModelColumns::Name:
        {
            wxDataViewIconTextRenderer *render = new wxDataViewIconTextRenderer();

            wxDataViewColumn *column = new wxDataViewColumn(
                ProjectModelColumns::Label( ProjectModelColumns::Name ),
                render,
                m_ColumnLookupTable.size(),
                ProjectModelColumns::Width( ProjectModelColumns::Name ),
                wxALIGN_LEFT,
                wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );

            m_ColumnLookupTable.push_back( id );

            return column;
        }
        break;

    //case ProjectModelColumns::Icon:
    //    {
    //        wxDataViewBitmapRenderer *render = new wxDataViewBitmapRenderer();

    //        wxDataViewColumn *column = new wxDataViewColumn(
    //            ProjectModelColumns::Label( ProjectModelColumns::Icon ),
    //            render,
    //            m_ColumnLookupTable.size(),
    //            ProjectModelColumns::Width( ProjectModelColumns::Icon ),
    //            wxALIGN_RIGHT,
    //            0 );

    //        column->IS

    //        m_ColumnLookupTable.push_back( id );

    //        return column;
    //    }
    //    break;

    case ProjectModelColumns::Details:
        {
            wxDataViewTextRenderer *render = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );

            wxDataViewColumn *column = new wxDataViewColumn(
                ProjectModelColumns::Label( ProjectModelColumns::Details ),
                render,
                m_ColumnLookupTable.size(),
                ProjectModelColumns::Width( ProjectModelColumns::Details ),
                wxALIGN_LEFT,
                wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE );

            m_ColumnLookupTable.push_back( id );

            return column;
        }
        break;

    case ProjectModelColumns::FileSize:
        {
            wxDataViewTextRenderer *render = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );

            wxDataViewColumn *column = new wxDataViewColumn(
                ProjectModelColumns::Label( ProjectModelColumns::FileSize ),
                render,
                m_ColumnLookupTable.size(),
                ProjectModelColumns::Width( ProjectModelColumns::FileSize ),
                wxALIGN_LEFT,
                wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE );

            m_ColumnLookupTable.push_back( id );

            return column;
        }
        break;
    }

    return NULL;
}

void ProjectViewModel::ResetColumns()
{
    m_ColumnLookupTable.clear();
}

ProjectViewModelNode* ProjectViewModel::OpenProject( Project* project, const Document* document )
{
    CloseProject();

    // Setup the new project view
    m_Project = project;
    if ( m_Project )
    {
        // Create the Node     
        m_RootNode = new ProjectViewModelNode( this, NULL, m_Project->a_Path.Get(), document, true );
        m_MM_ProjectViewModelNodesByPath.insert( MM_ProjectViewModelNodesByPath::value_type( m_Project->a_Path.Get(), m_RootNode.Ptr() ));
                
        // Add the Project's Children
        for ( std::set< Path >::const_iterator itr = m_Project->Paths().begin(), end = m_Project->Paths().end();
            itr != end; ++itr )
        {
            AddChildItem( wxDataViewItem( (void*) m_RootNode.Ptr() ), *itr );
        }

        // Connect to the Project
        m_Project->e_PathAdded.AddMethod( this, &ProjectViewModel::OnPathAdded );
        m_Project->e_PathRemoved.AddMethod( this, &ProjectViewModel::OnPathRemoved );

        return m_RootNode.Ptr();    
    }

    return NULL;
}

void ProjectViewModel::CloseProject()
{
    // Cleanup the old view
    if ( m_Project )
    {
        // Disconnect to the Project
        m_Project->e_PathAdded.RemoveMethod( this, &ProjectViewModel::OnPathAdded );
        m_Project->e_PathRemoved.RemoveMethod( this, &ProjectViewModel::OnPathRemoved );

        // Remove the Node
        if ( m_RootNode )
        {
            RemoveItem( wxDataViewItem( (void*) m_RootNode.Ptr() ) );
            m_RootNode = NULL;
        }

        // Remove the Project's Children
        m_MM_ProjectViewModelNodesByPath.clear();

        m_Project = NULL;
    }
}

bool ProjectViewModel::AddChildItem( const wxDataViewItem& parenItem, const Helium::Path& path )
{
    // Get the parent node
    ProjectViewModelNode *parentNode = static_cast< ProjectViewModelNode* >( parenItem.GetID() );
    if ( !parentNode )
    {
        HELIUM_ASSERT( m_RootNode );
        parentNode = m_RootNode.Ptr();
    }

    bool isContainer = path.HasExtension( TXT( "HeliumScene" ) );

    // Create the child node
    const Document* document = m_DocumentManager->FindDocument( path );
    Helium::StdInsert<S_ProjectViewModelNodeChildren>::Result inserted = parentNode->GetChildren().insert( new ProjectViewModelNode( this, parentNode, path, document, isContainer ) );
    if ( inserted.second )
    {
        ProjectViewModelNode* childNode = (*inserted.first);

        // See if the document is already open
        //MM_ProjectViewModelNodesByPath::iterator findNode = m_MM_ProjectViewModelNodesByPath.find( path );
        //if ( findNode != m_MM_ProjectViewModelNodesByPath.end()
        //    && findNode->first == path
        //    && findNode->second->GetDocument() ) 
        //{
        //    childNode->ConnectDocument( findNode->second->GetDocument() );
        //}

        // Add the node to the multimap and call ItemAdded
        m_MM_ProjectViewModelNodesByPath.insert( MM_ProjectViewModelNodesByPath::value_type( path, childNode ));
        if ( parentNode == m_RootNode.Ptr() )
        {
            parentNode = NULL;
        }
        ItemAdded( wxDataViewItem( parentNode ), wxDataViewItem( childNode ) );

        return true;
    }
    return false;
}

bool ProjectViewModel::RemoveChildItem( const wxDataViewItem& parenItem, const Helium::Path& path )
{
    ProjectViewModelNode *parentNode = static_cast< ProjectViewModelNode* >( parenItem.GetID() );
    if ( !parentNode )
    {
        HELIUM_ASSERT( m_RootNode );
        parentNode = m_RootNode.Ptr();
    }

    S_ProjectViewModelNodeChildren::const_iterator foundChild = parentNode->GetChildren().end();
    for ( S_ProjectViewModelNodeChildren::const_iterator itr = parentNode->GetChildren().begin(),
        end = parentNode->GetChildren().end(); itr != end; ++itr )
    {
        if ( _tcsicmp( (*itr)->GetPath().Get().c_str(), path.Get().c_str() ) == 0 )
        {
            foundChild = itr;
            break;
        }
    }

    if ( foundChild != parentNode->GetChildren().end() )
    {
        RemoveItem( wxDataViewItem( (void*)(*foundChild) ) );

        return true;
    }
    return false;
}

void ProjectViewModel::RemoveItem( const wxDataViewItem& item )
{
    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    if ( !node )
    {
        return;
    }

    // remove all of childNode's children
    while( node->GetChildren().size() > 0 )
    {
        RemoveItem( wxDataViewItem( (void*)( *node->GetChildren().begin() ) ) );
    }

    // remove it from the multimap
    for ( MM_ProjectViewModelNodesByPath::iterator lower = m_MM_ProjectViewModelNodesByPath.lower_bound( node->GetPath() ),
        upper = m_MM_ProjectViewModelNodesByPath.upper_bound( node->GetPath() );
        lower != upper && lower != m_MM_ProjectViewModelNodesByPath.end();
    ++lower )
    {
        if ( lower->second == node )
        {
            m_MM_ProjectViewModelNodesByPath.erase( lower );
            break;
        }
    }

    if ( node == m_RootNode )
    {
        return;
    }

    // Remove from the parent's childern
    // this should free the node if there are no more references to it
    ProjectViewModelNode *parentNode = node->GetParent();
    if ( parentNode )
    {
        parentNode->GetChildren().erase( node );
    }

    if ( parentNode == m_RootNode.Ptr() )
    {
        parentNode = NULL;
    }

    ItemDeleted( wxDataViewItem( (void*) parentNode ), item );
}

bool ProjectViewModel::IsDropPossible( const wxDataViewItem& item )
{
    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );

    if ( !node )
    {
        return true;
    }

    return false;
}

void ProjectViewModel::SetActive( const Path& path, bool active )
{
    Path relativePath = path.GetRelativePath( m_Project->a_Path.Get() );
    for ( MM_ProjectViewModelNodesByPath::iterator lower = m_MM_ProjectViewModelNodesByPath.lower_bound( relativePath ),
        upper = m_MM_ProjectViewModelNodesByPath.upper_bound( relativePath );
        lower != upper && lower != m_MM_ProjectViewModelNodesByPath.end();
    ++lower )
    {
        ProjectViewModelNode *node = lower->second;
        node->m_IsActive = active;
    }
}


void ProjectViewModel::OnPathAdded( const Helium::Path& path )
{
    AddChildItem( wxDataViewItem( (void*) m_RootNode.Ptr() ), path );   
}

void ProjectViewModel::OnPathRemoved( const Helium::Path& path )
{
    RemoveChildItem( wxDataViewItem( (void*) m_RootNode.Ptr() ), path );   
}

void ProjectViewModel::OnDocumentOpened( const DocumentEventArgs& args )
{
    const Document* document = static_cast< const Document* >( args.m_Document );
    HELIUM_ASSERT( document );

    HELIUM_ASSERT( m_Project );
    for ( MM_ProjectViewModelNodesByPath::iterator lower = m_MM_ProjectViewModelNodesByPath.lower_bound( document->GetPath().GetRelativePath( m_Project->a_Path.Get() ) ),
        upper = m_MM_ProjectViewModelNodesByPath.upper_bound( document->GetPath().GetRelativePath( m_Project->a_Path.Get() ) );
        lower != upper && lower != m_MM_ProjectViewModelNodesByPath.end();
    ++lower )
    {
        ProjectViewModelNode *node = lower->second;
        node->ConnectDocument( document );
    }
}

void ProjectViewModel::OnDocumenClosed( const DocumentEventArgs& args )
{
    const Document* document = static_cast< const Document* >( args.m_Document );
    HELIUM_ASSERT( document );

    for ( MM_ProjectViewModelNodesByPath::iterator lower = m_MM_ProjectViewModelNodesByPath.lower_bound( document->GetPath() ),
        upper = m_MM_ProjectViewModelNodesByPath.upper_bound( document->GetPath() );
        lower != upper && lower != m_MM_ProjectViewModelNodesByPath.end();
    ++lower )
    {
        ProjectViewModelNode *node = lower->second;
        node->DisconnectDocument();
    }

}

unsigned int ProjectViewModel::GetColumnCount() const
{
    return ProjectModelColumns::COUNT;
}

wxString ProjectViewModel::GetColumnType( unsigned int type ) const
{
    if ( ( type >= 0 )
        && ( type < ProjectModelColumns::COUNT ) )
    {
        return ProjectModelColumns::Label( type );
    }
    else
    {
        return ProjectModelColumns::s_Labels[ProjectModelColumns::COUNT];
    }
}

void ProjectViewModel::GetValue( wxVariant& variant, const wxDataViewItem& item, unsigned int column ) const
{
    if ( !item.IsOk()
        || ( column < 0 )
        || ( column >= m_ColumnLookupTable.size() ) )
    {
        return;
    }

    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    if ( !node )
    {
        return;
    }

    switch( m_ColumnLookupTable.at( column ) )
    {
    default:
        break;

    case ProjectModelColumns::Name:
        {            
            uint32_t docStatus = node->GetDocumentStatus();

            wxString name = node->GetName();
            if ( HasFlags<uint32_t>( docStatus, DocumentStatus::Changed ) )
            {
                name = wxString( TXT( '*' ) ) + name; 
            }

            wxBitmap bitmap = wxArtProvider::GetBitmap( GetArtIDFromPath( node->GetPath() ), wxART_OTHER, wxSize(16, 16) );
            if ( docStatus > 0 )
            {
                wxImage image = bitmap.ConvertToImage();
                HELIUM_ASSERT( image.Ok() );

                int overlayWidth = image.GetWidth() / 2;
                int overlayHeight = image.GetHeight() / 2;

                wxImage overlayImage;

                if ( HasFlags<uint32_t>( docStatus, DocumentStatus::Saving ) )
                {
                    overlayImage = wxArtProvider::GetBitmap( ArtIDs::Status::Busy, wxART_OTHER, wxSize( overlayWidth, overlayHeight ) ).ConvertToImage();
                    HELIUM_ASSERT( overlayImage.Ok() );
                }
                else if ( HasFlags<uint32_t>( docStatus, DocumentStatus::Loading ) )
                {
                    overlayImage = wxArtProvider::GetBitmap( ArtIDs::Status::Busy, wxART_OTHER, wxSize( overlayWidth, overlayHeight ) ).ConvertToImage();
                    HELIUM_ASSERT( overlayImage.Ok() );
                }
                else if ( HasFlags<uint32_t>( docStatus, DocumentStatus::Changed ) )
                {
                    overlayImage = wxArtProvider::GetBitmap( ArtIDs::Actions::Edit, wxART_OTHER, wxSize( overlayWidth, overlayHeight ) ).ConvertToImage();
                    HELIUM_ASSERT( overlayImage.Ok() );
                }

                if ( overlayImage.Ok() )
                {
                    if ( overlayImage.GetWidth() != overlayWidth || overlayImage.GetHeight() != overlayHeight )
                    {
                        overlayImage.Rescale( overlayWidth, overlayHeight );
                    }

                    int x = 0;
                    int y = 0;
                    IconArtFile::CalculatePlacement( image, overlayImage, OverlayQuadrants::BottomRight, x, y );
                    image.Paste( overlayImage, x, y );
                }

                bitmap = wxBitmap( image );
            }

            wxIcon icon;
            icon.CopyFromBitmap( bitmap );

            variant << wxDataViewIconText( name, icon );

        }
        break;

    //case ProjectModelColumns::Icon:
    //    {
    //        int32_t imageID = GlobalFileIconsTable().GetIconIDFromPath( node->GetPath() );
    //        wxVariant bitmapVariant;
    //        bitmapVariant.
    //        variant = GlobalFileIconsTable().GetSmallImageList()->GetBitmap( imageID );
    //    }
    //    break;

    case ProjectModelColumns::Details:
        {
            variant = node->GetDetails();
        }
        break;

    case ProjectModelColumns::FileSize:
        {
            variant = node->GetFileSize();
        }
        break;
    }
}

bool ProjectViewModel::SetValue( const wxVariant& variant, const wxDataViewItem& item, unsigned int column )
{
    // nothing should be setting column values yet!
    HELIUM_BREAK();
    return false;

    //if ( !item.IsOk()
    //    || ( column < 0 )
    //    || ( column >= m_ColumnLookupTable.size() ) )
    //{
    //    return false;
    //}

    //ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    //if ( !node )
    //{
    //    return false;
    //}

    //switch( m_ColumnLookupTable.at( column ) )
    //{
    //default:
    //    {
    //        return false;
    //    }
    //    break;

    //case ProjectModelColumns::Name:
    //    {
    //        //wxDataViewIconText iconText;
    //        //iconText << variant;
    //        //node->m_Name = iconText.GetText();
    //    }
    //    break;

    //case ProjectModelColumns::Details:
    //    {
    //        //node->m_Details = variant.GetString();
    //    }
    //    break;

    //case ProjectModelColumns::Size:
    //    {
    //        //node->m_Details = variant.GetString();
    //    }
    //    break;
    //}

    //return true;
}

bool ProjectViewModel::GetAttr( const wxDataViewItem& item, unsigned int column, wxDataViewItemAttr& attr ) const
{
    if ( !item.IsOk() )
    {
        return false;
    }

    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    HELIUM_ASSERT( node );

    // bold the entry if the node is active
    attr.SetBold( node->m_IsActive );

    // italicize the entry if it is modified
    attr.SetItalic( ( node->GetDocument() && node->GetDocument()->HasChanged() ) );

    Path nodePath = node->GetPath().GetAbsolutePath( m_Project->a_Path.Get() );
    if ( !nodePath.Exists() )
    {
        attr.SetColour( *wxRED );
    }
    else
    {
        attr.SetColour( *wxBLACK );
    }

    return true;
}

wxDataViewItem ProjectViewModel::GetParent( const wxDataViewItem& item ) const
{
    if ( !item.IsOk() )
    {
        return wxDataViewItem( 0 );
    }

    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    if ( !node
        || node == m_RootNode.Ptr()
        || !node->GetParent()
        || node->GetParent() == m_RootNode.Ptr() )
    {
        return wxDataViewItem( 0 );
    }

    return wxDataViewItem( (void*) node->GetParent() );
}

unsigned int ProjectViewModel::GetChildren( const wxDataViewItem& item, wxDataViewItemArray& items ) const
{
    ProjectViewModelNode *parentNode = static_cast< ProjectViewModelNode* >( item.GetID() );
    if ( !parentNode )
    {
        parentNode = m_RootNode.Ptr();
    }

    if ( parentNode->GetChildren().size() < 1 )
    {
        return 0;
    }

    uint32_t numAdded = 0;
    for ( S_ProjectViewModelNodeChildren::const_iterator itr = parentNode->GetChildren().begin(),
        end = parentNode->GetChildren().end(); itr != end; ++itr, ++numAdded )
    {
        items.Add( wxDataViewItem( (void*) (*itr) ) );
    }

    return numAdded;
}

bool ProjectViewModel::IsContainer( const wxDataViewItem& item ) const
{
    // root node can have children
    if ( !item.IsOk() )
    {
        return true;
    }

    ProjectViewModelNode *node = static_cast< ProjectViewModelNode* >( item.GetID() );
    return node ? node->IsContainer() : false;
}

const wxArtID& ProjectViewModel::GetArtIDFromPath( const Path& path ) const
{
    tstring extension = path.FullExtension();
    if ( extension.empty() )
    {
        return DefaultFileIcon;
    }

    M_FileIconExtensionLookup::const_iterator foundArtID = m_FileIconExtensionLookup.find( extension );
    if ( foundArtID != m_FileIconExtensionLookup.end() )
    {
        return foundArtID->second;
    }

    // try just the end extension
    if ( extension.find( '.' ) != tstring::npos )
    {
        extension = path.Extension();
        foundArtID = m_FileIconExtensionLookup.find( extension );
        if ( foundArtID != m_FileIconExtensionLookup.end() )
        {
            return foundArtID->second;
        }
    }
    
    return DefaultFileIcon;
}