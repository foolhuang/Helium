#include "AttributeInit.h" 

#include "Attribute.h"
#include "AttributeCollection.h"

#include "AttributeCategory.h"
#include "AttributeCategories.h"

#include "Common/InitializerStack.h"
#include "Reflect/Registry.h"

using namespace Attribute;

i32 g_InitCount = 0; 
Nocturnal::InitializerStack g_RegisteredTypes;

Nocturnal::InitializerStack g_InitializerStack;

void Attribute::Initialize()
{
  if (++g_InitCount == 1)
  {
    g_InitializerStack.Push( &Reflect::Initialize, &Reflect::Cleanup );

    g_RegisteredTypes.Push( Reflect::RegisterClass<ComponentCollection>( "ComponentCollection" ) );

    g_RegisteredTypes.Push( Reflect::RegisterClass<AttributeBase>( "AttributeBase" ) );
    g_RegisteredTypes.Push( Reflect::RegisterClass<AttributeCollection>( "AttributeCollection" ) );
    g_RegisteredTypes.Push( Reflect::RegisterClass<AttributeCategory>( "AttributeCategory" ) );

    AttributeCategories::Initialize();
  }
}

void Attribute::Cleanup()
{
  if (--g_InitCount == 0)
  {
    AttributeCategories::Cleanup();
    g_RegisteredTypes.Cleanup();
    g_InitializerStack.Cleanup();
  }
}

REFLECT_DEFINE_ENTRY_POINTS(Attribute::Initialize, Attribute::Cleanup); 