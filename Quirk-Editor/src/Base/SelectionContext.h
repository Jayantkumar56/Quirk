

#pragma once

#include <typeindex>
#include <tuple>
#include <variant>
#include <vector>


namespace QuirkEditor {
    
    // ============================================================================================================================
    //                                           Global Selection Context
    // ============================================================================================================================
    // Notes:
    // 
    // - all the storage is per type and the containers are static 
    //   thus only a single global SelectionContext is available right now.
    //   
    // TODO:
    // 
    // - make the SelectionContext usable as an instance 
    //   (would enables to create different selection contexts per editor frame for example)
    //   (mainly to move all the individual containers associated with different types from static space to dynamic space)
    // 
    // ----------------------------------------------------------------------------------------------------------------------------

    class SelectionContext {
        template<
            template<template<typename...> class, typename ...> class SelectionAccessPolicy,
            template<typename...> class SelectionRetentionPolicy,
            typename... Types
        >
        friend class SelectionHandle;

    public:
        template<typename T>
        static void SetSelected(T data) {
            s_DataStorage<T> = data;

            for (size_t i = 0; i < s_Callbacks<T>.size(); ++i)
                s_Callbacks<T>[i].Call(data);

            const std::type_index currentTypeIndex = typeid(T);

            if (s_LastSelectedType != currentTypeIndex) {
                for (size_t i = 0; i < s_TypeChangeCallbacks.size(); ++i)
                    s_TypeChangeCallbacks[i].Call(currentTypeIndex);

                s_LastSelectedType = currentTypeIndex;
            }
        }

        template<typename T>
        static T GetSelected() { return s_DataStorage<T>; }

        static inline std::type_index GetLastSelectedType() noexcept { return s_LastSelectedType; }

    private:
        template<typename T>
        struct SelectionCallback {
            void* Obj;
            void (*Func)(void*, T);

            inline void Call(T value) { Func(Obj, value); }
        };

        struct TypeChangeCallback {
            void* Obj;
            void (*Func)(void*, std::type_index);

            inline void Call(std::type_index value) { Func(Obj, value); }
        };

    private:
        template<typename T>
        static void RegisterCallback(void* obj, void (*func)(void*, T)) {
            s_Callbacks<T>.emplace_back(obj, func);
        }

        template<typename T>
        static void UnRegisterCallback(void* obj, void (*func)(void*, T)) {
            for (size_t i = 0; i < s_Callbacks<T>.size(); ++i) {
                if (s_Callbacks<T>[i].Obj == obj && s_Callbacks<T>[i].Func == func) {
                    s_Callbacks<T>.erase(s_Callbacks<T>.begin() + i);
                    return;
                }
            }
        }

        static inline void RegisterTypeChangeCallback(void* obj, void (*func)(void*, std::type_index)) {
            s_TypeChangeCallbacks.emplace_back(obj, func);
        }

        static inline void UnRegisterTypeChangeCallback(void* obj, void (*func)(void*, std::type_index)) {
            for (size_t i = 0; i < s_TypeChangeCallbacks.size(); ++i) {
                if (s_TypeChangeCallbacks[i].Obj == obj && s_TypeChangeCallbacks[i].Func == func) {
                    s_TypeChangeCallbacks.erase(s_TypeChangeCallbacks.begin() + i);
                    return;
                }
            }
        }

    private:
        static inline std::type_index s_LastSelectedType = typeid(void);

        static inline std::vector<TypeChangeCallback> s_TypeChangeCallbacks;

        template<typename T>
        static inline T s_DataStorage{};

        template<typename T>
        static inline std::vector<SelectionCallback<T>> s_Callbacks{};
    };

    // ============================================================================================================================



    // ============================================================================================================================
    //                                               Selection Clearing Policy
    // ============================================================================================================================
    // Notes:
    // 
    // - Every Selection_Clearing_Policy must have a function with the name and signature as
    //   bool ShouldRetainSelection(std::type_index newtype) 
    // 
    // ----------------------------------------------------------------------------------------------------------------------------

    // these are just marker policies actual implementation 
    // should be done in each of the Selection Storate/Acess Policy

    // retains selection until a new selection of same type arrives then replace the data with new one
    template<typename ...Types>
    struct RetainSelection {
    protected:
        constexpr bool ShouldRetainSelection(std::type_index) const noexcept { return true; }
    };

    // retains selection until a selection of different type arrives then clear the selection data
    template<typename ...Types>
    struct ClearOnTypeMismatch {
    protected:
        bool ShouldRetainSelection(std::type_index newtype) const {
            return ((newtype == typeid(Types)) || ...);
        }
    };

    // ============================================================================================================================



    // ============================================================================================================================
    //                                           Selection Storage/Access Policy
    // ============================================================================================================================
    // Notes:
    // 
    // - initialization of the data should be considered in the constructor of the policy
    // 
    // - Every Selection_(Storage/Access)_Policy must have these functions with the name and signature as
    // 
    //   1.:  template<typename T>
    //        void Set(T data)
    // 
    //   2.:  void OnSelectedTypeChange(std::type_index newType)
    // 
    //   3.:  template<typename T>
    //        T& Get()
    // 
    // ----------------------------------------------------------------------------------------------------------------------------

    // storage is provided for every registered type
    template<template<typename...> class SelectionRetentionPolicy, typename ...Types>
    struct PerTypeSelection : public SelectionRetentionPolicy<Types...> {
        using RetentionPolicy = SelectionRetentionPolicy<Types...>;

    public:
        PerTypeSelection() :
            m_Data(SelectionContext::GetSelected<Types>()...)
        {}

        template<typename T>
        T& Get() noexcept { return std::get<T>(m_Data); }

        template<typename T>
        void Set(T data) { std::get<T>(m_Data) = data; }

    protected:
        inline void OnSelectedTypeChange(std::type_index newType) {
            if (!RetentionPolicy::ShouldRetainSelection(newType)) {
                // TODO: think about the values to fall onto
                m_Data = std::tuple<Types...>{};
            }
        }

    private:
        std::tuple<Types...> m_Data{};
    };

    // TODO: think about the initial state of the selection for SingleActiveSelection
    // only the recent incoming selection among the registered type is stored
    template<template<typename...> class SelectionRetentionPolicy, typename ...Types>
    struct SingleActiveSelection : public SelectionRetentionPolicy<Types...> {
        using RetentionPolicy = SelectionRetentionPolicy<Types...>;

    public:
        template<typename T>
        void Set(T data) { m_Data = data; }

        template<typename T>
        T& Get() { return std::get<T>(m_Data); }

        template<typename T>
        bool Has() noexcept { return std::holds_alternative<T>(m_Data); }

    protected:
        inline void OnSelectedTypeChange(std::type_index newType) {
            if (!RetentionPolicy::ShouldRetainSelection(newType)) {
                m_Data = std::monostate{};
            }
        }

    private:
        std::variant<std::monostate, Types...> m_Data;
    };

    // ============================================================================================================================



    // ============================================================================================================================
    //                                           Local Selection Handle
    // ============================================================================================================================

    template<
        template<template<typename...> class, typename ...> class SelectionAccessPolicy,
        template<typename...> class SelectionRetentionPolicy,
        typename... Types
    >
    class SelectionHandle : public SelectionAccessPolicy<SelectionRetentionPolicy, Types...>  {
    private:
        using AccessPolicy = SelectionAccessPolicy<SelectionRetentionPolicy, Types...>;

    public:
        SelectionHandle() {
            (SelectionContext::RegisterCallback<Types>(static_cast<void*>(this), &OnSelection<Types>), ...);
            SelectionContext::RegisterTypeChangeCallback(static_cast<void*>(this), &OnLastSelectedTypeChange);
        }

        ~SelectionHandle() {
            (SelectionContext::UnRegisterCallback<Types>(static_cast<void*>(this), &OnSelection<Types>), ...);
            SelectionContext::UnRegisterTypeChangeCallback(static_cast<void*>(this), &OnLastSelectedTypeChange);
        }

        SelectionHandle(SelectionHandle&&)                  = default;
        SelectionHandle& operator =(SelectionHandle&&)      = default;

        SelectionHandle(const SelectionHandle&)             = default;
        SelectionHandle& operator =(const SelectionHandle&) = default;

    private:
        template<typename T>
        static void OnSelection(void* obj, T value) {
            static_cast<SelectionHandle*>(obj)->AccessPolicy::Set(value);
        }

        static inline void OnLastSelectedTypeChange(void* obj, std::type_index newType) {
            static_cast<SelectionHandle*>(obj)->AccessPolicy::OnSelectedTypeChange(newType);
        }
    };

    // selection handle that only allows one active selection at a time.
    // automatically clears the selection if a type mismatch occurs.
    template<typename... Types>
    using UniqueSelectionHandle = SelectionHandle<SingleActiveSelection, ClearOnTypeMismatch, Types...>;

    // selection handle that allows one active selection per type.
    // automatically clears the selection if a type mismatch occurs.
    template<typename... Types>
    using PerTypeSelectionHandle = SelectionHandle<PerTypeSelection, ClearOnTypeMismatch, Types...>;

    // ============================================================================================================================

}
