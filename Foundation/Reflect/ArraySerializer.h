#pragma once

#include "Registry.h"
#include "SimpleSerializer.h"
#include "ContainerSerializer.h"

namespace Helium
{
    namespace Reflect
    {
        class FOUNDATION_API ArraySerializer : public ContainerSerializer
        {
        public:
            REFLECT_DECLARE_ABSTRACT( ArraySerializer, ContainerSerializer );

            virtual void SetSize(size_t size) = 0;

            virtual int32_t GetItemType() const = 0;
            virtual SerializerPtr GetItem(size_t at) = 0;
            virtual ConstSerializerPtr GetItem(size_t at) const = 0;
            virtual void SetItem(size_t at, const Serializer* value) = 0;
            virtual void Insert( size_t at, const Serializer* value ) = 0;
            virtual void Remove( size_t at ) = 0;
            virtual void MoveUp( std::set< size_t >& selectedIndices ) = 0;
            virtual void MoveDown( std::set< size_t >& selectedIndices ) = 0;
        };

        template <class T>
        class FOUNDATION_API SimpleArraySerializer : public ArraySerializer
        {
        public:
            typedef std::vector<T> DataType;
            Serializer::DataPtr<DataType> m_Data;

            typedef SimpleArraySerializer<T> ArraySerializerT;
            REFLECT_DECLARE_CLASS( ArraySerializerT, ArraySerializer )

                SimpleArraySerializer();
            virtual ~SimpleArraySerializer();

            virtual void ConnectData(Helium::HybridPtr<void> data) HELIUM_OVERRIDE;

            virtual size_t GetSize() const HELIUM_OVERRIDE;
            virtual void SetSize(size_t size) HELIUM_OVERRIDE;
            virtual void Clear() HELIUM_OVERRIDE;

            virtual int32_t GetItemType() const HELIUM_OVERRIDE;
            virtual SerializerPtr GetItem(size_t at) HELIUM_OVERRIDE;
            virtual ConstSerializerPtr GetItem(size_t at) const HELIUM_OVERRIDE;
            virtual void SetItem(size_t at, const Serializer* value) HELIUM_OVERRIDE;
            virtual void Insert( size_t at, const Serializer* value ) HELIUM_OVERRIDE;
            virtual void Remove( size_t at ) HELIUM_OVERRIDE;
            virtual void MoveUp( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;
            virtual void MoveDown( std::set< size_t >& selectedIndices ) HELIUM_OVERRIDE;

            virtual bool Set(const Serializer* src, uint32_t flags = 0) HELIUM_OVERRIDE;
            virtual bool Equals(const Serializer* s) const HELIUM_OVERRIDE;

            virtual void Serialize(Archive& archive) const HELIUM_OVERRIDE;
            virtual void Deserialize(Archive& archive) HELIUM_OVERRIDE;

            virtual tostream& operator>> (tostream& stream) const HELIUM_OVERRIDE;
            virtual tistream& operator<< (tistream& stream) HELIUM_OVERRIDE;
        };

        typedef SimpleArraySerializer<tstring> StringArraySerializer;                   REFLECT_SPECIALIZE_SERIALIZER( StringArraySerializer );
        typedef SimpleArraySerializer<bool> BoolArraySerializer;                        REFLECT_SPECIALIZE_SERIALIZER( BoolArraySerializer );
        typedef SimpleArraySerializer<uint8_t> U8ArraySerializer;                            REFLECT_SPECIALIZE_SERIALIZER( U8ArraySerializer );
        typedef SimpleArraySerializer<int8_t> I8ArraySerializer;                            REFLECT_SPECIALIZE_SERIALIZER( I8ArraySerializer );
        typedef SimpleArraySerializer<uint16_t> U16ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( U16ArraySerializer );
        typedef SimpleArraySerializer<int16_t> I16ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( I16ArraySerializer );
        typedef SimpleArraySerializer<uint32_t> U32ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( U32ArraySerializer );
        typedef SimpleArraySerializer<int32_t> I32ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( I32ArraySerializer );
        typedef SimpleArraySerializer<uint64_t> U64ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( U64ArraySerializer );
        typedef SimpleArraySerializer<int64_t> I64ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( I64ArraySerializer );
        typedef SimpleArraySerializer<float32_t> F32ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( F32ArraySerializer );
        typedef SimpleArraySerializer<float64_t> F64ArraySerializer;                          REFLECT_SPECIALIZE_SERIALIZER( F64ArraySerializer );
        typedef SimpleArraySerializer<Helium::GUID> GUIDArraySerializer;             REFLECT_SPECIALIZE_SERIALIZER( GUIDArraySerializer );
        typedef SimpleArraySerializer<Helium::TUID> TUIDArraySerializer;             REFLECT_SPECIALIZE_SERIALIZER( TUIDArraySerializer );
        typedef SimpleArraySerializer<Helium::Path> PathArraySerializer;             REFLECT_SPECIALIZE_SERIALIZER( PathArraySerializer );

        typedef SimpleArraySerializer<Vector2> Vector2ArraySerializer;            REFLECT_SPECIALIZE_SERIALIZER( Vector2ArraySerializer );
        typedef SimpleArraySerializer<Vector3> Vector3ArraySerializer;            REFLECT_SPECIALIZE_SERIALIZER( Vector3ArraySerializer );
        typedef SimpleArraySerializer<Vector4> Vector4ArraySerializer;            REFLECT_SPECIALIZE_SERIALIZER( Vector4ArraySerializer );
        typedef SimpleArraySerializer<Matrix3> Matrix3ArraySerializer;            REFLECT_SPECIALIZE_SERIALIZER( Matrix3ArraySerializer );
        typedef SimpleArraySerializer<Matrix4> Matrix4ArraySerializer;            REFLECT_SPECIALIZE_SERIALIZER( Matrix4ArraySerializer );

        typedef SimpleArraySerializer<Color3> Color3ArraySerializer;              REFLECT_SPECIALIZE_SERIALIZER( Color3ArraySerializer );
        typedef SimpleArraySerializer<Color4> Color4ArraySerializer;              REFLECT_SPECIALIZE_SERIALIZER( Color4ArraySerializer );
        typedef SimpleArraySerializer<HDRColor3> HDRColor3ArraySerializer;        REFLECT_SPECIALIZE_SERIALIZER( HDRColor3ArraySerializer );
        typedef SimpleArraySerializer<HDRColor4> HDRColor4ArraySerializer;        REFLECT_SPECIALIZE_SERIALIZER( HDRColor4ArraySerializer );
    }
}