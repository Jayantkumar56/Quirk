

#pragma once

#include "Core/Utility/UUID.h"
#include "Core/Core.h"
#include "Core/Assets/AssetHandle.h"

#include <typeindex>


namespace Quirk {

	class AssetStorage {
	public:
		AssetStorage() {
			auto storage = m_StoragePools.insert({ typeid(int), CreateScope<StorageForType<int>>() });
			
		}

		template<typename T>
		void AddAsset(const AssetHandle& handle, Ref<T> asset) {
			GetOrCreateStorage<T>()->Add(handle, std::move(asset));
		}

		template<typename T>
		Ref<T> GetAsset(const AssetHandle& handle) const {
			auto* storage = GetStorage<T>();
			return storage ? storage->Get(handle) : nullptr;
		}

		template<typename T>
		bool HasAsset(const AssetHandle& handle) const {
			auto* storage = GetStorage<T>();
			return storage ? storage->Has(handle) : false;
		}

		template<typename T>
		void RemoveAsset(const AssetHandle& handle) {
			auto* storage = GetStorage<T>();
			if (storage)
				storage->Remove(handle);
		}

	private:
		class StorageForTypeBase{
		public:
			virtual ~StorageForTypeBase() = default;
		};

		template<typename T>
		class StorageForType : public StorageForTypeBase {
		public:
			std::unordered_map<AssetHandle, Ref<T>> Assets;

			Ref<T> Get(const AssetHandle& handle) const {
				auto it = Assets.find(handle);
				return it != Assets.end() ? it->second : nullptr;
			}

			void Add(const AssetHandle& handle, Ref<T> asset) {
				Assets[handle] = std::move(asset);
			}

			bool Has(const AssetHandle& handle) const {
				return Assets.find(handle) != Assets.end();
			}

			void Remove(const AssetHandle& handle) {
				Assets.erase(handle);
			}
		};

	private:
		template<typename T>
		StorageForType<T>* GetStorage() const {
			auto it = m_StoragePools.find(typeid(T));
			if (it != m_StoragePools.end()) {
				return static_cast<StorageForType<T>*>(it->second.get());
			}
			return nullptr;
		}

		template<typename T>
		StorageForType<T>* GetOrCreateStorage() {
			auto it = m_StoragePools.find(typeid(T));
			if (it != m_StoragePools.end()) {
				return static_cast<StorageForType<T>*>(it->second.get());
			}

			auto storage = m_StoragePools.insert({ typeid(T), CreateScope<StorageForType<T>>() });

			return static_cast<StorageForType<T>*>(storage.first->second.get());
		}

	private:
		std::unordered_map<std::type_index, Scope<StorageForTypeBase>> m_StoragePools;
	};

}
