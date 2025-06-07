

#pragma once

#include "Core/Core.h"

namespace Quirk {

    template<typename T>
    class View {
    public:
        View() noexcept = default;
        View(T* ptr) noexcept : m_Ptr(ptr) {}

        // ==== BEGIN: Conversion functions ===========

        View( Scope<T>& scoped ) noexcept : m_Ptr( scoped.get() ) {}
        View( Ref<T>&   ref    ) noexcept : m_Ptr( ref.get()    ) {}

        View( Scope<T>&& scoped ) noexcept = delete;
        View( Ref<T>&&   ref    ) noexcept = delete;

        template<typename Derived>
        requires (std::derived_from<Derived, T> && !std::same_as<T, Derived>)
        View(View<Derived>& other) noexcept : m_Ptr(other.Get()) {}

        template<typename Base>
        requires (std::derived_from<T, Base> && !std::same_as<T, Base>)
        View(View<Base>& other) noexcept : m_Ptr(static_cast<T*>(other.Get())) {}

        // ==== END:   Conversion functions ===========

        inline T* operator->()       noexcept { return m_Ptr;  }
        inline T& operator*()        noexcept { return *m_Ptr; }
        inline T* Get()        const noexcept { return m_Ptr;  }

        inline operator bool() const noexcept { return m_Ptr != nullptr; }

        inline bool operator==(std::nullptr_t) const noexcept { return m_Ptr == nullptr; }
        inline bool operator!=(std::nullptr_t) const noexcept { return m_Ptr != nullptr; }

        inline bool IsValid() const noexcept { return m_Ptr != nullptr; }

        template<typename U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
        View(const View<U>& other) noexcept : m_Ptr(other.Get()) {}

    private:
        T* m_Ptr = nullptr;
    };

    template<typename T>
    class ConstView {
    public:
        ConstView() noexcept = default;
        ConstView(const T* ptr) noexcept : m_Ptr(ptr) {}

        // ==== BEGIN: Conversion functions ===========

        ConstView( const Scope<T>& scoped ) noexcept : m_Ptr( scoped.get() ) {}
        ConstView( const Ref<T>&   ref    ) noexcept : m_Ptr( ref.get()    ) {}
        ConstView( const View<T>&  view   ) noexcept : m_Ptr( view.Get()   ) {}

        ConstView( Scope<T>&& scoped ) noexcept = delete;
        ConstView( Ref<T>&&   ref    ) noexcept = delete;
        ConstView( View<T>&&  view   ) noexcept = delete;

        template<typename Derived>
        requires (std::derived_from<Derived, T> && !std::same_as<T, Derived>)
        ConstView(ConstView<Derived>& other) noexcept : m_Ptr(other.Get()) {}

        template<typename Base>
        requires (std::derived_from<T, Base> && !std::same_as<T, Base>)
        ConstView(ConstView<Base>& other) noexcept : m_Ptr(static_cast<T*>(other.Get())) {}

        // ==== END:   Conversion functions ===========

        inline const T* operator->() const noexcept { return m_Ptr;  }
        inline const T& operator*()  const noexcept { return *m_Ptr; }
        inline const T* Get()        const noexcept { return m_Ptr;  }

        inline operator bool() const noexcept { return m_Ptr != nullptr; }

        inline bool operator==(std::nullptr_t) const noexcept { return m_Ptr == nullptr; }
        inline bool operator!=(std::nullptr_t) const noexcept { return m_Ptr != nullptr; }

        inline bool IsValid() const noexcept { return m_Ptr != nullptr; }

    private:
        const T* m_Ptr = nullptr;
    };

}
