#include "ReflectPch.h"
#include "Reflect/Composite.h"

#include "Foundation/Log.h"
#include "Reflect/Object.h"
#include "Reflect/Registry.h"
#include "Reflect/Enumeration.h"
#include "Reflect/Data/DataDeduction.h"
#include "Reflect/ArchiveBinary.h"

using namespace Helium;
using namespace Helium::Reflect;

Field::Field()
: m_Composite( NULL )
, m_Name( NULL )
, m_Flags( 0 )
, m_Index( -1 )
, m_Type( NULL )
, m_DataClass( NULL )
, m_Offset( -1 )
{

}

DataPtr Field::CreateData() const
{
    DataPtr data;

    if ( m_DataClass != NULL )
    {
        ObjectPtr object = Registry::GetInstance()->CreateInstance( m_DataClass );

        if (object.ReferencesObject())
        {
            data = AssertCast<Data>(object);
        }
    }

    return data;
}

DataPtr Field::CreateData(void* instance) const
{
    DataPtr data = CreateData();

    if ( data.ReferencesObject() )
    {
        if ( instance )
        {
            data->ConnectField( instance, this );
        }
    }

    return data;
}

DataPtr Field::CreateDefaultData() const
{
    return CreateData( m_Composite->m_Default );
}

DataPtr Field::ShouldSerialize( void* instance ) const
{
    // never write discard fields
    if ( m_Flags & FieldFlags::Discard )
    {
        return NULL;
    }

    ObjectPtr object = Registry::GetInstance()->CreateInstance( m_DataClass );
    DataPtr data = ThrowCast< Data >( object );
    data->ConnectField( instance, this );

    // always write force fields
    if ( m_Flags & FieldFlags::Force )
    {
        return data;
    }

    // check for empty/null/invalid state
    if ( !data->ShouldSerialize() )
    {
        return NULL;
    }

    // don't write field at the default value
    DataPtr defaultData = CreateDefaultData();
    if ( defaultData.ReferencesObject() && defaultData->Equals(data) )
    {
        return NULL;
    }

    return data;
}

Composite::Composite()
: m_Base( NULL )
, m_FirstDerived( NULL )
, m_NextSibling( NULL )
, m_Populate( NULL )
, m_Default( NULL )
{

}

Composite::~Composite()
{
}

void Composite::Register() const
{
    Type::Register();

    uint32_t computedSize = 0;
    DynArray< Field >::ConstIterator itr = m_Fields.Begin();
    DynArray< Field >::ConstIterator end = m_Fields.End();
    for ( ; itr != end; ++itr )
    {
        computedSize += itr->m_Size;
        Log::Debug( TXT( "  Index: %3d, Size %4d, Name: %s\n" ), itr->m_Index, itr->m_Size, itr->m_Name );
    }

    if (computedSize != m_Size)
    {
        Log::Debug( TXT( " %d bytes of hidden fields and padding\n" ), m_Size - computedSize );
    }
}

void Composite::Unregister() const
{
    Type::Unregister();
}

bool Composite::IsType(const Composite* type) const
{
    for ( const Composite* base = this; base; base = base->m_Base )
    {
        if ( base == type )
        {
            return true;
        }
    }

    return false;
}

void Composite::AddDerived( const Composite* derived ) const
{
    HELIUM_ASSERT( derived );

    derived->m_NextSibling = m_FirstDerived;
    m_FirstDerived = derived;
}

void Composite::RemoveDerived( const Composite* derived ) const
{
    HELIUM_ASSERT( derived );

    if ( m_FirstDerived == derived )
    {
        m_FirstDerived = derived->m_NextSibling;
    }
    else
    {
        for ( const Composite* sibling = m_FirstDerived; sibling; sibling = sibling->m_NextSibling )
        {
            if ( sibling->m_NextSibling == derived )
            {
                sibling->m_NextSibling = derived->m_NextSibling;
                break;
            }
        }
    }

    derived->m_NextSibling = NULL;
}

bool Composite::Equals(void* a, void* b) const
{
    if (a == b)
    {
        return true;
    }

    if (!a || !b)
    {
        return false;
    }

    DynArray< Field >::ConstIterator itr = m_Fields.Begin();
    DynArray< Field >::ConstIterator end = m_Fields.End();
    for ( ; itr != end; ++itr )
    {
        const Field* field = &*itr;

        // create data objects
        DataPtr aData = field->CreateData();
        DataPtr bData = field->CreateData();

        // connnect
        aData->ConnectField(a, field);
        bData->ConnectField(b, field);

        bool equality = aData->Equals( bData );

        // disconnect
        aData->Disconnect();
        bData->Disconnect();

        if ( !equality )
        {
            return false;
        }
    }

    return true;
}

void Composite::Visit(void* instance, Visitor& visitor) const
{
    if (!instance)
    {
        return;
    }

    DynArray< Field >::ConstIterator itr = m_Fields.Begin();
    DynArray< Field >::ConstIterator end = m_Fields.End();
    for ( ; itr != end; ++itr )
    {
        const Field* field = &*itr;

        if ( !visitor.VisitField( instance, field ) )
        {
            continue;
        }

        DataPtr data = field->CreateData();

        data->ConnectField( instance, field );

        data->Accept( visitor );

        data->Disconnect();
    }
}

void Composite::Copy( void* source, void* destination ) const
{
    if ( source != destination )
    {
#pragma TODO("This should be inside a virtual function (like CopyTo) instead of a type check conditional")
        if ( IsType( GetClass<Data>() ) )
        {
            Data* src = static_cast<Data*>(source);
            Data* dest = static_cast<Data*>(destination);
            dest->Set( src );
        }
        else
        {
            DynArray< Field >::ConstIterator itr = m_Fields.Begin();
            DynArray< Field >::ConstIterator end = m_Fields.End();
            for ( ; itr != end; ++itr )
            {
                const Field* field = &*itr;

                // create data objects
                DataPtr lhs = field->CreateData();
                DataPtr rhs = field->CreateData();

                // connnect
                lhs->ConnectField(destination, field);
                rhs->ConnectField(source, field);

                // for normal data types, run overloaded assignement operator via data's vtable
                // for reference container types, this deep copies containers (which is bad for 
                //  non-cloneable (FieldFlags::Share) reference containers)
                bool result = lhs->Set(rhs, field->m_Flags & FieldFlags::Share ? DataFlags::Shallow : 0);
                HELIUM_ASSERT(result);

                // disconnect
                lhs->Disconnect();
                rhs->Disconnect();
            }
        }
    }
}

const Field* Composite::FindFieldByName(uint32_t crc) const
{
    for ( const Composite* current = this; current != NULL; current = current->m_Base )
    {
        DynArray< Field >::ConstIterator itr = current->m_Fields.Begin();
        DynArray< Field >::ConstIterator end = current->m_Fields.End();
        for ( ; itr != end; ++itr )
        {
            if ( Crc32( itr->m_Name ) == crc )
            {
                return &*itr;
            }
        }
    }

    return NULL;
}

const Field* Composite::FindFieldByIndex(uint32_t index) const
{
    for ( const Composite* current = this; current != NULL; current = current->m_Base )
    {
        if ( current->m_Fields.GetSize() && index >= current->m_Fields.GetFirst().m_Index && index <= current->m_Fields.GetFirst().m_Index )
        {
            return &current->m_Fields[ index - current->m_Fields.GetFirst().m_Index ];
        }
    }

    return NULL;
}

const Field* Composite::FindFieldByOffset(uint32_t offset) const
{
#pragma TODO("Implement binary search")
    for ( const Composite* current = this; current != NULL; current = current->m_Base )
    {
        if ( current->m_Fields.GetSize() && offset >= current->m_Fields.GetFirst().m_Offset && offset <= current->m_Fields.GetFirst().m_Offset )
        {
            DynArray< Field >::ConstIterator itr = current->m_Fields.Begin();
            DynArray< Field >::ConstIterator end = current->m_Fields.End();
            for ( ; itr != end; ++itr )
            {
                if ( itr->m_Offset == offset )
                {
                    return &*itr;
                }
            }
        }
    }

    return NULL;
}

uint32_t Composite::GetBaseFieldCount() const
{
    uint32_t count = 0;

    for ( const Composite* base = m_Base; base; base = base->m_Base )
    {
        if ( m_Base->m_Fields.GetSize() )
        {
            count = m_Base->m_Fields.GetLast().m_Index + 1;
            break;
        }
    }

    return count;
}

Reflect::Field* Composite::AddField( const tchar_t* name, const uint32_t offset, uint32_t size, const Class* dataClass, const Type* type, int32_t flags )
{
    // deduction of the data class has failed, you must provide one yourself!
    HELIUM_ASSERT( dataClass );

    if ( dataClass == Reflect::GetClass< PointerData >() )
    {
        const Class* classType = ReflectionCast< Class >( type );

        // if you hit this, then you need to make sure you register your class before you register fields that are pointers of that type
        HELIUM_ASSERT( classType != NULL );
    }
    else if ( dataClass == Reflect::GetClass< EnumerationData >() || dataClass == Reflect::GetClass< BitfieldData >() )
    {
        const Enumeration* enumerationType = ReflectionCast< Enumeration >( type );

        // if you hit this, then you need to make sure you register your enums before you register objects that use them
        HELIUM_ASSERT( enumerationType != NULL );
    }

    Field field;
    field.m_Composite = this;
    field.m_Name = name;
    field.m_Size = size;
    field.m_Offset = offset;
    field.m_Flags = flags;
    field.m_Index = GetBaseFieldCount() + (uint32_t)m_Fields.GetSize();
    field.m_Type = type;
    field.m_DataClass = dataClass;
    m_Fields.Add( field );

    return &m_Fields.GetLast();
}
