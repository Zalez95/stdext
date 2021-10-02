#include "se/utils/Log.h"
#include "se/physics/RigidBodyWorld.h"
#include "se/app/TransformsComponent.h"
#include "se/app/RigidBodyComponent.h"
#include "se/app/Application.h"
#include "se/app/PhysicsSystem.h"

namespace se::app {

	PhysicsSystem::PhysicsSystem(Application& application) :
		ISystem(application.getEntityDatabase()), mApplication(application)
	{
		mEntityDatabase.addSystem(this, EntityDatabase::ComponentMask().set<RigidBodyComponent>());
	}


	PhysicsSystem::~PhysicsSystem()
	{
		mEntityDatabase.removeSystem(this);
	}


	void PhysicsSystem::update()
	{
		SOMBRA_DEBUG_LOG << "Start";

		SOMBRA_DEBUG_LOG << "Updating the RigidBodies";
		mEntityDatabase.iterateComponents<TransformsComponent, RigidBodyComponent>(
			[this](Entity, TransformsComponent* transforms, RigidBodyComponent* rigidBody) {
				if (!transforms->updated[static_cast<int>(TransformsComponent::Update::RigidBody)]) {
					physics::RigidBodyState state = rigidBody->getState();
					state.position			= transforms->position;
					state.linearVelocity	= transforms->velocity;
					state.orientation		= transforms->orientation;
					rigidBody->setState(state);

					transforms->updated.set(static_cast<int>(TransformsComponent::Update::RigidBody));
				}
			},
			true
		);

		SOMBRA_DEBUG_LOG << "Updating the RigidBodyWorld";
		mApplication.getExternalTools().rigidBodyWorld->update(mDeltaTime);

		SOMBRA_DEBUG_LOG << "Updating the Entities";
		mEntityDatabase.iterateComponents<TransformsComponent, RigidBodyComponent>(
			[this](Entity, TransformsComponent* transforms, RigidBodyComponent* rigidBody) {
				if (!rigidBody->get().getStatus(physics::RigidBodyState::Status::Sleeping)) {
					transforms->position	= rigidBody->getState().position;
					transforms->velocity	= rigidBody->getState().linearVelocity;
					transforms->orientation	= rigidBody->getState().orientation;

					transforms->updated.reset().set(static_cast<int>(TransformsComponent::Update::RigidBody));
				}
			},
			true
		);

		SOMBRA_DEBUG_LOG << "End";
	}

// Private functions
	void PhysicsSystem::onNewRigidBody(Entity entity, RigidBodyComponent* rigidBody)
	{
		auto [transforms] = mEntityDatabase.getComponents<TransformsComponent>(entity, true);
		if (transforms) {
			transforms->updated.reset(static_cast<int>(TransformsComponent::Update::RigidBody));
		}

		mApplication.getExternalTools().rigidBodyWorld->addRigidBody(&rigidBody->get());
		SOMBRA_INFO_LOG << "Entity " << entity << " with RigidBodyComponent " << rigidBody << " added successfully";
	}


	void PhysicsSystem::onRemoveRigidBody(Entity entity, RigidBodyComponent* rigidBody)
	{
		mApplication.getExternalTools().rigidBodyWorld->removeRigidBody(&rigidBody->get());
		SOMBRA_INFO_LOG << "Entity " << entity << " with RigidBodyComponent " << rigidBody << " removed successfully";
	}

}