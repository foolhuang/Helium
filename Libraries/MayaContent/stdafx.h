#pragma once

// windows
#include "Windows/Windows.h"

// stdlib
#include <map>
#include <string>

// maya
#include <maya/M3dView.h>
#include <maya/MAnimControl.h>
#include <maya/MArgList.h>
#include <maya/MBoundingBox.h>
#include <maya/MDGContext.h>
#include <maya/MDagModifier.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MFileIO.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnLight.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxCommand.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxTransform.h>
#include <maya/MQuaternion.h>
#include <maya/MSelectionList.h>
#include <maya/MStatus.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>