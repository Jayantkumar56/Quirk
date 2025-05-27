

#pragma once

#include "Core/Core.h"

namespace Quirk {

    template<typename T>
    class View {
    public:
        View() noexcept = default;

        View( const T* ptr           ) noexcept : m_Ptr( ptr          ) {}
        View( const Scope<T>& scoped ) noexcept : m_Ptr( scoped.get() ) {}
        View( const Ref<T>&   ref    ) noexcept : m_Ptr( ref.get()    ) {}

        View( Scope<T>&& scoped ) noexcept = delete;
        View( Ref<T>&&   ref    ) noexcept = delete;

        inline T* operator->() noexcept { return m_Ptr;  }
        inline T& operator*()  noexcept { return *m_Ptr; }
        inline T* Get()        noexcept { return m_Ptr;  }

        inline const T* operator->() const noexcept { return m_Ptr;  }
        inline const T& operator*()  const noexcept { return *m_Ptr; }
        inline const T* Get()        const noexcept { return m_Ptr;  }

        inline operator bool() const noexcept { return m_Ptr != nullptr; }

        inline bool operator==(std::nullptr_t) const noexcept { return m_Ptr == nullptr; }
        inline bool operator!=(std::nullptr_t) const noexcept { return m_Ptr != nullptr; }

        inline bool IsValid() const noexcept { return m_Ptr != nullptr; }

    private:
        T* m_Ptr = nullptr;
    };

    template<typename T>
    class ConstView {
    public:
        ConstView() noexcept = default;

        ConstView( const T* ptr           ) noexcept : m_Ptr( ptr          ) {}
        ConstView( const Scope<T>& scoped ) noexcept : m_Ptr( scoped.get() ) {}
        ConstView( const Ref<T>&   ref    ) noexcept : m_Ptr( ref.get()    ) {}
        ConstView( const View<T>&  view   ) noexcept : m_Ptr( view.Get()   ) {}

        ConstView( Scope<T>&& scoped ) noexcept = delete;
        ConstView( Ref<T>&&   ref    ) noexcept = delete;
        ConstView( View<T>&&  view   ) noexcept = delete;

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
