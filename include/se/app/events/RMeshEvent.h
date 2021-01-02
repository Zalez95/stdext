#ifndef RMESH_EVENT_H
#define RMESH_EVENT_H

#include "../Entity.h"
#include "Event.h"

namespace se::app {

	/**
	 * Class RMeshEvent, its an event used for notify of a MeshComponent update
	 */
	class RMeshEvent : public Event<Topic::RMesh>
	{
	public:		// Nested types
		/** The different operations to perform with the RMeshEvent */
		enum class Operation { Add, Remove };

	private:	// Attributes
		/** The Operation of the RMeshEvent */
		Operation mOperation;

		/** The Entity that owns the MeshComponent updated */
		Entity mEntity;

		/** The index of the RenderableMesh updated */
		std::size_t mRIndex;

	public:		// Functions
		/** Creates a new RMeshEvent used for notifying of a MeshComponent
		 * add/remove Operation
		 *
		 * @param	operation the Operation to perform
		 * @param	entity the Entity that owns the MeshComponent updated
		 * @param	rIndex the index of the RenderableMesh updated */
		RMeshEvent(Operation operation, Entity entity, std::size_t rIndex) :
			mOperation(operation), mEntity(entity), mRIndex(rIndex) {};

		/** @return	the Operation to perform */
		Operation getOperation() const { return mOperation; };

		/** @return	the Entity to notify */
		Entity getEntity() const { return mEntity; };

		/** @return	the index of the RenderableMesh updated */
		std::size_t getRIndex() const { return mRIndex; };
	};

}

#endif		// RMESH_EVENT_H
