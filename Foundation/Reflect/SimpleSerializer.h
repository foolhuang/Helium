#pragma once

#include "Serializer.h"
#include "Foundation/Math/Vector2.h"
#include "Foundation/Math/Vector3.h"
#include "Foundation/Math/Vector4.h"
#include "Foundation/Math/Matrix3.h"
#include "Foundation/Math/Matrix4.h"
#include "Foundation/Math/Color3.h"
#include "Foundation/Math/Color4.h"
#include "Foundation/Math/HDRColor3.h"
#include "Foundation/Math/HDRColor4.h"
#include "Foundation/GUID.h"
#include "Foundation/TUID.h"

namespace Helium
{
    namespace Reflect
    {
        //
        // SimpleSerializer wraps data and read/writes it from the archive
        //  This template class can be extended to wrap any primitive type that
        //  has constant size and support for iostream insertion and extraction
        //

        template <class T>
        class FOUNDATION_API SimpleSerializer : public Serializer
        {
        public:
            typedef T DataType;
            Serializer::DataPtr<DataType> m_Data;

            typedef SimpleSerializer<DataType> SimpleSerializerT;
            REFLECT_DECLARE_CLASS( SimpleSerializerT, Serializer );

            SimpleSerializer ();
            ~SimpleSerializer();

            virtual bool IsCompact() const HELIUM_OVERRIDE { return true; }

            virtual void ConnectData(Helium::HybridPtr<void> data) HELIUM_OVERRIDE;

            virtual bool Set(const Serializer* src, uint32_t flags = 0) HELIUM_OVERRIDE;
            virtual bool Equals(const Serializer* s) const HELIUM_OVERRIDE;

            virtual void Serialize(const Helium::BasicBufferPtr& buffer, const tchar* debugStr) const HELIUM_OVERRIDE;
            virtual void Serialize(Archive& archive) const HELIUM_OVERRIDE;
            virtual void Deserialize(Archive& archive) HELIUM_OVERRIDE;

            virtual tostream& operator>> (tostream& stream) const HELIUM_OVERRIDE;
            virtual tistream& operator<< (tistream& stream);
        };

        typedef SimpleSerializer<tstring> StringSerializer;                       REFLECT_SPECIALIZE_SERIALIZER( StringSerializer );
        typedef SimpleSerializer<bool> BoolSerializer;                            REFLECT_SPECIALIZE_SERIALIZER( BoolSerializer );
        typedef SimpleSerializer<uint8_t> U8Serializer;                                REFLECT_SPECIALIZE_SERIALIZER( U8Serializer );
        typedef SimpleSerializer<int8_t> I8Serializer;                                REFLECT_SPECIALIZE_SERIALIZER( I8Serializer );
        typedef SimpleSerializer<uint16_t> U16Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( U16Serializer );
        typedef SimpleSerializer<int16_t> I16Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( I16Serializer );
        typedef SimpleSerializer<uint32_t> U32Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( U32Serializer );
        typedef SimpleSerializer<int32_t> I32Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( I32Serializer );
        typedef SimpleSerializer<uint64_t> U64Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( U64Serializer );
        typedef SimpleSerializer<int64_t> I64Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( I64Serializer );
        typedef SimpleSerializer<float32_t> F32Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( F32Serializer );
        typedef SimpleSerializer<float64_t> F64Serializer;                              REFLECT_SPECIALIZE_SERIALIZER( F64Serializer );
        typedef SimpleSerializer<Helium::GUID> GUIDSerializer;                 REFLECT_SPECIALIZE_SERIALIZER( GUIDSerializer );
        typedef SimpleSerializer<Helium::TUID> TUIDSerializer;                 REFLECT_SPECIALIZE_SERIALIZER( TUIDSerializer );

        typedef SimpleSerializer<Vector2> Vector2Serializer;              REFLECT_SPECIALIZE_SERIALIZER( Vector2Serializer );
        typedef SimpleSerializer<Vector3> Vector3Serializer;              REFLECT_SPECIALIZE_SERIALIZER( Vector3Serializer );
        typedef SimpleSerializer<Vector4> Vector4Serializer;              REFLECT_SPECIALIZE_SERIALIZER( Vector4Serializer );
        typedef SimpleSerializer<Matrix3> Matrix3Serializer;              REFLECT_SPECIALIZE_SERIALIZER( Matrix3Serializer );
        typedef SimpleSerializer<Matrix4> Matrix4Serializer;              REFLECT_SPECIALIZE_SERIALIZER( Matrix4Serializer );

        typedef SimpleSerializer<Color3> Color3Serializer;                REFLECT_SPECIALIZE_SERIALIZER( Color3Serializer );
        typedef SimpleSerializer<Color4> Color4Serializer;                REFLECT_SPECIALIZE_SERIALIZER( Color4Serializer );
        typedef SimpleSerializer<HDRColor3> HDRColor3Serializer;          REFLECT_SPECIALIZE_SERIALIZER( HDRColor3Serializer );
        typedef SimpleSerializer<HDRColor4> HDRColor4Serializer;          REFLECT_SPECIALIZE_SERIALIZER( HDRColor4Serializer );
    }
}