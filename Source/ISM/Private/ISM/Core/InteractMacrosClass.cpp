#include "InteractMacrosClass.h"
#include "InteractComponent.h"


TArray<FString> UInteractMacrosClass::actorBaseFunctionName;
TArray<FString> UInteractMacrosClass::actorCheckBaseFunctionName;
TArray<FString> UInteractMacrosClass::enumFunctions;

FFunctionActorData UInteractMacrosClass::GetFunctionActorDataCore(FFunctionData functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeAction) {
		return functionData.executeAction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::externalInteract) {
		return functionData.externalInteract.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playAnimCharacter) {
		return functionData.playAnimCharacter.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setVisibilityActor) {
		return functionData.setVisibilityActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTransformActor) {
		return functionData.setTransformActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::destroyEntity) {
		return functionData.destroyEntity.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::relocateFunction) {
		return functionData.relocateFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::spawnerFunction) {
		return functionData.spawnerFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setRagdoll) {
		return functionData.setRagdoll.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideTooltip) {
		return functionData.showHideTooltip.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addImpulseFunction) {
		return functionData.addImpulseFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setCollisionSettings) {
		return functionData.setCollisionSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::slowTimeFunction) {
		return functionData.slowTimeFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playSoundFunction) {
		return functionData.playSoundFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::changePriority) {
		return functionData.changePriority.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setPhysicsSettings) {
		return functionData.setPhysicsSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addTagFunction) {
		return functionData.addTagFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setMaterialMesh) {
		return functionData.setMaterialMesh.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::createEditVariable) {
		return functionData.createEditVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getExternalVariable) {
		return functionData.getExternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTooltipLocationSize) {
		return functionData.setTooltipLocationSize.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideOutlineShine) {
		return functionData.showHideOutlineShine.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getInternalVariable) {
		return functionData.getInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setInternalVariable) {
		return functionData.setInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeInternalFunction) {
		return functionData.executeInternalFunction.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionActorDataWrapperCore(FFunctionDataWrapper functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeAction) {
		return functionData.executeAction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::externalInteract) {
		return functionData.externalInteract.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playAnimCharacter) {
		return functionData.playAnimCharacter.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setVisibilityActor) {
		return functionData.setVisibilityActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTransformActor) {
		return functionData.setTransformActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::destroyEntity) {
		return functionData.destroyEntity.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::relocateFunction) {
		return functionData.relocateFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::spawnerFunction) {
		return functionData.spawnerFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setRagdoll) {
		return functionData.setRagdoll.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideTooltip) {
		return functionData.showHideTooltip.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addImpulseFunction) {
		return functionData.addImpulseFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setCollisionSettings) {
		return functionData.setCollisionSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::slowTimeFunction) {
		return functionData.slowTimeFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playSoundFunction) {
		return functionData.playSoundFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::changePriority) {
		return functionData.changePriority.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setPhysicsSettings) {
		return functionData.setPhysicsSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addTagFunction) {
		return functionData.addTagFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setMaterialMesh) {
		return functionData.setMaterialMesh.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::createEditVariable) {
		return functionData.createEditVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getExternalVariable) {
		return functionData.getExternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTooltipLocationSize) {
		return functionData.setTooltipLocationSize.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideOutlineShine) {
		return functionData.showHideOutlineShine.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getInternalVariable) {
		return functionData.getInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setInternalVariable) {
		return functionData.setInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeInternalFunction) {
		return functionData.executeInternalFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::callExternalDelegateFunction) {
		return functionData.callExternalDelegateFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::whenFunction) {
		return functionData.whenFunction.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionActorDataBaseCore(FFunctionDataBase functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeAction) {
		return functionData.executeAction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::externalInteract) {
		return functionData.externalInteract.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playAnimCharacter) {
		return functionData.playAnimCharacter.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setVisibilityActor) {
		return functionData.setVisibilityActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTransformActor) {
		return functionData.setTransformActor.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::destroyEntity) {
		return functionData.destroyEntity.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::relocateFunction) {
		return functionData.relocateFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::spawnerFunction) {
		return functionData.spawnerFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setRagdoll) {
		return functionData.setRagdoll.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideTooltip) {
		return functionData.showHideTooltip.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addImpulseFunction) {
		return functionData.addImpulseFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setCollisionSettings) {
		return functionData.setCollisionSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::slowTimeFunction) {
		return functionData.slowTimeFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::playSoundFunction) {
		return functionData.playSoundFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::changePriority) {
		return functionData.changePriority.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setPhysicsSettings) {
		return functionData.setPhysicsSettings.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::addTagFunction) {
		return functionData.addTagFunction.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setMaterialMesh) {
		return functionData.setMaterialMesh.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::createEditVariable) {
		return functionData.createEditVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getExternalVariable) {
		return functionData.getExternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setTooltipLocationSize) {
		return functionData.setTooltipLocationSize.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::showHideOutlineShine) {
		return functionData.showHideOutlineShine.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::getInternalVariable) {
		return functionData.getInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::setInternalVariable) {
		return functionData.setInternalVariable.actor;
	}
	else if (functionData.enumNameCore == EActionListCore::executeInternalFunction) {
		return functionData.executeInternalFunction.actor;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionActorDataCheckCore(FFunctionCheckDataBase functionData)
{
	if (functionData.enumNameCore == ECheckListCore::checkActorLocation) {
		return functionData.checkActorLocation.actor;
	}
	else if (functionData.enumNameCore == ECheckListCore::checkActorRotation) {
		return functionData.checkActorRotation.actor;
	}
	else if (functionData.enumNameCore == ECheckListCore::checkRaycast) {
		return functionData.checkRaycast.actor;
	}
	else if (functionData.enumNameCore == ECheckListCore::checkIfActorExists) {
		return functionData.checkIfActorExists.actor;
	}
	else {
		return {};
	}
}

//If Function have other actor
FFunctionActorData UInteractMacrosClass::GetFunctionCurrentActorDataCore(FFunctionData functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor2;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionCurrentActorDataWrapperCore(FFunctionDataWrapper functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::whenFunction) {
		return functionData.whenFunction.actor2;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionCurrentActorDataBaseCore(FFunctionDataBase functionData)
{
	if (functionData.enumNameCore == EActionListCore::executeTrigger) {
		return functionData.executeTrigger.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::attachToObject) {
		return functionData.attachToObject.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::moveToFunction) {
		return functionData.moveToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateToFunction) {
		return functionData.rotateToFunction.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::teleportTo) {
		return functionData.teleportTo.actor2;
	}
	else if (functionData.enumNameCore == EActionListCore::rotateLookAtActor) {
		return functionData.rotateLookAtActor.actor2;
	}
	else {
		return {};
	}
}

FFunctionActorData UInteractMacrosClass::GetFunctionCurrentActorDataCheckCore(FFunctionCheckDataBase functionData)
{
	if (functionData.enumNameCore == ECheckListCore::checkActorLocation) {
		return functionData.checkActorLocation.actor2;
	}
	else if (functionData.enumNameCore == ECheckListCore::checkActorRotation) {
		return functionData.checkActorRotation.actor2;
	}
	else {
		return {};
	}
}

void UInteractMacrosClass::CallDelegateCoreFunction(EActionListCore tempEnumNameCore, AActor* actor, FFunctionDataBase function, UInteractComponent* interactComponent)
{
	if (EActionListCore::playSequence == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.playSequence), function.playSequence.customVariableSet, FPlaySequence::StaticStruct(), interactComponent);
		UInteractComponent::PlaySequence(actor, function.playSequence, interactComponent);
	}
	else if (EActionListCore::executeAction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.executeAction), function.executeAction.customVariableSet, FExecuteAction::StaticStruct(), interactComponent);
		UInteractComponent::ExecuteAction(actor, function.executeAction, interactComponent);
	}
	else if (EActionListCore::jumpStage == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.jumpStage), function.jumpStage.customVariableSet, FJumpStage::StaticStruct(), interactComponent);
		UInteractComponent::JumpStage(actor, function.jumpStage, interactComponent);
	}
	else if (EActionListCore::debugText == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.debugText), function.debugText.customVariableSet, FDebugText::StaticStruct(), interactComponent);
		UInteractComponent::DebugText(actor, function.debugText, interactComponent);
	}
	else if (EActionListCore::externalInteract == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.externalInteract), function.externalInteract.customVariableSet, FExternalInteract::StaticStruct(), interactComponent);
		UInteractComponent::ExternalInteract(actor, function.externalInteract, interactComponent);
	}
	else if (EActionListCore::jumpFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.jumpFunction), function.jumpFunction.customVariableSet, FJumpFunction::StaticStruct(), interactComponent);
		UInteractComponent::JumpFunction(actor, function.jumpFunction, interactComponent);
	}
	else if (EActionListCore::cancelJumpStage == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.cancelJumpStage), function.cancelJumpStage.customVariableSet, FCancelJumpStage::StaticStruct(), interactComponent);
		UInteractComponent::CancelJumpStage(actor, function.cancelJumpStage, interactComponent);
	}
	else if (EActionListCore::breakFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.breakFunction), function.breakFunction.customVariableSet, FBreakFunction::StaticStruct(), interactComponent);
		UInteractComponent::BreakFunction(actor, function.breakFunction, interactComponent);
	}
	else if (EActionListCore::teleportTo == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.teleportTo), function.teleportTo.customVariableSet, FTeleportTo::StaticStruct(), interactComponent);
		UInteractComponent::TeleportTo(actor, function.teleportTo, interactComponent);
	}
	else if (EActionListCore::rotateLookAtActor == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.rotateLookAtActor), function.rotateLookAtActor.customVariableSet, FRotateLookAtActor::StaticStruct(), interactComponent);
		UInteractComponent::RotateLookAtActor(actor, function.rotateLookAtActor, interactComponent);
	}
	else if (EActionListCore::playAnimCharacter == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.playAnimCharacter), function.playAnimCharacter.customVariableSet, FPlayAnimCharacter::StaticStruct(), interactComponent);
		UInteractComponent::PlayAnimCharacter(actor, function.playAnimCharacter, interactComponent);
	}
	else if (EActionListCore::setVisibilityActor == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setVisibilityActor), function.setVisibilityActor.customVariableSet, FSetVisibilityActor::StaticStruct(), interactComponent);
		UInteractComponent::SetVisibilityActor(actor, function.setVisibilityActor, interactComponent);
	}
	else if (EActionListCore::setTransformActor == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setTransformActor), function.setTransformActor.customVariableSet, FSetTransformActor::StaticStruct(), interactComponent);
		UInteractComponent::SetTransformActor(actor, function.setTransformActor, interactComponent);
	}
	else if (EActionListCore::destroyEntity == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.destroyEntity), function.destroyEntity.customVariableSet, FDestroyEntity::StaticStruct(), interactComponent);
		UInteractComponent::DestroyEntity(actor, function.destroyEntity, interactComponent);
	}
	else if (EActionListCore::moveToFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.moveToFunction), function.moveToFunction.customVariableSet, FMoveToFunction::StaticStruct(), interactComponent);
		UInteractComponent::MoveToFunction(actor, function.moveToFunction, interactComponent);
	}
	else if (EActionListCore::rotateToFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.rotateToFunction), function.rotateToFunction.customVariableSet, FRotateToFunction::StaticStruct(), interactComponent);
		UInteractComponent::RotateToFunction(actor, function.rotateToFunction, interactComponent);
	}
	else if (EActionListCore::createEditVariable == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.createEditVariable), function.createEditVariable.customVariableSet, FCreateEditVariable::StaticStruct(), interactComponent);
		UInteractComponent::CreateEditVariable(actor, function.createEditVariable, interactComponent);
	}
	else if (EActionListCore::spawnerFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.spawnerFunction), function.spawnerFunction.customVariableSet, FSpawnerFunction::StaticStruct(), interactComponent);
		UInteractComponent::SpawnerFunction(actor, function.spawnerFunction, interactComponent);
	}
	else if (EActionListCore::setRagdoll == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setRagdoll), function.setRagdoll.customVariableSet, FSetRagdoll::StaticStruct(), interactComponent);
		UInteractComponent::SetRagdoll(actor, function.setRagdoll, interactComponent);
	}
	else if (EActionListCore::showHideTooltip == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.showHideTooltip), function.showHideTooltip.customVariableSet, FShowHideTooltip::StaticStruct(), interactComponent);
		UInteractComponent::ShowHideTooltip(actor, function.showHideTooltip, interactComponent);
	}
	else if (EActionListCore::addImpulseFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.addImpulseFunction), function.addImpulseFunction.customVariableSet, FAddImpulseFunction::StaticStruct(), interactComponent);
		UInteractComponent::AddImpulseFunction(actor, function.addImpulseFunction, interactComponent);
	}
	else if (EActionListCore::setCollisionSettings == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setCollisionSettings), function.setCollisionSettings.customVariableSet, FSetCollisionSettings::StaticStruct(), interactComponent);
		UInteractComponent::SetCollisionSettings(actor, function.setCollisionSettings, interactComponent);
	}
	else if (EActionListCore::slowTimeFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.slowTimeFunction), function.slowTimeFunction.customVariableSet, FSlowTimeFunction::StaticStruct(), interactComponent);
		UInteractComponent::SlowTimeFunction(actor, function.slowTimeFunction, interactComponent);
	}
	else if (EActionListCore::attachToObject == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.attachToObject), function.attachToObject.customVariableSet, FAttachToObject::StaticStruct(), interactComponent);
		UInteractComponent::AttachToObject(actor, function.attachToObject, interactComponent);
	}
	else if (EActionListCore::playSoundFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.playSoundFunction), function.playSoundFunction.customVariableSet, FPlaySoundFunction::StaticStruct(), interactComponent);
		UInteractComponent::PlaySoundFunction(actor, function.playSoundFunction, interactComponent);
	}
	else if (EActionListCore::stopSoundFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.stopSoundFunction), function.stopSoundFunction.customVariableSet, FStopSoundFunction::StaticStruct(), interactComponent);
		UInteractComponent::StopSoundFunction(actor, function.stopSoundFunction, interactComponent);
	}
	else if (EActionListCore::modifySoundFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.modifySoundFunction), function.modifySoundFunction.customVariableSet, FModifySoundFunction::StaticStruct(), interactComponent);
		UInteractComponent::ModifySoundFunction(actor, function.modifySoundFunction, interactComponent);
	}
	else if (EActionListCore::cameraEffect == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.cameraEffect), function.cameraEffect.customVariableSet, FCameraEffect::StaticStruct(), interactComponent);
		UInteractComponent::CameraEffect(actor, function.cameraEffect, interactComponent);
	}
	else if (EActionListCore::openLevelFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.openLevelFunction), function.openLevelFunction.customVariableSet, FOpenLevelFunction::StaticStruct(), interactComponent);
		UInteractComponent::OpenLevelFunction(actor, function.openLevelFunction, interactComponent);
	}
	else if (EActionListCore::changePriority == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.changePriority), function.changePriority.customVariableSet, FChangePriority::StaticStruct(), interactComponent);
		UInteractComponent::ChangePriority(actor, function.changePriority, interactComponent);
	}
	else if (EActionListCore::executeTrigger == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.executeTrigger), function.executeTrigger.customVariableSet, FExecuteTrigger::StaticStruct(), interactComponent);
		UInteractComponent::ExecuteTrigger(actor, function.executeTrigger, interactComponent);
	}
	else if (EActionListCore::addWidgetToViewport == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.addWidgetToViewport), function.addWidgetToViewport.customVariableSet, FAddWidgetToViewport::StaticStruct(), interactComponent);
		UInteractComponent::AddWidgetToViewport(actor, function.addWidgetToViewport, interactComponent);
	}
	else if (EActionListCore::removeWidgetFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.removeWidgetFunction), function.removeWidgetFunction.customVariableSet, FRemoveWidgetFunction::StaticStruct(), interactComponent);
		UInteractComponent::RemoveWidgetFunction(actor, function.removeWidgetFunction, interactComponent);
	}
	else if (EActionListCore::setPhysicsSettings == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setPhysicsSettings), function.setPhysicsSettings.customVariableSet, FSetPhysicsSettings::StaticStruct(), interactComponent);
		UInteractComponent::SetPhysicsSettings(actor, function.setPhysicsSettings, interactComponent);
	}
	else if (EActionListCore::addTagFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.addTagFunction), function.addTagFunction.customVariableSet, FAddTagFunction::StaticStruct(), interactComponent);
		UInteractComponent::AddTagFunction(actor, function.addTagFunction, interactComponent);
	}
	else if (EActionListCore::setMaterialMesh == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setMaterialMesh), function.setMaterialMesh.customVariableSet, FSetMaterialMesh::StaticStruct(), interactComponent);
		UInteractComponent::SetMaterialMesh(actor, function.setMaterialMesh, interactComponent);
	}
	else if (EActionListCore::getExternalVariable == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.getExternalVariable), function.getExternalVariable.customVariableSet, FGetExternalVariable::StaticStruct(), interactComponent);
		UInteractComponent::GetExternalVariable(actor, function.getExternalVariable, interactComponent);
	}
	else if (EActionListCore::setTooltipLocationSize == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setTooltipLocationSize), function.setTooltipLocationSize.customVariableSet, FSetTooltipLocationSize::StaticStruct(), interactComponent);
		UInteractComponent::SetTooltipLocationSize(actor, function.setTooltipLocationSize, interactComponent);
	}
	else if (EActionListCore::relocateFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.relocateFunction), function.relocateFunction.customVariableSet, FRelocateFunction::StaticStruct(), interactComponent);
		UInteractComponent::RelocateFunction(actor, function.relocateFunction, interactComponent);
	}
	else if (EActionListCore::playWidgetAnimation == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.playWidgetAnimation), function.playWidgetAnimation.customVariableSet, FPlayWidgetAnimation::StaticStruct(), interactComponent);
		UInteractComponent::PlayWidgetAnimation(actor, function.playWidgetAnimation, interactComponent);
	}
	else if (EActionListCore::showHideOutlineShine == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.showHideOutlineShine), function.showHideOutlineShine.customVariableSet, FShowHideOutlineShine::StaticStruct(), interactComponent);
		UInteractComponent::ShowHideOutlineShine(actor, function.showHideOutlineShine, interactComponent);
	}
	else if (EActionListCore::getInternalVariable == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.getInternalVariable), function.getInternalVariable.customVariableSet, FGetInternalVariable::StaticStruct(), interactComponent);
		UInteractComponent::GetInternalVariable(actor, function.getInternalVariable, interactComponent);
	}
	else if (EActionListCore::execCommand == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.execCommand), function.execCommand.customVariableSet, FExecCommand::StaticStruct(), interactComponent);
		UInteractComponent::ExecCommand(actor, function.execCommand, interactComponent);
	}
	else if (EActionListCore::setInternalVariable == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.setInternalVariable), function.setInternalVariable.customVariableSet, FSetInternalVariable::StaticStruct(), interactComponent);
		UInteractComponent::SetInternalVariable(actor, function.setInternalVariable, interactComponent);
	}
	else if (EActionListCore::executeInternalFunction == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.executeInternalFunction), function.executeInternalFunction.customVariableSet, FExecuteInternalFunction::StaticStruct(), interactComponent);
		UInteractComponent::ExecuteInternalFunction(actor, function.executeInternalFunction, interactComponent);
	}
	else if (EActionListCore::loadStreamLevelInteract == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.loadStreamLevelInteract), function.loadStreamLevelInteract.customVariableSet, FLoadStreamLevelInteract::StaticStruct(), interactComponent);
		UInteractComponent::LoadStreamLevelInteract(actor, function.loadStreamLevelInteract, interactComponent);
	}
	else if (EActionListCore::unloadStreamLevelInteract == tempEnumNameCore) {
		UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&function.unloadStreamLevelInteract), function.unloadStreamLevelInteract.customVariableSet, FUnloadStreamLevelInteract::StaticStruct(), interactComponent);
		UInteractComponent::UnloadStreamLevelInteract(actor, function.unloadStreamLevelInteract, interactComponent);
	}

}


UInteractMacrosClass::UInteractMacrosClass()
{
	PrimaryComponentTick.bCanEverTick = false;


	BINDACTIONSFINALCORE

	BINDCHECKSFINALCORE



	getActorFunctionsCore.Add(EActionListCore::executeAction);
	getActorFunctionsCore.Add(EActionListCore::externalInteract);
	getActorFunctionsCore.Add(EActionListCore::playAnimCharacter);
	getActorFunctionsCore.Add(EActionListCore::teleportTo);
	getActorFunctionsCore.Add(EActionListCore::rotateLookAtActor);
	getActorFunctionsCore.Add(EActionListCore::setVisibilityActor);
	getActorFunctionsCore.Add(EActionListCore::setTransformActor);
	getActorFunctionsCore.Add(EActionListCore::destroyEntity);
	getActorFunctionsCore.Add(EActionListCore::moveToFunction);
	getActorFunctionsCore.Add(EActionListCore::rotateToFunction);
	getActorFunctionsCore.Add(EActionListCore::spawnerFunction);
	getActorFunctionsCore.Add(EActionListCore::setRagdoll);
	getActorFunctionsCore.Add(EActionListCore::showHideTooltip);
	getActorFunctionsCore.Add(EActionListCore::addImpulseFunction);
	getActorFunctionsCore.Add(EActionListCore::setCollisionSettings);
	getActorFunctionsCore.Add(EActionListCore::slowTimeFunction);
	getActorFunctionsCore.Add(EActionListCore::attachToObject);
	getActorFunctionsCore.Add(EActionListCore::playSoundFunction);
	getActorFunctionsCore.Add(EActionListCore::changePriority);
	getActorFunctionsCore.Add(EActionListCore::executeTrigger);
	getActorFunctionsCore.Add(EActionListCore::setPhysicsSettings);
	getActorFunctionsCore.Add(EActionListCore::addTagFunction);
	getActorFunctionsCore.Add(EActionListCore::setMaterialMesh);
	getActorFunctionsCore.Add(EActionListCore::createEditVariable);
	getActorFunctionsCore.Add(EActionListCore::getExternalVariable);
	getActorFunctionsCore.Add(EActionListCore::setTooltipLocationSize);
	getActorFunctionsCore.Add(EActionListCore::relocateFunction);
	getActorFunctionsCore.Add(EActionListCore::showHideOutlineShine);
	getActorFunctionsCore.Add(EActionListCore::getInternalVariable);
	getActorFunctionsCore.Add(EActionListCore::setInternalVariable);
	getActorFunctionsCore.Add(EActionListCore::executeInternalFunction);
	getActorFunctionsCore.Add(EActionListCore::callExternalDelegateFunction);
	getActorFunctionsCore.Add(EActionListCore::whenFunction);

	//AQUI LAS CHECKFUNCTIONS

	getActorCheckFunctionsCore.Add(ECheckListCore::checkActorLocation);
	getActorCheckFunctionsCore.Add(ECheckListCore::checkActorRotation);
	getActorCheckFunctionsCore.Add(ECheckListCore::checkRaycast);
	getActorCheckFunctionsCore.Add(ECheckListCore::checkIfActorExists);


	if (actorBaseFunctionName.Num() <= 0) {
		actorBaseFunctionName.Add("debugTest");
	}

	if (actorCheckBaseFunctionName.Num() <= 0) {
	}

}

CREATE_FUNCTIONACTOR(GetCustomActor, UInteractComponent, FGetCustomActor, getCustomActor)
CREATE_FUNCTIONACTOR(GetSpecificActor, UInteractComponent, FGetSpecificActor, getSpecificActor)
CREATE_FUNCTIONACTOR(GetThisActor, UInteractComponent, FGetThisActor, getThisActor)


CREATE_FUNCTIONCORE(PlaySequence, UInteractComponent, FPlaySequence, playSequence)
CREATE_FUNCTIONCORE(ExecuteAction, UInteractComponent, FExecuteAction, executeAction)
CREATE_FUNCTIONCORE(JumpStage, UInteractComponent, FJumpStage, jumpStage)
CREATE_FUNCTIONCORE(DebugText, UInteractComponent, FDebugText, debugText)
CREATE_FUNCTIONCORE(ExternalInteract, UInteractComponent, FExternalInteract, externalInteract)
CREATE_FUNCTIONCORE(JumpFunction, UInteractComponent, FJumpFunction, jumpFunction)
CREATE_FUNCTIONCORE(CancelJumpStage, UInteractComponent, FCancelJumpStage, cancelJumpStage)
CREATE_FUNCTIONCORE(BreakFunction, UInteractComponent, FBreakFunction, breakFunction)
CREATE_FUNCTIONCORE(TeleportTo, UInteractComponent, FTeleportTo, teleportTo)
CREATE_FUNCTIONCORE(RotateLookAtActor, UInteractComponent, FRotateLookAtActor, rotateLookAtActor)
CREATE_FUNCTIONCORE(PlayAnimCharacter, UInteractComponent, FPlayAnimCharacter, playAnimCharacter)
CREATE_FUNCTIONCORE(SetVisibilityActor, UInteractComponent, FSetVisibilityActor, setVisibilityActor)
CREATE_FUNCTIONCORE(SetTransformActor, UInteractComponent, FSetTransformActor, setTransformActor)
CREATE_FUNCTIONCORE(DestroyEntity, UInteractComponent, FDestroyEntity, destroyEntity)
CREATE_FUNCTIONCORE(MoveToFunction, UInteractComponent, FMoveToFunction, moveToFunction)
CREATE_FUNCTIONCORE(RotateToFunction, UInteractComponent, FRotateToFunction, rotateToFunction)
CREATE_FUNCTIONCORE(CancelTimer, UInteractComponent, FCancelTimer, cancelTimer)
CREATE_FUNCTIONCORE(CreateEditVariable, UInteractComponent, FCreateEditVariable, createEditVariable)
CREATE_FUNCTIONCORE(SpawnerFunction, UInteractComponent, FSpawnerFunction, spawnerFunction)
CREATE_FUNCTIONCORE(SetRagdoll, UInteractComponent, FSetRagdoll, setRagdoll)
CREATE_FUNCTIONCORE(ShowHideTooltip, UInteractComponent, FShowHideTooltip, showHideTooltip)
CREATE_FUNCTIONCORE(AddImpulseFunction, UInteractComponent, FAddImpulseFunction, addImpulseFunction)
CREATE_FUNCTIONCORE(SetCollisionSettings, UInteractComponent, FSetCollisionSettings, setCollisionSettings)
CREATE_FUNCTIONCORE(SlowTimeFunction, UInteractComponent, FSlowTimeFunction, slowTimeFunction)
CREATE_FUNCTIONCORE(AttachToObject, UInteractComponent, FAttachToObject, attachToObject)
CREATE_FUNCTIONCORE(PlaySoundFunction, UInteractComponent, FPlaySoundFunction, playSoundFunction)
CREATE_FUNCTIONCORE(StopSoundFunction, UInteractComponent, FStopSoundFunction, stopSoundFunction)
CREATE_FUNCTIONCORE(ModifySoundFunction, UInteractComponent, FModifySoundFunction, modifySoundFunction)
CREATE_FUNCTIONCORE(CameraEffect, UInteractComponent, FCameraEffect, cameraEffect)
CREATE_FUNCTIONCORE(OpenLevelFunction, UInteractComponent, FOpenLevelFunction, openLevelFunction)
CREATE_FUNCTIONCORE(ChangePriority, UInteractComponent, FChangePriority, changePriority)
CREATE_FUNCTIONCORE(ExecuteTrigger, UInteractComponent, FExecuteTrigger, executeTrigger)
CREATE_FUNCTIONCORE(AddWidgetToViewport, UInteractComponent, FAddWidgetToViewport, addWidgetToViewport)
CREATE_FUNCTIONCORE(RemoveWidgetFunction, UInteractComponent, FRemoveWidgetFunction, removeWidgetFunction)
CREATE_FUNCTIONCORE(SetPhysicsSettings, UInteractComponent, FSetPhysicsSettings, setPhysicsSettings)
CREATE_FUNCTIONCORE(AddTagFunction, UInteractComponent, FAddTagFunction, addTagFunction)
CREATE_FUNCTIONCORE(SetMaterialMesh, UInteractComponent, FSetMaterialMesh, setMaterialMesh)
CREATE_FUNCTIONCORE(GetExternalVariable, UInteractComponent, FGetExternalVariable, getExternalVariable)
CREATE_FUNCTIONCORE(SetTooltipLocationSize, UInteractComponent, FSetTooltipLocationSize, setTooltipLocationSize)
CREATE_FUNCTIONCORE(RelocateFunction, UInteractComponent, FRelocateFunction, relocateFunction)
CREATE_FUNCTIONCORE(PlayWidgetAnimation, UInteractComponent, FPlayWidgetAnimation, playWidgetAnimation)
CREATE_FUNCTIONCORE(ShowHideOutlineShine, UInteractComponent, FShowHideOutlineShine, showHideOutlineShine)
CREATE_FUNCTIONCORE(GetInternalVariable, UInteractComponent, FGetInternalVariable, getInternalVariable)
CREATE_FUNCTIONCORE(ExecCommand, UInteractComponent, FExecCommand, execCommand)
CREATE_FUNCTIONCORE(SetInternalVariable, UInteractComponent, FSetInternalVariable, setInternalVariable)
CREATE_FUNCTIONCORE(ExecuteInternalFunction, UInteractComponent, FExecuteInternalFunction, executeInternalFunction)
CREATE_FUNCTIONCORE(CallExternalDelegateFunction, UInteractComponent, FCallExternalDelegateFunction, callExternalDelegateFunction)
CREATE_FUNCTIONCORE(LoadStreamLevelInteract, UInteractComponent, FLoadStreamLevelInteract, loadStreamLevelInteract)
CREATE_FUNCTIONCORE(UnloadStreamLevelInteract, UInteractComponent, FUnloadStreamLevelInteract, unloadStreamLevelInteract)


CREATE_FUNCTIONCHECKCORE(DebugCondition, UInteractComponent, FDebugCondition, debugCondition)
CREATE_FUNCTIONCHECKCORE(DecrementIncrement, UInteractComponent, FDecrementIncrement, decrementIncrement)
CREATE_FUNCTIONCHECKCORE(CheckActorLocation, UInteractComponent, FCheckActorLocation, checkActorLocation)
CREATE_FUNCTIONCHECKCORE(CheckActorRotation, UInteractComponent, FCheckActorRotation, checkActorRotation)
CREATE_FUNCTIONCHECKCORE(CheckVariableValue, UInteractComponent, FCheckVariableValue, checkVariableValue)
CREATE_FUNCTIONCHECKCORE(CheckRaycast, UInteractComponent, FCheckRaycast, checkRaycast)
CREATE_FUNCTIONCHECKCORE(CheckTooltipVisibility, UInteractComponent, FCheckTooltipVisibility, checkTooltipVisibility)
CREATE_FUNCTIONCHECKCORE(CheckIfActorExists, UInteractComponent, FCheckIfActorExists, checkIfActorExists)
CREATE_FUNCTIONCHECKCORE(CheckIfSoundIsPlaying, UInteractComponent, FCheckIfSoundIsPlaying, checkIfSoundIsPlaying)



bool UInteractMacrosClass::AndFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType) {
	if (actionStageType == EActionStageType::Pre) {
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) {
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) {
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction), interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.customVariableSet, FAndFunction::StaticStruct(), interactComponent);
				return UInteractComponent::AndFunction(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction, interactComponent);
			}
		}
	}
	else if (actionStageType == EActionStageType::Init) {
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) {
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) {
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.customVariableSet, FAndFunction::StaticStruct(), interactComponent);
				return UInteractComponent::AndFunction(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction, interactComponent);
			}
		}
	}
	else if (actionStageType == EActionStageType::Interact) {
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) {
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) {
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction), interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.customVariableSet, FAndFunction::StaticStruct(), interactComponent);
				return UInteractComponent::AndFunction(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction, interactComponent);
			}
		}
	}
	else if (actionStageType == EActionStageType::Exit) {
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) {
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) {
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.customVariableSet, FAndFunction::StaticStruct(), interactComponent);
				return UInteractComponent::AndFunction(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction, interactComponent);
			}
		}
	}
	return false;
}

void UInteractMacrosClass::DelayFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, bool inElse)
{
	int functionIn = functionInd == -1 ? interactComponent->functionIndex : functionInd;
	int currentSta = currentStag == -1 ? interactComponent->m_currentStage : currentStag;
	if (actionStageType == EActionStageType::Pre) {
		if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::delayFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].delayFunction), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].delayFunction.customVariableSet, FDelayFunction::StaticStruct(), interactComponent);
			UInteractComponent::DelayFunction(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].delayFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Init) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::delayFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].delayFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].delayFunction.customVariableSet, FDelayFunction::StaticStruct(), interactComponent);
			UInteractComponent::DelayFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].delayFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Interact) {
		if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::delayFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].delayFunction), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].delayFunction.customVariableSet, FDelayFunction::StaticStruct(), interactComponent);
			UInteractComponent::DelayFunction(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].delayFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Exit) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::delayFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].delayFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].delayFunction.customVariableSet, FDelayFunction::StaticStruct(), interactComponent);
			UInteractComponent::DelayFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].delayFunction, interactComponent);
		}
	}
	else {
		return;
	}
}


void UInteractMacrosClass::WhenFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, bool inElse) {
	int functionIn = functionInd == -1 ? interactComponent->functionIndex : functionInd;
	int currentSta = currentStag == -1 ? interactComponent->m_currentStage : currentStag;
	if (actionStageType == EActionStageType::Pre) {
		if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::whenFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].whenFunction), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
			UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].whenFunction, interactComponent);
		}
		else if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			if (!inElse) {
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.whenFunction), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.whenFunction, interactComponent);
				}
			}
			else {
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.whenFunction), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.whenFunction, interactComponent);
				}
			}
		}
	}
	else if (actionStageType == EActionStageType::Init) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::whenFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
			UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].whenFunction, interactComponent);
		}
		else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			if (!inElse) {
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.whenFunction, interactComponent);
				}
			}
			else {
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.whenFunction, interactComponent);
				}
			}
		}
	}
	else if (actionStageType == EActionStageType::Interact) {
		if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::whenFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].whenFunction), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
			UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].whenFunction, interactComponent);
		}
		else if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			if (!inElse) {
				if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.whenFunction), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.whenFunction, interactComponent);
				}
			}
			else {
				if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.whenFunction), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.whenFunction, interactComponent);
				}
			}
		}
	}
	else if (actionStageType == EActionStageType::Exit) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::whenFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
			UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].whenFunction, interactComponent);
		}
		else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			if (!inElse) {
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.whenFunction, interactComponent);
				}
			}
			else {
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::whenFunction) {
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.whenFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.whenFunction.customVariableSet, FWhenFunction::StaticStruct(), interactComponent);
					UInteractComponent::WhenFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.whenFunction, interactComponent);
				}
			}
		}
	}
	else {
		return;
	}
}

void UInteractMacrosClass::IfFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, bool inElse) {
	int functionIn = functionInd == -1 ? interactComponent->functionIndex : functionInd;
	int currentSta = currentStag == -1 ? interactComponent->m_currentStage : currentStag;
	if (actionStageType == EActionStageType::Pre) {
		if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.customVariableSet, FIfFunction::StaticStruct(), interactComponent);
			UInteractComponent::IfFunction(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Init) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.customVariableSet, FIfFunction::StaticStruct(), interactComponent);
			UInteractComponent::IfFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Interact) {
		if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.customVariableSet, FIfFunction::StaticStruct(), interactComponent);
			UInteractComponent::IfFunction(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction, interactComponent);
		}
	}
	else if (actionStageType == EActionStageType::Exit) {
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::ifFunction) {
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.customVariableSet, FIfFunction::StaticStruct(), interactComponent);
			UInteractComponent::IfFunction(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction, interactComponent);
		}
	}
	else {
		return;
	}
}

void UInteractMacrosClass::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInteractMacrosClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

