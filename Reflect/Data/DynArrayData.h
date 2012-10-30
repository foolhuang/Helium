#pragma once

#include "Foundation/String.h"
#include "Reflect/Registry.h"
#include "Reflect/Data/SimpleData.h"
#include "Reflect/Data/ContainerData.h"

namespace Helium
{
    namespace Reflect
    {
        class HELIUM_REFLECT_API DynArrayData : public ContainerData
        {
        public:
            REFLECT_DECLARE_ABSTRACT( DynArrayData, ContainerData );

            virtual void SetSize( size_t size ) = 0;

            virtual const Class* GetItemClass() const = 0;
            virtual DataPtr GetItem( size_t at ) = 0;
            virtual void SetItem( size_t at, Data* value ) = 0;
            virtual void Insert( size_t at, Data* value ) = 0;
            virtual void Remove( size_t at ) = 0;
            virtual void MoveUp( std::set< size_t >& selectedIndices ) = 0;
            virtual void MoveDown( std::set< size_t >& selectedIndices ) = 0;
        };

        template< class T >
        class HELIUM_REFLECT_API SimpleDynArrayData : public DynArrayData
        {
        public:
            typedef DynArray< T > DataType;
            DataPointer< DataType > m_Data;

            typedef SimpleDynArrayData< T > DynArrayDataT;
            REFLECT_DECLARE_OBJECT( DynArrayDataT, DynArrayData )

            SimpleDynArrayData();
            ~SimpleDynArrayData();

            virtual void ConnectData( void* data ) HELIUM_OVERRIDE;

            virtual size_t GetSize() const HELIUM_OVERRIDE;
            virtual void SetSize( size_t size ) HELIUM_OVERRIDE;
            virtual void Clear() HELIUM_OVERRIDE;

            virtual const Class* GetItemClass() const HELIUM_OVERRIDE;
            virtual DataPtr GetItem( size_t at ) HELIUM_OVERRIDE;
            virtual void SetItem( size_t at, Data* value ) HELIUM_OVERRIDE;
            virtual void Insert( size_t at, Data* value ) HELIUM_OVERRIDE;
            virtual void Remove( size_t at ) HELIUM_OVERRIDE;
            virtual void MoveUp( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;
            virtual void MoveDown( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;

            virtual bool Set( Data* src, uint32_t flags = 0 ) HELIUM_OVERRIDE;
            virtual bool Equals( Object* object ) HELIUM_OVERRIDE;

            virtual void Serialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;
            virtual void Deserialize( ArchiveBinary& archive ) HELIUM_OVERRIDE;

            virtual void Serialize( ArchiveXML& archive ) HELIUM_OVERRIDE;
            virtual void Deserialize( ArchiveXML& archive ) HELIUM_OVERRIDE;

            virtual tostream& operator>>( tostream& stream ) const HELIUM_OVERRIDE;
            virtual tistream& operator<<( tistream& stream ) HELIUM_OVERRIDE;
        };
        
        typedef SimpleDynArrayData< Name > NameDynArrayData;
        typedef SimpleDynArrayData< String > StringDynArrayData;
        typedef SimpleDynArrayData< bool > BoolDynArrayData;
        typedef SimpleDynArrayData< uint8_t > UInt8DynArrayData;
        typedef SimpleDynArrayData< int8_t > Int8DynArrayData;
        typedef SimpleDynArrayData< uint16_t > UInt16DynArrayData;
        typedef SimpleDynArrayData< int16_t > Int16DynArrayData;
        typedef SimpleDynArrayData< uint32_t > UInt32DynArrayData;
        typedef SimpleDynArrayData< int32_t > Int32DynArrayData;
        typedef SimpleDynArrayData< uint64_t > UInt64DynArrayData;
        typedef SimpleDynArrayData< int64_t > Int64DynArrayData;
        typedef SimpleDynArrayData< float32_t > Float32DynArrayData;
        typedef SimpleDynArrayData< float64_t > Float64DynArrayData;
        typedef SimpleDynArrayData< Helium::GUID > GUIDDynArrayData;
        typedef SimpleDynArrayData< Helium::TUID > TUIDDynArrayData;
        typedef SimpleDynArrayData< Helium::Path > PathDynArrayData;
    }
}
