#include "FrameworkPch.h"
#include "Framework/World.h"

#include "Rendering/Renderer.h"
#include "Rendering/RSurface.h"
#include "Graphics/GraphicsScene.h"
#include "Graphics/RenderResourceManager.h"
#include "Framework/Entity.h"
#include "Framework/Layer.h"

namespace Helium
{
    class World;
    typedef Helium::StrongPtr< World > WorldPtr;
    typedef Helium::StrongPtr< const World > ConstWorldPtr;
}

HELIUM_IMPLEMENT_OBJECT( Helium::World, Framework, 0 );

using namespace Helium;

/// Constructor.
World::World()
{
}

/// Destructor.
World::~World()
{
    HELIUM_ASSERT( m_layers.IsEmpty() );
    HELIUM_ASSERT( !m_spGraphicsScene );
}

/// Initialize this world instance.
///
/// @return  True if initialization was successful, false if not.
///
/// @see Shutdown()
bool World::Initialize()
{
    HELIUM_ASSERT( m_layers.IsEmpty() );
    HELIUM_ASSERT( !m_spGraphicsScene );

    // Create the main graphics scene.
    const GameObjectType* pSceneType = GraphicsScene::GetStaticType();
    HELIUM_ASSERT( pSceneType );
    bool bCreateResult = GameObject::Create< GraphicsScene >( m_spGraphicsScene, pSceneType->GetName(), this );
    HELIUM_ASSERT( bCreateResult );
    if( !bCreateResult )
    {
        HELIUM_TRACE( TRACE_ERROR, TXT( "World::Initialize(): Failed to create a primary graphics scene.\n" ) );

        return false;
    }

    HELIUM_ASSERT( m_spGraphicsScene );

    return true;
}

/// Shut down this world instance.
///
/// @see Initialize()
void World::Shutdown()
{
    // Remove all layers first.
    while( !m_layers.IsEmpty() )
    {
        Layer* pLayer = m_layers.GetLast();
        HELIUM_ASSERT( pLayer );
        HELIUM_VERIFY( RemoveLayer( pLayer ) );
    }

    // Release the graphics scene for the world.
    m_spGraphicsScene.Release();
}

/// Update the graphics scene for this world for the current frame.
void World::UpdateGraphicsScene()
{
    HELIUM_ASSERT( m_spGraphicsScene );

    m_spGraphicsScene->Update();
}

/// @copydoc GameObject::PreDestroy()
void World::PreDestroy()
{
    Shutdown();

    Base::PreDestroy();
}

/// Create an entity in this world.
///
/// @param[in] pLayer                Layer in which to create the entity.
/// @param[in] pType                 Entity type.
/// @param[in] rPosition             Entity position.
/// @param[in] rRotation             Entity rotation.
/// @param[in] rScale                Entity scale.
/// @param[in] pTemplate             Template from which to create the entity.
/// @param[in] name                  GameObject name to assign to the entity, or a null name to automatically generate a
///                                  name based on the entity type.
/// @param[in] bAssignInstanceIndex  True to assign an instance index to the entity, false to not include an
///                                  instance index.
///
/// @return  Pointer to the entity instance if created successfully, null if not.
///
/// @see DestroyEntity()
Entity* World::CreateEntity(
    Layer* pLayer,
    const GameObjectType* pType,
    const Simd::Vector3& rPosition,
    const Simd::Quat& rRotation,
    const Simd::Vector3& rScale,
    Entity* pTemplate,
    Name name,
    bool bAssignInstanceIndex )
{
    // Make sure the destination layer is valid.
    HELIUM_ASSERT( pLayer );
    if( !pLayer )
    {
        HELIUM_TRACE( TRACE_ERROR, TXT( "World::CreateEntity(): Missing entity layer.\n" ) );

        return NULL;
    }

    World* pLayerWorld = pLayer->GetWorld();
    HELIUM_ASSERT( pLayerWorld == this );
    if( pLayerWorld != this )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::CreateEntity(): Layer \"%s\" is not bound to world \"%s\".\n" ),
            *pLayer->GetPath().ToString(),
            *GetPath().ToString() );

        return NULL;
    }

    // Attempt to create the entity.
    Entity* pEntity = pLayer->CreateEntity(
        pType,
        rPosition,
        rRotation,
        rScale,
        pTemplate,
        name,
        bAssignInstanceIndex );
    if( !pEntity )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::CreateEntity(): Failed to create entity in world \"%s\", layer \"%s\".\n" ),
            *GetPath().ToString(),
            *pLayer->GetPath().ToString() );

        return NULL;
    }

    // Attach the entity to the world.
    pEntity->Attach();

    return pEntity;
}

/// Destroy an entity in this world.
///
/// @param[in] pEntity  Entity to destroy.
///
/// @return  True if the entity was destroyed successfully, false if not.
///
/// @see CreateEntity()
bool World::DestroyEntity( Entity* pEntity )
{
    HELIUM_ASSERT( pEntity );

    // Get the entity layer and make sure the entity is part of this world.
    LayerPtr spEntityLayer( pEntity->GetLayer() );
    if( !spEntityLayer )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::DestroyEntity(): Entity \"%s\" is not bound to a layer.\n" ),
            *pEntity->GetPath().ToString() );

        return false;
    }

    WorldPtr spEntityWorld( spEntityLayer->GetWorld() );
    if( spEntityWorld.Get() != this )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::DestroyEntity(): Entity \"%s\" is not part of world \"%s\".\n" ),
            *pEntity->GetPath().ToString() );

        return false;
    }

    // Detach the entity from this world and destroy it.
    pEntity->Detach();

    bool bDestroyResult = spEntityLayer->DestroyEntity( pEntity );

    return bDestroyResult;
}

/// Add a layer to this world.
///
/// @param[in] pLayer  Layer to add.
///
/// @return  True if the layer was added successfully, false if not.
///
/// @see RemoveLayer()
bool World::AddLayer( Layer* pLayer )
{
    // Make sure a valid layer not already attached to a world was specified.
    HELIUM_ASSERT( pLayer );
    if( !pLayer )
    {
        HELIUM_TRACE( TRACE_ERROR, TXT( "World::AddLayer(): Null layer specified.\n" ) );

        return false;
    }

    World* pExistingWorld = pLayer->GetWorld();
    HELIUM_ASSERT( !pExistingWorld );
    if( pExistingWorld )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::AddLayer(): Layer \"%s\" is already bound to world \"%s\".\n" ),
            *pLayer->GetPath().ToString(),
            *pExistingWorld->GetPath().ToString() );

        return false;
    }

    // Add the layer to our layer list and set it referencing back to this world.
    size_t layerIndex = m_layers.Push( LayerPtr( pLayer ) );
    HELIUM_ASSERT( IsValid( layerIndex ) );
    pLayer->SetWorldInfo( this, layerIndex );

    // Attach all entities in the layer.
    size_t entityCount = pLayer->GetEntityCount();
    for( size_t entityIndex = 0; entityIndex < entityCount; ++entityIndex )
    {
        Entity* pEntity = pLayer->GetEntity( entityIndex );
        HELIUM_ASSERT( pEntity );
        pEntity->Attach();
    }

    return true;
}

/// Remove a layer from this world.
///
/// @param[in] pLayer  Layer to remove.
///
/// @return  True if the layer was removed successfully, false if not.
///
/// @see AddLayer()
bool World::RemoveLayer( Layer* pLayer )
{
    HELIUM_ASSERT( pLayer );

    // Make sure the layer is part of this world.
    if( pLayer->GetWorld().Get() != this )
    {
        HELIUM_TRACE(
            TRACE_ERROR,
            TXT( "World::RemoveLayer(): Layer \"%s\" is not part of world \"%s\".\n" ),
            *pLayer->GetPath().ToString(),
            *GetPath().ToString() );

        return false;
    }

    // Detach all entities in the layer.
    size_t entityCount = pLayer->GetEntityCount();
    for( size_t entityIndex = 0; entityIndex < entityCount; ++entityIndex )
    {
        Entity* pEntity = pLayer->GetEntity( entityIndex );
        HELIUM_ASSERT( pEntity );
        pEntity->Detach();
    }

    // Remove the layer from the layer list and clear out all references back to this world.
    size_t index = pLayer->GetWorldIndex();
    HELIUM_ASSERT( index < m_layers.GetSize() );

    pLayer->ClearWorldInfo();
    m_layers.RemoveSwap( index );

    // Update the index of the layer which has been moved to fill the layer list entry we just removed.
    size_t layerCount = m_layers.GetSize();
    if( index < layerCount )
    {
        Layer* pMovedLayer = m_layers[ index ];
        HELIUM_ASSERT( pMovedLayer );
        HELIUM_ASSERT( pMovedLayer->GetWorldIndex() == layerCount );
        pMovedLayer->SetWorldIndex( index );
    }

    return true;
}

/// Get the layer associated with the given index in this world.
///
/// @param[in] index  Layer index.
///
/// @return  Layer instance.
///
/// @see GetLayerCount()
Layer* World::GetLayer( size_t index ) const
{
    HELIUM_ASSERT( index < m_layers.GetSize() );

    return m_layers[ index ];
}

/// Get the graphics scene for this world instance.
///
/// @return  Graphics scene instance.
GraphicsScene* World::GetGraphicsScene() const
{
    return m_spGraphicsScene;
}
