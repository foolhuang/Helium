//----------------------------------------------------------------------------------------------------------------------
// GraphicsJobsInterface.inl
//
// Copyright (C) 2012 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

namespace Helium
{

/// Constructor.
UpdateGraphicsSceneConstantBuffersJobSpawner::UpdateGraphicsSceneConstantBuffersJobSpawner()
{
}

/// Destructor.
UpdateGraphicsSceneConstantBuffersJobSpawner::~UpdateGraphicsSceneConstantBuffersJobSpawner()
{
}

/// Get the parameters for this job.
///
/// @return  Reference to the structure containing the job parameters.
///
/// @see SetParameters()
UpdateGraphicsSceneConstantBuffersJobSpawner::Parameters& UpdateGraphicsSceneConstantBuffersJobSpawner::GetParameters()
{
    return m_parameters;
}

/// Get the parameters for this job.
///
/// @return  Constant reference to the structure containing the job parameters.
///
/// @see SetParameters()
const UpdateGraphicsSceneConstantBuffersJobSpawner::Parameters& UpdateGraphicsSceneConstantBuffersJobSpawner::GetParameters() const
{
    return m_parameters;
}

/// Set the job parameters.
///
/// @param[in] rParameters  Structure containing the job parameters.
///
/// @see GetParameters()
void UpdateGraphicsSceneConstantBuffersJobSpawner::SetParameters( const Parameters& rParameters )
{
    m_parameters = rParameters;
}

/// Callback executed to run the job.
///
/// @param[in] pJob      Job to run.
/// @param[in] pContext  Context associated with the running job instance.
void UpdateGraphicsSceneConstantBuffersJobSpawner::RunCallback( void* pJob, JobContext* pContext )
{
    HELIUM_ASSERT( pJob );
    HELIUM_ASSERT( pContext );
    static_cast< UpdateGraphicsSceneConstantBuffersJobSpawner* >( pJob )->Run( pContext );
}

/// Constructor.
UpdateGraphicsSceneConstantBuffersJobSpawner::Parameters::Parameters()
{
}

/// Constructor.
UpdateGraphicsSceneObjectBuffersJobSpawner::UpdateGraphicsSceneObjectBuffersJobSpawner()
{
}

/// Destructor.
UpdateGraphicsSceneObjectBuffersJobSpawner::~UpdateGraphicsSceneObjectBuffersJobSpawner()
{
}

/// Get the parameters for this job.
///
/// @return  Reference to the structure containing the job parameters.
///
/// @see SetParameters()
UpdateGraphicsSceneObjectBuffersJobSpawner::Parameters& UpdateGraphicsSceneObjectBuffersJobSpawner::GetParameters()
{
    return m_parameters;
}

/// Get the parameters for this job.
///
/// @return  Constant reference to the structure containing the job parameters.
///
/// @see SetParameters()
const UpdateGraphicsSceneObjectBuffersJobSpawner::Parameters& UpdateGraphicsSceneObjectBuffersJobSpawner::GetParameters() const
{
    return m_parameters;
}

/// Set the job parameters.
///
/// @param[in] rParameters  Structure containing the job parameters.
///
/// @see GetParameters()
void UpdateGraphicsSceneObjectBuffersJobSpawner::SetParameters( const Parameters& rParameters )
{
    m_parameters = rParameters;
}

/// Callback executed to run the job.
///
/// @param[in] pJob      Job to run.
/// @param[in] pContext  Context associated with the running job instance.
void UpdateGraphicsSceneObjectBuffersJobSpawner::RunCallback( void* pJob, JobContext* pContext )
{
    HELIUM_ASSERT( pJob );
    HELIUM_ASSERT( pContext );
    static_cast< UpdateGraphicsSceneObjectBuffersJobSpawner* >( pJob )->Run( pContext );
}

/// Constructor.
UpdateGraphicsSceneObjectBuffersJobSpawner::Parameters::Parameters()
{
}

/// Constructor.
UpdateGraphicsSceneSubMeshBuffersJobSpawner::UpdateGraphicsSceneSubMeshBuffersJobSpawner()
{
}

/// Destructor.
UpdateGraphicsSceneSubMeshBuffersJobSpawner::~UpdateGraphicsSceneSubMeshBuffersJobSpawner()
{
}

/// Get the parameters for this job.
///
/// @return  Reference to the structure containing the job parameters.
///
/// @see SetParameters()
UpdateGraphicsSceneSubMeshBuffersJobSpawner::Parameters& UpdateGraphicsSceneSubMeshBuffersJobSpawner::GetParameters()
{
    return m_parameters;
}

/// Get the parameters for this job.
///
/// @return  Constant reference to the structure containing the job parameters.
///
/// @see SetParameters()
const UpdateGraphicsSceneSubMeshBuffersJobSpawner::Parameters& UpdateGraphicsSceneSubMeshBuffersJobSpawner::GetParameters() const
{
    return m_parameters;
}

/// Set the job parameters.
///
/// @param[in] rParameters  Structure containing the job parameters.
///
/// @see GetParameters()
void UpdateGraphicsSceneSubMeshBuffersJobSpawner::SetParameters( const Parameters& rParameters )
{
    m_parameters = rParameters;
}

/// Callback executed to run the job.
///
/// @param[in] pJob      Job to run.
/// @param[in] pContext  Context associated with the running job instance.
void UpdateGraphicsSceneSubMeshBuffersJobSpawner::RunCallback( void* pJob, JobContext* pContext )
{
    HELIUM_ASSERT( pJob );
    HELIUM_ASSERT( pContext );
    static_cast< UpdateGraphicsSceneSubMeshBuffersJobSpawner* >( pJob )->Run( pContext );
}

/// Constructor.
UpdateGraphicsSceneSubMeshBuffersJobSpawner::Parameters::Parameters()
{
}

/// Constructor.
UpdateGraphicsSceneObjectBuffersJob::UpdateGraphicsSceneObjectBuffersJob()
{
}

/// Destructor.
UpdateGraphicsSceneObjectBuffersJob::~UpdateGraphicsSceneObjectBuffersJob()
{
}

/// Get the parameters for this job.
///
/// @return  Reference to the structure containing the job parameters.
///
/// @see SetParameters()
UpdateGraphicsSceneObjectBuffersJob::Parameters& UpdateGraphicsSceneObjectBuffersJob::GetParameters()
{
    return m_parameters;
}

/// Get the parameters for this job.
///
/// @return  Constant reference to the structure containing the job parameters.
///
/// @see SetParameters()
const UpdateGraphicsSceneObjectBuffersJob::Parameters& UpdateGraphicsSceneObjectBuffersJob::GetParameters() const
{
    return m_parameters;
}

/// Set the job parameters.
///
/// @param[in] rParameters  Structure containing the job parameters.
///
/// @see GetParameters()
void UpdateGraphicsSceneObjectBuffersJob::SetParameters( const Parameters& rParameters )
{
    m_parameters = rParameters;
}

/// Callback executed to run the job.
///
/// @param[in] pJob      Job to run.
/// @param[in] pContext  Context associated with the running job instance.
void UpdateGraphicsSceneObjectBuffersJob::RunCallback( void* pJob, JobContext* pContext )
{
    HELIUM_ASSERT( pJob );
    HELIUM_ASSERT( pContext );
    static_cast< UpdateGraphicsSceneObjectBuffersJob* >( pJob )->Run( pContext );
}

/// Constructor.
UpdateGraphicsSceneObjectBuffersJob::Parameters::Parameters()
{
}

/// Constructor.
UpdateGraphicsSceneSubMeshBuffersJob::UpdateGraphicsSceneSubMeshBuffersJob()
{
}

/// Destructor.
UpdateGraphicsSceneSubMeshBuffersJob::~UpdateGraphicsSceneSubMeshBuffersJob()
{
}

/// Get the parameters for this job.
///
/// @return  Reference to the structure containing the job parameters.
///
/// @see SetParameters()
UpdateGraphicsSceneSubMeshBuffersJob::Parameters& UpdateGraphicsSceneSubMeshBuffersJob::GetParameters()
{
    return m_parameters;
}

/// Get the parameters for this job.
///
/// @return  Constant reference to the structure containing the job parameters.
///
/// @see SetParameters()
const UpdateGraphicsSceneSubMeshBuffersJob::Parameters& UpdateGraphicsSceneSubMeshBuffersJob::GetParameters() const
{
    return m_parameters;
}

/// Set the job parameters.
///
/// @param[in] rParameters  Structure containing the job parameters.
///
/// @see GetParameters()
void UpdateGraphicsSceneSubMeshBuffersJob::SetParameters( const Parameters& rParameters )
{
    m_parameters = rParameters;
}

/// Callback executed to run the job.
///
/// @param[in] pJob      Job to run.
/// @param[in] pContext  Context associated with the running job instance.
void UpdateGraphicsSceneSubMeshBuffersJob::RunCallback( void* pJob, JobContext* pContext )
{
    HELIUM_ASSERT( pJob );
    HELIUM_ASSERT( pContext );
    static_cast< UpdateGraphicsSceneSubMeshBuffersJob* >( pJob )->Run( pContext );
}

/// Constructor.
UpdateGraphicsSceneSubMeshBuffersJob::Parameters::Parameters()
{
}

}  // namespace Helium

