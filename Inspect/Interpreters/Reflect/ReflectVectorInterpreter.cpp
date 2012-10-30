#include "InspectPch.h"
#include "ReflectVectorInterpreter.h"

#include "Inspect/Controls/LabelControl.h"
#include "Inspect/Controls/ValueControl.h"
#include "Inspect/Container.h"
#include "Inspect/DataBinding.h"

using namespace Helium;
using namespace Helium::Reflect;
using namespace Helium::Inspect;

ReflectVectorInterpreter::ReflectVectorInterpreter (Container* container)
: ReflectFieldInterpreter (container)
{

}

void ReflectVectorInterpreter::InterpretField(const Field* field, const std::vector<Reflect::Object*>& instances, Container* parent)
{
    if ( field->m_Flags & FieldFlags::Hide )
    {
        return;
    }

    // create the container
    ContainerPtr container = CreateControl< Container >();
    parent->AddChild( container );

    // create the label
    LabelPtr label = CreateControl< Label >();
    container->AddChild( label );
    label->a_HelpText.Set( field->GetProperty( TXT( "HelpText" ) ) );

    tstring temp;
    field->GetProperty( TXT( "UIName" ), temp );
    if ( temp.empty() )
    {
        bool converted = Helium::ConvertString( field->m_Name, temp );
        HELIUM_ASSERT( converted );
    }

    label->BindText( temp );
    label->a_HelpText.Set( field->GetProperty( TXT( "HelpText" ) ) );

#ifdef REFLECT_REFACTOR
    // compute dimensions
    int dimensions = 2;
    if ( field->m_DataClass == Reflect::GetClass<Vector3Data>() )
    {
        dimensions += 1;
    }
    if ( field->m_DataClass == Reflect::GetClass<Vector4Data>() )
    {
        dimensions += 2;
    }

    // create the dimension ui
    for ( int offset = 0; offset < dimensions*4; offset += 4 )
    {
        // create the data objects
        std::vector<Reflect::Data*> data;
        std::vector<Reflect::Object*>::const_iterator itr = instances.begin();
        std::vector<Reflect::Object*>::const_iterator end = instances.end();
        for ( ; itr != end; ++itr )
        {
            DataPtr s = new Float32Data ();

            s->ConnectField(*itr, field, offset); 

            m_Datas.push_back(s);

            data.push_back(s);
        }

        // create the text box
        ValuePtr value = CreateControl< Value >();
        container->AddChild( value );
        value->a_IsReadOnly.Set( ( field->m_Flags & FieldFlags::ReadOnly ) == FieldFlags::ReadOnly );
        value->a_HelpText.Set( field->GetProperty( TXT( "HelpText" ) ) );

        // bind the ui to the data objects
        value->Bind( new MultiStringFormatter<Data>( data ) );
    }
#endif
}
