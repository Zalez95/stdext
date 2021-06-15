#ifndef DYNAMICS_SYSTEM_H
#define DYNAMICS_SYSTEM_H

#include "ECS.h"

namespace se::app {

	class Application;
	class RigidBodyComponent;


	/**
	 * Class DynamicsSystem, it's a System for updating the Entities' physics
	 * data
	 */
	class DynamicsSystem : public ISystem
	{
	private:	// Attributes
		/** The Application that holds the PhysicsEngine used for updating
		 * Entities */
		Application& mApplication;

	public:		// Functions
		/** Creates a new DynamicsSystem
		 *
		 * @param	application a reference to the Application that holds the
		 *			current System */
		DynamicsSystem(Application& application);

		/** Class destructor */
		~DynamicsSystem();

		/** @copydoc ISystem::onNewComponent(Entity, const EntityDatabase::ComponentMask&) */
		virtual void onNewComponent(
			Entity entity, const EntityDatabase::ComponentMask& mask
		) override
		{ tryCallC(&DynamicsSystem::onNewRigidBody, entity, mask); };

		/** @copydoc ISystem::onRemoveComponent(Entity, const EntityDatabase::ComponentMask&) */
		virtual void onRemoveComponent(
			Entity entity, const EntityDatabase::ComponentMask& mask
		) override
		{ tryCallC(&DynamicsSystem::onRemoveRigidBody, entity, mask); };

		/** Integrates the RigidBodies data of the entities */
		virtual void update() override;
	private:
		/** Function called when a RigidBodyComponent is added to an Entity
		 *
		 * @param	entity the Entity that holds the RigidBodyComponent
		 * @param	rigidBody a pointer to the new RigidBodyComponent */
		void onNewRigidBody(Entity entity, RigidBodyComponent* rigidBody);

		/** Function called when a RigidBodyComponent is going to be removed
		 * from an Entity
		 *
		 * @param	entity the Entity that holds the RigidBodyComponent
		 * @param	rigidBody a pointer to the RigidBodyComponent that is going
		 *			to be removed */
		void onRemoveRigidBody(Entity entity, RigidBodyComponent* rigidBody);
	};

}

#endif	// DYNAMICS_SYSTEM_H
