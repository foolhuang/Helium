#include "ReflectPch.h"
#include "Reflect/ArchiveBinary.h"

#include "Reflect/Object.h"
#include "Reflect/Structure.h"
#include "Reflect/Registry.h"
#include "Reflect/Data/DataDeduction.h"
#include "Foundation/Container/Insert.h" 
#include "Foundation/Memory/Endian.h"

using Helium::Insert;
using namespace Helium;
using namespace Helium::Reflect; 

const uint32_t ArchiveBinary::CURRENT_VERSION = 8;

class StlVectorPusher : NonCopyable
{
public:
    std::vector< ObjectPtr >& m_ObjectVector;

    explicit StlVectorPusher( std::vector< ObjectPtr >& objectVector )
        : m_ObjectVector( objectVector )
    {
    }

    void operator()( const ObjectPtr& object )
    {
        m_ObjectVector.push_back( object );
    }
};

class DynArrayPusher : NonCopyable
{
public:
    DynArray< ObjectPtr >& m_ObjectArray;

    explicit DynArrayPusher( DynArray< ObjectPtr >& objectArray )
        : m_ObjectArray( objectArray )
    {
    }

    void operator()( const ObjectPtr& object )
    {
        m_ObjectArray.Push( object );
    }
};

ArchiveBinary::ArchiveBinary( const Path& path, ByteOrder byteOrder )
: Archive( path, byteOrder )
, m_Version( CURRENT_VERSION )
, m_Size( 0 )
, m_Skip( false )
{
}

ArchiveBinary::ArchiveBinary( CharStream *stream, bool write /*= false */ )
: Archive()
, m_Version( CURRENT_VERSION )
, m_Size( 0 )
, m_Skip( false )
{
    OpenStream(stream, write);
}

ArchiveBinary::ArchiveBinary()
: Archive()
, m_Version( CURRENT_VERSION )
, m_Size( 0 )
, m_Skip( false )
{
}

void ArchiveBinary::Open( bool write )
{
#ifdef REFLECT_ARCHIVE_VERBOSE
    Log::Print(TXT("Opening file '%s'\n"), m_Path.c_str());
#endif

    Reflect::CharStreamPtr stream = new CharFileStream( m_Path, write ); 
    stream->SetByteOrder( m_ByteOrder );
    OpenStream( stream, write );
}

void ArchiveBinary::OpenStream( CharStream* stream, bool write )
{
    // save the mode here, so that we safely refer to it later.
    m_Mode = (write) ? ArchiveModes::Write : ArchiveModes::Read; 

    // open the stream, this is "our interface" 
    stream->Open(); 

    // Setup stream
    m_Stream = stream; 
}

void ArchiveBinary::Close()
{
    HELIUM_ASSERT( m_Stream );

    m_Stream->Close(); 
    m_Stream = NULL; 
}

void ArchiveBinary::Read()
{
    REFLECT_SCOPE_TIMER( ("Reflect - Binary Read") );

    ArchiveStatus info( *this, ArchiveStates::Starting );
    e_Status.Raise( info );

    m_Abort = false;

    // determine the size of the input stream
    m_Stream->SeekRead(0, std::ios_base::end);
    m_Size = (long) m_Stream->TellRead();
    m_Stream->SeekRead(0, std::ios_base::beg);

    // fail on an empty input stream
    if ( m_Size == 0 )
    {
        throw Reflect::StreamException( TXT( "Input stream is empty (%s)" ), m_Path.c_str() );
    }

    // read byte order
    ByteOrder byteOrder = Helium::PlatformByteOrder;
    uint16_t byteOrderMarker = 0;
    m_Stream->Read( &byteOrderMarker );
    switch ( byteOrderMarker )
    {
    case 0xfeff:
        byteOrder = Helium::PlatformByteOrder;
        break;
    case 0xfffe:
        switch ( Helium::PlatformByteOrder )
        {
        case ByteOrders::LittleEndian:
            byteOrder = ByteOrders::BigEndian;
            break;
        case ByteOrders::BigEndian:
            byteOrder = ByteOrders::LittleEndian;
            break;
        }
        break;
    default:
        throw Helium::Exception( TXT( "Unknown byte order read from file: %s" ), m_Path.c_str() );
    }
    m_Stream->SetByteOrder( byteOrder );

    // read character encoding
    CharacterEncoding characterEncoding = CharacterEncodings::ASCII;
    uint8_t encodingByte;
    m_Stream->Read(&encodingByte);
    characterEncoding = (CharacterEncoding)encodingByte;
    if ( characterEncoding != CharacterEncodings::ASCII && characterEncoding != CharacterEncodings::UTF_16 )
    {
        throw Reflect::StreamException( TXT( "Input stream contains an unknown character encoding: %d (%s)\n" ), characterEncoding, m_Path.c_str() ); 
    }
    m_Stream->SetCharacterEncoding( characterEncoding );

    // read version
    m_Stream->Read(&m_Version);

#pragma TODO( "Decide what to do with regard to backwards/forwards compatibility." )
    if (m_Version != CURRENT_VERSION)
    {
        throw Reflect::StreamException( TXT( "Input stream version for '%s' is not what is currently supported (input: %d, current: %d)\n" ), m_Path.c_str(), m_Version, CURRENT_VERSION); 
    }

    // deserialize main file objects
    {
        REFLECT_SCOPE_TIMER( ("Read Objects") );
        DeserializeArray(m_Objects, ArchiveFlags::Status);
    }

    // invalidate the search type and abort flags so we process the append block
    const Class* searchClass = m_SearchClass;
    if ( m_SearchClass != NULL )
    {
        m_SearchClass = NULL;
        m_Skip = false;
    }

    // restore state, just in case someone wants to consume this after the fact
    m_SearchClass = searchClass;

    info.m_State = ArchiveStates::Complete;
    e_Status.Raise( info );
}

void ArchiveBinary::Write()
{
    REFLECT_SCOPE_TIMER( ("Reflect - Binary Write") );

    ArchiveStatus info( *this, ArchiveStates::Starting );
    e_Status.Raise( info );

    // write BOM
    uint16_t feff = 0xfeff;
    m_Stream->Write( &feff ); // byte order mark

    // save character encoding value
    CharacterEncoding encoding;
#ifdef UNICODE
    encoding = CharacterEncodings::UTF_16;
    HELIUM_COMPILE_ASSERT( sizeof(wchar_t) == 2 );
#else
    encoding = CharacterEncodings::ASCII;
    HELIUM_COMPILE_ASSERT( sizeof(char) == 1 );
#endif
    uint8_t encodingByte = (uint8_t)encoding;
    m_Stream->Write(&encodingByte);

    // write version
    HELIUM_ASSERT( m_Version == CURRENT_VERSION );
    m_Stream->Write(&m_Version); 

    // serialize main file objects
    {
        REFLECT_SCOPE_TIMER( ("Write Objects") );
        SerializeArray(m_Objects, ArchiveFlags::Status);
    }

    // do cleanup
    m_Stream->Flush();

    info.m_State = ArchiveStates::Complete;
    e_Status.Raise( info );
}

void ArchiveBinary::SerializeInstance(Object* object)
{
    // write the crc of the class of object (used to factory allocate an instance when reading)
    uint32_t classCrc = 0;
    if ( object )
    {
        classCrc = Crc32( object->GetClass()->m_Name );
    }

    m_Stream->Write(&classCrc); 

    // stub out the length we are about to write
    uint32_t start_offset = (uint32_t)m_Stream->TellWrite();
    m_Stream->Write(&start_offset); 

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Get(stdout);
    Log::Print( TXT( "Serializing %s\n" ), object->GetClass()->m_Name );
    m_Indent.Push();
#endif

    if ( object )
    {
        object->PreSerialize( NULL );

        Data* data = SafeCast< Data >( object );
        if ( data )
        {
            data->Serialize(*this);
        }
        else
        {
            // push a new struct on the stack
            WriteFields data;
            data.m_Count = 0;
            data.m_CountOffset = m_Stream->TellWrite();
            m_FieldStack.push(data);
            {
                // stub out the number of fields we are about to write
                m_Stream->Write(&m_FieldStack.top().m_Count);

                // serialize each field of the object
                SerializeFields(object);

                // seek back and write our count
                m_Stream->SeekWrite(m_FieldStack.top().m_CountOffset, std::ios_base::beg);
                m_Stream->Write(&m_FieldStack.top().m_Count); 
            }
            m_FieldStack.pop();
        }

        object->PostSerialize( NULL );
    }

    // compute amound written
    uint32_t end_offset = (uint32_t)m_Stream->TellWrite();
    uint32_t length = end_offset - start_offset;

    // seek back and write written amount at start offset
    m_Stream->SeekWrite(start_offset, std::ios_base::beg);
    m_Stream->Write(&length); 

    // seek back to the end of the stream
    m_Stream->SeekWrite(0, std::ios_base::end);

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Pop();
#endif
}

void ArchiveBinary::SerializeInstance( void* structure, const Structure* type )
{
    // write the crc of the class of structure (used to factory allocate an instance when reading)
    uint32_t typeCrc = Crc32( type->m_Name );
    m_Stream->Write(&typeCrc); 

    // stub out the length we are about to write
    uint32_t start_offset = (uint32_t)m_Stream->TellWrite();
    m_Stream->Write(&start_offset); 

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Get(stdout);
    Log::Print( TXT( "Serializing %s\n" ), type->m_Name );
    m_Indent.Push();
#endif

    if ( structure )
    {
        // push a new struct on the stack
        WriteFields data;
        data.m_Count = 0;
        data.m_CountOffset = m_Stream->TellWrite();
        m_FieldStack.push(data);
        {
            // stub out the number of fields we are about to write
            m_Stream->Write(&m_FieldStack.top().m_Count);

            // serialize each field of the structure
            SerializeFields(structure, type);

            // seek back and write our count
            m_Stream->SeekWrite(m_FieldStack.top().m_CountOffset, std::ios_base::beg);
            m_Stream->Write(&m_FieldStack.top().m_Count); 
        }
        m_FieldStack.pop();
    }

    // compute amound written
    uint32_t end_offset = (uint32_t)m_Stream->TellWrite();
    uint32_t length = end_offset - start_offset;

    // seek back and write written amount at start offset
    m_Stream->SeekWrite(start_offset, std::ios_base::beg);
    m_Stream->Write(&length); 

    // seek back to the end of the stream
    m_Stream->SeekWrite(0, std::ios_base::end);

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Pop();
#endif
}

void ArchiveBinary::SerializeFields( Object* object )
{
    const Composite* composite = object->GetClass();

    std::stack< const Composite* > bases;
    for ( const Composite* current = composite; current != NULL; current = current->m_Base )
    {
        bases.push( current );
    }

    while ( !bases.empty() )
    {
        const Composite* current = bases.top();
        bases.pop();

        DynArray< Field >::ConstIterator itr = current->m_Fields.Begin();
        DynArray< Field >::ConstIterator end = current->m_Fields.End();
        for ( ; itr != end; ++itr )
        {
            const Field* field = &*itr;

            // check to see if we should serialize (will return non-null if we are gtg)
            DataPtr data = object->ShouldSerialize( field );
            if ( data )
            {
                uint32_t fieldNameCrc = Crc32( field->m_Name );
                m_Stream->Write(&fieldNameCrc); 

#ifdef REFLECT_ARCHIVE_VERBOSE
                m_Indent.Get(stdout);
                Log::Print(TXT("Serializing field %s (class %s)\n"), field->m_Name, field->m_DataClass->m_Name);
                m_Indent.Push();
#endif

                object->PreSerialize( field );
                SerializeInstance( data );
                object->PostSerialize( field );

                // might be useful to cache the data object here
                data->Disconnect();               

#ifdef REFLECT_ARCHIVE_VERBOSE
                m_Indent.Pop();
#endif

                // we wrote a field, so increment our count
                HELIUM_ASSERT(m_FieldStack.size() > 0);
                m_FieldStack.top().m_Count++;
            }
        }
    }

    const int32_t terminator = -1;
    m_Stream->Write(&terminator); 
}

void ArchiveBinary::SerializeFields( void* structure, const Structure* type )
{
    std::stack< const Composite* > bases;
    for ( const Composite* current = type; current != NULL; current = current->m_Base )
    {
        bases.push( current );
    }

    while ( !bases.empty() )
    {
        const Composite* current = bases.top();
        bases.pop();

        DynArray< Field >::ConstIterator itr = current->m_Fields.Begin();
        DynArray< Field >::ConstIterator end = current->m_Fields.End();
        for ( ; itr != end; ++itr )
        {
            const Field* field = &*itr;

            // check to see if we should serialize (will return non-null if we are gtg)
            DataPtr data = field->ShouldSerialize( structure );
            if ( data )
            {
                uint32_t fieldNameCrc = Crc32( field->m_Name );
                m_Stream->Write(&fieldNameCrc); 

#ifdef REFLECT_ARCHIVE_VERBOSE
                m_Indent.Get(stdout);
                Log::Print(TXT("Serializing field %s (class %s)\n"), field->m_Name, field->m_DataClass->m_Name);
                m_Indent.Push();
#endif

                SerializeInstance( data );

                // might be useful to cache the data object here
                data->Disconnect();               

#ifdef REFLECT_ARCHIVE_VERBOSE
                m_Indent.Pop();
#endif

                // we wrote a field, so increment our count
                HELIUM_ASSERT(m_FieldStack.size() > 0);
                m_FieldStack.top().m_Count++;
            }
        }
    }

    const int32_t terminator = -1;
    m_Stream->Write(&terminator); 
}

void ArchiveBinary::SerializeArray(const std::vector< ObjectPtr >& objects, uint32_t flags)
{
    SerializeArray( objects.begin(), objects.end(), flags );
}

void ArchiveBinary::SerializeArray( const DynArray< ObjectPtr >& objects, uint32_t flags )
{
    SerializeArray( objects.Begin(), objects.End(), flags );
}

template< typename ConstIteratorType >
void ArchiveBinary::SerializeArray( ConstIteratorType begin, ConstIteratorType end, uint32_t flags )
{
    int32_t size = (int32_t)( end - begin );
    m_Stream->Write(&size); 

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Get(stdout);
    Log::Print(TXT("Serializing %d objects\n"), size);
    m_Indent.Push();
#endif

    ConstIteratorType itr = begin;
    for (int index = 0; itr != end; ++itr, ++index )
    {
        SerializeInstance(*itr);

        if ( flags & ArchiveFlags::Status )
        {
            ArchiveStatus info( *this, ArchiveStates::ObjectProcessed );
            info.m_Progress = (int)(((float)(index) / (float)size) * 100.0f);
            e_Status.Raise( info );
        }
    }

    if ( flags & ArchiveFlags::Status )
    {
        ArchiveStatus info( *this, ArchiveStates::ObjectProcessed );
        info.m_Progress = 100;
        e_Status.Raise( info );
    }

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Pop();
#endif

    const int32_t terminator = -1;
    m_Stream->Write(&terminator); 
}

void Helium::Reflect::ArchiveBinary::ReadSingleObject( ObjectPtr& object )
{
    if (!object.ReferencesObject())
    {
        object = Allocate();
    }
    else
    {
        // consume crc
        uint32_t typeCrc = Helium::BeginCrc32();
        m_Stream->Read(&typeCrc);

        const Class* type = NULL;
        if ( typeCrc != 0 )
        {
            type = Reflect::Registry::GetInstance()->GetClass( typeCrc );
            HELIUM_ASSERT(type == object->GetClass());
        }

        // consume length
        uint32_t length = 0;
        m_Stream->Read(&length);
    }

    DeserializeInstance(object);
}

void ArchiveBinary::DeserializeInstance(ObjectPtr& object)
{
    //
    // If we don't have an object allocated for deserialization, pull one from the stream
    //

    if (!object.ReferencesObject())
    {
        object = Allocate();
    }

    //
    // We should now have an instance (unless data was skipped)
    //

    if (object.ReferencesObject())
    {
#ifdef REFLECT_ARCHIVE_VERBOSE
        m_Indent.Get(stdout);
        Log::Print(TXT("Deserializing %s\n"), object->GetClass()->m_Name);
        m_Indent.Push();
#endif

        object->PreDeserialize( NULL );

        Data* data = SafeCast<Data>(object);

        if ( data )
        {
            const DeserializingField *deserializing_field = GetDeserializingField();
            if (deserializing_field)
            {
                //data->ConnectField(deserializing_field->m_Instance, deserializing_field->m_Field);
            }

            data->Deserialize(*this);
        }
        else
        {
            DeserializeFields(object);
        }

        object->PostDeserialize( NULL );

#ifdef REFLECT_ARCHIVE_VERBOSE
        m_Indent.Pop();
#endif
    }
}

void ArchiveBinary::DeserializeInstance( void* structure, const Structure* type )
{
#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Get(stdout);
    Log::Print(TXT("Deserializing %s\n"), type->m_Name);
    m_Indent.Push();
#endif

    // Inserting a dummy allocate call here to read through the header for the struct type
    //Allocate();
    DeserializeFields(structure, type);

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Pop();
#endif
}

void ArchiveBinary::DeserializeFields(Object* object)
{
    int32_t fieldCount = -1;
    m_Stream->Read(&fieldCount); 
    
    size_t deserializing_field_index = m_DeserializingFieldStack.GetSize();
    //DeserializingField *deserializing_field = m_DeserializingFieldStack.New();
    m_DeserializingFieldStack.New();
    //HELIUM_ASSERT(deserializing_field);
    HELIUM_ASSERT(m_DeserializingFieldStack.GetSize() == (deserializing_field_index + 1));
    //deserializing_field->m_Instance = structure;
    m_DeserializingFieldStack[deserializing_field_index].m_Instance = object;
    for (int i=0; i<fieldCount; i++)
    {
        uint32_t fieldNameCrc = BeginCrc32();
        m_Stream->Read( &fieldNameCrc );

        const Class* type = object->GetClass();
        HELIUM_ASSERT( type );

        ObjectPtr unknown;

        const Field* field = type->FindFieldByName(fieldNameCrc);
        //deserializing_field->m_Field = field;
        m_DeserializingFieldStack[deserializing_field_index].m_Field = field;
        if ( field )
        {
#ifdef REFLECT_ARCHIVE_VERBOSE
            m_Indent.Get(stdout);
            Log::Print(TXT("Deserializing field %s\n"), field->m_Name);
            m_Indent.Push();
#endif

            // pull and object and downcast to data
            DataPtr latentData = SafeCast<Data>( Allocate() );
            if (!latentData.ReferencesObject())
            {
                // this should never happen, the type id read from the file is bogus
                throw Reflect::TypeInformationException( TXT( "Unknown data for field %s (%s)" ), field->m_Name, m_Path.c_str() );
#pragma TODO("Support blind data")
            }

            // if the types match we are a natural fit to just deserialize directly into the field data
            if ( field->m_DataClass == field->m_DataClass )
            {
                // set data pointer
                latentData->ConnectField( object, field );

                // process natively
                object->PreDeserialize( field );
                DeserializeInstance( (ObjectPtr&)latentData );
                object->PostDeserialize( field );

                // disconnect
                latentData->Disconnect();
            }
            else // else the type does not match, deserialize it into temp data then attempt to cast it into the field data
            {
                REFLECT_SCOPE_TIMER(("Casting"));

                // construct current serialization object
                ObjectPtr currentObject = Registry::GetInstance()->CreateInstance( field->m_DataClass );

                // downcast to data
                DataPtr currentData = SafeCast<Data>(currentObject);
                if (!currentData.ReferencesObject())
                {
                    // this should never happen, the type id in the rtti data is bogus
                    throw Reflect::TypeInformationException( TXT( "Invalid type id for field %s (%s)" ), field->m_Name, m_Path.c_str() );
                }

                // process into temporary memory
                currentData->ConnectField(object, field);

                // process natively
                object->PreDeserialize( field );
                DeserializeInstance( (ObjectPtr&)latentData );

                // attempt cast data into new definition
                if ( !Data::CastValue( latentData, currentData, DataFlags::Shallow ) )
                {
                    // handle as unknown
                    unknown = latentData;
                }
                else
                {
                    // post process
                    object->PostDeserialize( field );
                }

                // disconnect
                currentData->Disconnect();
            }
        }
        else // else the field does not exist in the current class anymore
        {
            try
            {
                DeserializeInstance( unknown );
            }
            catch (Reflect::LogisticException& ex)
            {
                Log::Debug( TXT( "Unable to deserialize %s::%s, discarding: %s\n" ), type->m_Name, field->m_Name, ex.What());
            }
        }

        if ( unknown.ReferencesObject() )
        {
            // attempt processing
            object->ProcessUnknown( unknown, field ? Crc32( field->m_Name ) : 0 );
        }

#ifdef REFLECT_ARCHIVE_VERBOSE
        m_Indent.Pop();
#endif
    }

    m_DeserializingFieldStack.Pop();

    int32_t terminator = -1;
    m_Stream->Read(&terminator); 
    if (terminator != -1)
    {
        throw Reflect::DataFormatException( TXT( "Unterminated field array block (%s)" ), m_Path.c_str() );
    }
}

void ArchiveBinary::DeserializeFields( void* structure, const Structure* type )
{
    int32_t fieldCount = -1;
    m_Stream->Read(&fieldCount);    

    size_t deserializing_field_index = m_DeserializingFieldStack.GetSize();
    //DeserializingField *deserializing_field = m_DeserializingFieldStack.New();
    m_DeserializingFieldStack.New();
    //HELIUM_ASSERT(deserializing_field);
    HELIUM_ASSERT(m_DeserializingFieldStack.GetSize() == (deserializing_field_index + 1));
    //deserializing_field->m_Instance = structure;
    m_DeserializingFieldStack[deserializing_field_index].m_Instance = structure;

    for (int i=0; i<fieldCount; i++)
    {
        uint32_t fieldNameCrc = BeginCrc32();
        m_Stream->Read( &fieldNameCrc );

        const Field* field = type->FindFieldByName(fieldNameCrc);

        //deserializing_field->m_Field = field;
        m_DeserializingFieldStack[deserializing_field_index].m_Field = field;
        if ( field )
        {
#ifdef REFLECT_ARCHIVE_VERBOSE
            m_Indent.Get(stdout);
            Log::Print(TXT("Deserializing field %s\n"), field->m_Name);
            m_Indent.Push();
#endif

            // pull and structure and downcast to data
            DataPtr latentData = SafeCast<Data>( Allocate() );
            if (!latentData.ReferencesObject())
            {
                // this should never happen, the type id read from the file is bogus
                throw Reflect::TypeInformationException( TXT( "Unknown data for field %s (%s)" ), field->m_Name, m_Path.c_str() );
#pragma TODO("Support blind data")
            }

            // if the types match we are a natural fit to just deserialize directly into the field data
            if ( field->m_DataClass == field->m_DataClass )
            {
                // set data pointer
                latentData->ConnectField( structure, field );

                // process natively
                DeserializeInstance( (ObjectPtr&)latentData );

                // disconnect
                latentData->Disconnect();
            }
            else // else the type does not match, deserialize it into temp data then attempt to cast it into the field data
            {
                REFLECT_SCOPE_TIMER(("Casting"));

                // construct current serialization structure
                ObjectPtr currentObject = Registry::GetInstance()->CreateInstance( field->m_DataClass );

                // downcast to data
                DataPtr currentData = SafeCast<Data>(currentObject);
                if (!currentData.ReferencesObject())
                {
                    // this should never happen, the type id in the rtti data is bogus
                    throw Reflect::TypeInformationException( TXT( "Invalid type id for field %s (%s)" ), field->m_Name, m_Path.c_str() );
                }

                // process into temporary memory
                currentData->ConnectField(structure, field);

                // process natively
                DeserializeInstance( (ObjectPtr&)latentData );

                // attempt cast data into new definition
                Data::CastValue( latentData, currentData, DataFlags::Shallow );

                // disconnect
                currentData->Disconnect();
            }
        }

#ifdef REFLECT_ARCHIVE_VERBOSE
        m_Indent.Pop();
#endif
    }

    m_DeserializingFieldStack.Pop();

    int32_t terminator = -1;
    m_Stream->Read(&terminator); 
    if (terminator != -1)
    {
        throw Reflect::DataFormatException( TXT( "Unterminated field array block (%s)" ), m_Path.c_str() );
    }
}

void ArchiveBinary::DeserializeArray( std::vector< ObjectPtr >& objects, uint32_t flags )
{
    DeserializeArray( StlVectorPusher( objects ), flags );
}

void ArchiveBinary::DeserializeArray( DynArray< ObjectPtr >& objects, uint32_t flags )
{
    DeserializeArray( DynArrayPusher( objects ), flags );
}

template< typename ArrayPusher >
void ArchiveBinary::DeserializeArray( ArrayPusher& push, uint32_t flags )
{
    uint32_t start_offset = (uint32_t)m_Stream->TellRead();

    int32_t element_count = -1;
    m_Stream->Read(&element_count); 

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Get(stdout);
    Log::Debug(TXT("Deserializing %d objects\n"), element_count);
    m_Indent.Push();
#endif

    if (element_count > 0)
    {
        for (int i=0; i<element_count && !m_Abort; i++)
        {
            ObjectPtr object = Allocate();

                // A bit of a hack to handle structs.
                StructureData* structure_data = SafeCast<StructureData>(object);
                if ( structure_data )
                {
                    //deserializing_field->m_Instance = structure_data->m_Data.Get(deserializing_field->m_Field->m_Size);
                    const DeserializingField *deserializing_field = GetDeserializingField();
                    HELIUM_ASSERT(deserializing_field);
                    structure_data->AllocateForArrayEntry(deserializing_field->m_Instance, deserializing_field->m_Field);
                    
    // Inserting a dummy allocate call here to read through the header for the struct type
    Allocate();
                }
                else
                {
                    //deserializing_field->m_Instance = object.Get();
                }

            DeserializeInstance(object);

            if (object.ReferencesObject())
            {
                if ( object->IsClass( m_SearchClass ) )
                {
                    m_Skip = true;
                }

                if ( flags & ArchiveFlags::Status )
                {
                    uint32_t current = (uint32_t)m_Stream->TellRead();

                    ArchiveStatus info( *this, ArchiveStates::ObjectProcessed );
                    info.m_Progress = (int)(((float)(current - start_offset) / (float)m_Size) * 100.0f);
                    e_Status.Raise( info );

                    m_Abort |= info.m_Abort;
                }
            }

            push( object );
        }
    }

#ifdef REFLECT_ARCHIVE_VERBOSE
    m_Indent.Pop();
#endif

    if (!m_Abort)
    {
        int32_t terminator = -1;
        m_Stream->Read(&terminator);
        if (terminator != -1)
        {
            throw Reflect::DataFormatException( TXT( "Unterminated object array block (%s)" ), m_Path.c_str() );
        }
    }

    if ( flags & ArchiveFlags::Status )
    {
        ArchiveStatus info( *this, ArchiveStates::ObjectProcessed );
        info.m_Progress = 100;
        e_Status.Raise( info );
    }
}

ObjectPtr ArchiveBinary::Allocate()
{
    ObjectPtr object;

    // read type string
    uint32_t typeCrc = Helium::BeginCrc32();
    m_Stream->Read(&typeCrc);

    // A null type name CRC indicates that a null reference was serialized, so no type lookup needs to be performed.
    const Class* reflect_class = NULL;
    const Type* reflect_type = NULL;
    if ( typeCrc != 0 )
    {
        reflect_class = Reflect::Registry::GetInstance()->GetClass( typeCrc );
        reflect_type = Reflect::Registry::GetInstance()->GetType( typeCrc );
    }

    // read length info if we have it
    uint32_t length = 0;
    m_Stream->Read(&length);

    if ( m_Skip || typeCrc == 0 )
    {
        // skip it, but account for already reading the length from the stream
        m_Stream->SeekRead(length - sizeof(uint32_t), std::ios_base::cur);
    }
    else
    {
        if (reflect_class)
        {
            // allocate instance by name
            object = Registry::GetInstance()->CreateInstance( reflect_class );
        }

        // if we failed
        if (!object.ReferencesObject())
        {
            if (!reflect_class && reflect_type)
            {
                // It's a struct, don't worry about it
            }
            else
            {
                // skip it, but account for already reading the length from the stream
                m_Stream->SeekRead(length - sizeof(uint32_t), std::ios_base::cur);

                // if you see this, then data is being lost because:
                //  1 - a type was completely removed from the codebase
                //  2 - a type was not found because its type library is not registered
                Log::Debug( TXT( "Unable to create object of type %s, size %d, skipping...\n" ), reflect_class ? reflect_class->m_Name : TXT("Unknown"), length);
    #pragma TODO("Support blind data")
            }
        }
    }

    return object;
}

void ArchiveBinary::ToStream( Object* object, std::iostream& stream )
{
    std::vector< ObjectPtr > objects(1);
    objects[0] = object;
    ToStream( objects, stream );
}

ObjectPtr ArchiveBinary::FromStream( std::iostream& stream, const Class* searchClass )
{
    if ( searchClass == NULL )
    {
        searchClass = Reflect::GetClass<Object>();
    }

    ArchiveBinary archive;
    archive.m_SearchClass = searchClass;

    Reflect::CharStreamPtr charStream = new CharStream( &stream, false ); 
    archive.OpenStream( charStream, false );
    archive.Read();
    archive.Close(); 

    std::vector< ObjectPtr >::iterator itr = archive.m_Objects.begin();
    std::vector< ObjectPtr >::iterator end = archive.m_Objects.end();
    for ( ; itr != end; ++itr )
    {
        if ((*itr)->IsClass(searchClass))
        {
            return *itr;
        }
    }

    return NULL;
}

void ArchiveBinary::ToStream( const std::vector< ObjectPtr >& objects, std::iostream& stream )
{
    ArchiveBinary archive;

    // fix the spool
    archive.m_Objects = objects;

    Reflect::CharStreamPtr charStream = new CharStream( &stream, false ); 
    archive.OpenStream( charStream, true );
    archive.Write();   
    archive.Close(); 
}

void ArchiveBinary::FromStream( std::iostream& stream, std::vector< ObjectPtr >& objects )
{
    ArchiveBinary archive;

    Reflect::CharStreamPtr charStream = new CharStream( &stream, false ); 
    archive.OpenStream( charStream, false );
    archive.Read();
    archive.Close(); 

    objects = archive.m_Objects;
}

void Helium::Reflect::ArchiveBinary::ReadString( tstring &str )
{
    GetStream().ReadString( str );
}

void Helium::Reflect::ArchiveBinary::WriteString( const tstring &str )
{
    GetStream().WriteString( str ); 
}
