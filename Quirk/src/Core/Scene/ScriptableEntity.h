

#pragma once

#include "Core/Scene/Entity.h"

namespace Quirk {

	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}

	protected:
		Entity m_Entity;
		friend class Scene;
	};

}
