#include "InteractFunctionsClass.h"
#include "InteractComponent.h"
using namespace std;
using namespace std::placeholders;

//If Function have one actor (also if have more)
FFunctionActorData UInteractFunctionsClass::GetFunctionActorData(FFunctionData functionData) {
	if (functionData.enumName == EActionList::objectInspector) {
		return functionData.objectInspector.actor;
	}
	else if (functionData.enumName == EActionList::curveLineTrace) {
		return functionData.curveLineTrace.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractFunctionsClass::GetFunctionActorDataWrapper(FFunctionDataWrapper functionData) {
	if (functionData.enumName == EActionList::objectInspector) {
		return functionData.objectInspector.actor;
	}
	else if (functionData.enumName == EActionList::curveLineTrace) {
		return functionData.curveLineTrace.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractFunctionsClass::GetFunctionActorDataBase(FFunctionDataBase functionData) {
	if (functionData.enumName == EActionList::objectInspector) {
		return functionData.objectInspector.actor;
	}
	else if (functionData.enumName == EActionList::curveLineTrace) {
		return functionData.curveLineTrace.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractFunctionsClass::GetFunctionActorDataCheck(FFunctionCheckDataBase functionData) {
	
	return {};
	
}


//If Function have other actor
FFunctionActorData UInteractFunctionsClass::GetFunctionCurrentActorData(FFunctionData functionData)
{
	return {};
}

FFunctionActorData UInteractFunctionsClass::GetFunctionCurrentActorDataWrapper(FFunctionDataWrapper functionData)
{
	return {};
}

FFunctionActorData UInteractFunctionsClass::GetFunctionCurrentActorDataBase(FFunctionDataBase functionData)
{
	return {};
}

FFunctionActorData UInteractFunctionsClass::GetFunctionCurrentActorDataCheck(FFunctionCheckDataBase functionData)
{
	return {};
}

void UInteractFunctionsClass::CallDelegateFunction(EActionList tempEnumNameCore, AActor* actor, FFunctionDataBase function, UInteractComponent* interactComponent)
{
	if (EActionList::objectInspector == tempEnumNameCore) {
	UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.objectInspector), function.objectInspector.customVariableSet, FObjectInspector::StaticStruct(), interactComponent);
		UInteractComponent::ObjectInspector(actor, function.objectInspector, interactComponent);
	}
	else if (EActionList::curveLineTrace == tempEnumNameCore) {
	UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.curveLineTrace), function.curveLineTrace.customVariableSet, FCurveLineTrace::StaticStruct(), interactComponent);
		UInteractComponent::CurveLineTrace(actor, function.curveLineTrace, interactComponent);
	}
}

//ADD CREATE FUNCTION WITH FUNCTION NAME, CLASS WHERE THE FUNCTION IS, STRUCT NAME
CREATE_FUNCTION(ObjectInspector, UInteractComponent, FObjectInspector, objectInspector)
CREATE_FUNCTION(CurveLineTrace, UInteractComponent, FCurveLineTrace, curveLineTrace)

CREATE_FUNCTIONCHECK(PlaceholderCheck, UInteractComponent, FPlaceholderCheck, placeholderCheck)

UInteractFunctionsClass::UInteractFunctionsClass()
{
	PrimaryComponentTick.bCanEverTick = false;


	BINDACTIONSFINAL

	BINDCHECKSFINAL

	BINDACTORSFINAL

	BINDENUMSFINAL


	getActorFunctions.Add(EActionList::objectInspector);
	getActorFunctions.Add(EActionList::curveLineTrace);

	if (actorBaseFunctionName.Num() <= 0) {
		actorBaseFunctionName.Add("executeAction");
		actorBaseFunctionName.Add("externalInteract");
		actorBaseFunctionName.Add("playAnimCharacter");
		actorBaseFunctionName.Add("teleportTo");
		actorBaseFunctionName.Add("rotateLookAtActor");
		actorBaseFunctionName.Add("setVisibilityActor");
		actorBaseFunctionName.Add("setTransformActor");
		actorBaseFunctionName.Add("destroyEntity");
		actorBaseFunctionName.Add("focusCamera");
		actorBaseFunctionName.Add("moveToFunction");
		actorBaseFunctionName.Add("rotateToFunction");
		actorBaseFunctionName.Add("spawnerFunction");
		actorBaseFunctionName.Add("updateCamera");
		actorBaseFunctionName.Add("setRagdoll");
		actorBaseFunctionName.Add("showHideTooltip");
		actorBaseFunctionName.Add("addImpulseFunction");
		actorBaseFunctionName.Add("setCollisionSettings");
		actorBaseFunctionName.Add("slowTimeFunction");
		actorBaseFunctionName.Add("attachToObject");
		actorBaseFunctionName.Add("playSoundFunction");
		actorBaseFunctionName.Add("objectInspector");
		actorBaseFunctionName.Add("changePriority");
		actorBaseFunctionName.Add("executeTrigger");
		actorBaseFunctionName.Add("setPhysicsSettings");
		actorBaseFunctionName.Add("addTagFunction");
		actorBaseFunctionName.Add("setMaterialMesh");
		actorBaseFunctionName.Add("saveActorFunction");
		actorBaseFunctionName.Add("createEditVariable");
		actorBaseFunctionName.Add("getExternalVariable");
		actorBaseFunctionName.Add("setTooltipLocationSize");
		actorBaseFunctionName.Add("relocateFunction");
		actorBaseFunctionName.Add("showHideOutlineShine");
		actorBaseFunctionName.Add("curveLineTrace");
		actorBaseFunctionName.Add("getInternalVariable");
		actorBaseFunctionName.Add("setInternalVariable");
		actorBaseFunctionName.Add("executeInternalFunction");
		actorBaseFunctionName.Add("callExternalDelegateFunction");
		actorBaseFunctionName.Add("whenFunction");
	}

	if (actorCheckBaseFunctionName.Num() <= 0) {
		actorCheckBaseFunctionName.Add("placeholderCheck");
	}
}

void UInteractFunctionsClass::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInteractFunctionsClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

