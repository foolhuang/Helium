#include "ReflectPch.h"
#include "Reflect/Data/ObjectSetData.h"

#include "Reflect/Data/DataDeduction.h"
#include "Reflect/ArchiveBinary.h"
#include "Reflect/ArchiveXML.h"

REFLECT_DEFINE_OBJECT( Helium::Reflect::ObjectSetData );

using namespace Helium::Reflect;

ObjectSetData::ObjectSetData()
{
}

ObjectSetData::~ObjectSetData()
{
}

void ObjectSetData::ConnectData( void* data )
{
    m_Data.Connect( data );
}

size_t ObjectSetData::GetSize() const
{
    return m_Data->GetSize();
}

void ObjectSetData::Clear()
{
    return m_Data->Clear();
}

bool ObjectSetData::Set( Data* src, uint32_t flags )
{
    const ObjectSetData* rhs = SafeCast< ObjectSetData >( src );
    if ( !rhs )
    {
        return false;
    }

    m_Data->Clear();

    if ( flags & DataFlags::Shallow )
    {
        *m_Data = *rhs->m_Data;
    }
    else
    {
        DataType::ConstIterator itr = rhs->m_Data->Begin();
        DataType::ConstIterator end = rhs->m_Data->End();
        for ( ; itr != end; ++itr )
        {
            Object* object = *itr;
            m_Data->Insert( object ? object->Clone() : NULL );
        }
    }

    return true;
}

bool ObjectSetData::Equals( Object* object )
{
    const ObjectSetData* rhs = SafeCast< ObjectSetData >(object);
    if ( !rhs )
    {
        return false;
    }

    if ( m_Data->GetSize() != rhs->m_Data->GetSize() )
    {
        return false;
    }

    const DataType& rhsData = *m_Data;

    DataType::ConstIterator itrLHS = m_Data->Begin();
    DataType::ConstIterator endLHS = m_Data->End();
    DataType::ConstIterator endRHS = rhsData.End();
    for ( ; itrLHS != endLHS; ++itrLHS )
    {
        Object* objectLHS = *itrLHS;

        DataType::ConstIterator itrRHS = rhsData.Begin();
        for( ; itrRHS != endRHS; ++itrRHS )
        {
            Object* objectRHS = *itrRHS;
            if( objectLHS == objectRHS || ( objectLHS && objectRHS && objectLHS->Equals( objectRHS ) ) )
            {
                break;
            }
        }

        if( itrRHS == endRHS )
        {
            return false;
        }
    }

    return true;
}

void ObjectSetData::Accept( Visitor& visitor )
{
    DataType::Iterator itr = const_cast< DataPointer< DataType >& >( m_Data )->Begin();
    DataType::Iterator end = const_cast< DataPointer< DataType >& >( m_Data )->End();
    for ( ; itr != end; ++itr )
    {
        // Set keys are immutable, so we need to const_cast here to get VisitPointer()'s non-const reference later.
        ObjectPtr& object = const_cast< ObjectPtr& >( *itr );
        if ( !object.Get() )
        {
            continue;
        }

        if ( !visitor.VisitPointer( object ) )
        {
            continue;
        }

        object->Accept( visitor );
    }
}

void ObjectSetData::Serialize( ArchiveBinary& archive )
{
    Serialize<ArchiveBinary>( archive );
}

void ObjectSetData::Deserialize( ArchiveBinary& archive )
{
    Deserialize<ArchiveBinary>( archive );
}

void ObjectSetData::Serialize( ArchiveXML& archive )
{
    Serialize<ArchiveXML>( archive );
}

void ObjectSetData::Deserialize( ArchiveXML& archive )
{
    Deserialize<ArchiveXML>( archive );
}

template< class ArchiveT >
void ObjectSetData::Serialize( ArchiveT& archive )
{
    DynArray< ObjectPtr > components;
    components.Reserve( m_Data->GetSize() );

    DataType::ConstIterator itr = m_Data->Begin();
    DataType::ConstIterator end = m_Data->End();
    for ( ; itr != end; ++itr )
    {
        components.Push( *itr );
    }

    archive.SerializeArray( components );
}

template< class ArchiveT >
void ObjectSetData::Deserialize( ArchiveT& archive )
{
    DynArray< ObjectPtr > components;
    archive.DeserializeArray( components );

    // if we are referring to a real field, clear its contents
    m_Data->Clear();
    m_Data->Reserve( components.GetSize() );

    DynArray< ObjectPtr >::ConstIterator itr = components.Begin();
    DynArray< ObjectPtr >::ConstIterator end = components.End();
    for ( ; itr != end; ++itr )
    {
        m_Data->Insert( *itr );
    }
}
