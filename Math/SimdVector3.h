#pragma once

#include "Math/API.h"
#include "Foundation/Math.h"
#include "Math/Simd.h"
#include "Platform/Utility.h"
#include "Reflect/Structure.h"

namespace Helium
{
    namespace Simd
    {
        /// Single-precision floating-point 3-component vector.  Note that this is padded to 16 bytes (4 components) for
        /// SIMD support.
        HELIUM_ALIGN_PRE( 16 ) class HELIUM_MATH_API Vector3
        {
        public:
            REFLECT_DECLARE_BASE_STRUCTURE(Vector3);
            static void PopulateComposite( Reflect::Composite& comp );

            /// @name Construction/Destruction
            //@{
            inline Vector3();
            inline Vector3( float32_t x, float32_t y, float32_t z );
            inline explicit Vector3( const float32_t* pVector );
            inline explicit Vector3( float32_t s );
#if HELIUM_SIMD_SIZE == 16
            inline explicit Vector3( const Register& rVector );
#endif
            //@}

            /// @name Data Access
            //@{
#if HELIUM_SIMD_SIZE == 16
            inline Register& GetSimdVector();
            inline const Register& GetSimdVector() const;
            inline void SetSimdVector( const Register& rVector );
#endif

            inline float32_t& GetElement( size_t index );
            inline float32_t GetElement( size_t index ) const;
            inline void SetElement( size_t index, float32_t value );
            //@}

            /// @name Math
            //@{
            inline Vector3 Add( const Vector3& rVector ) const;
            inline Vector3 Subtract( const Vector3& rVector ) const;
            inline Vector3 Multiply( const Vector3& rVector ) const;
            inline Vector3 Divide( const Vector3& rVector ) const;

            inline void AddSet( const Vector3& rVector0, const Vector3& rVector1 );
            inline void SubtractSet( const Vector3& rVector0, const Vector3& rVector1 );
            inline void MultiplySet( const Vector3& rVector0, const Vector3& rVector1 );
            inline void DivideSet( const Vector3& rVector0, const Vector3& rVector1 );

            inline void MultiplyAddSet( const Vector3& rVectorMul0, const Vector3& rVectorMul1, const Vector3& rVectorAdd );

            inline Vector3 GetScaled( float32_t scale ) const;
            inline void Scale( float32_t scale );

            inline float32_t Dot( const Vector3& rVector ) const;

            inline Vector3 Cross( const Vector3& rVector ) const;
            inline void CrossSet( const Vector3& rVector0, const Vector3& rVector1 );

            inline float32_t GetMagnitude() const;
            inline float32_t GetMagnitudeSquared() const;

            inline Vector3 GetNormalized( float32_t epsilon = HELIUM_EPSILON ) const;
            inline void Normalize( float32_t epsilon = HELIUM_EPSILON );

            inline Vector3 GetNegated() const;
            inline void GetNegated( Vector3& rResult ) const;
            inline void Negate();
            //@}

            /// @name Comparison
            //@{
            inline bool Equals( const Vector3& rVector, float32_t epsilon = HELIUM_EPSILON ) const;
            //@}

            /// @name Overloaded Operators
            //@{
            inline Vector3 operator-() const;

            inline Vector3 operator+( const Vector3& rVector ) const;
            inline Vector3 operator-( const Vector3& rVector ) const;
            inline Vector3 operator*( const Vector3& rVector ) const;
            inline Vector3 operator/( const Vector3& rVector ) const;

            inline Vector3& operator+=( const Vector3& rVector );
            inline Vector3& operator-=( const Vector3& rVector );
            inline Vector3& operator*=( const Vector3& rVector );
            inline Vector3& operator/=( const Vector3& rVector );

            inline Vector3 operator*( float32_t scale ) const;
            inline Vector3& operator*=( float32_t scale );

            inline bool operator==( const Vector3& rVector ) const;
            inline bool operator!=( const Vector3& rVector ) const;
            //@}

            /// @name Friend Functions
            //@{
            inline friend Vector3 operator*( float32_t scale, const Vector3& rVector );
            //@}

        private:

            union
            {
                
#if HELIUM_SIMD_SIZE == 16
                /// SIMD vector containing the vector values.
                Register m_vector;
#else
                /// Vector values.
                float32_t m_vector[ 4 ];
#endif
                /// Vector values.
                float32_t m_vectorAsFloatArray[ 4 ];
                struct 
                {
                    float32_t m_x;
                    float32_t m_y;
                    float32_t m_z;
                    float32_t m_w;
                };
            };

        } HELIUM_ALIGN_POST( 16 );
    }
}

#include "Math/SimdVector3.inl"

#if HELIUM_SIMD_SSE
#include "Math/SimdVector3Sse.inl"
#endif
