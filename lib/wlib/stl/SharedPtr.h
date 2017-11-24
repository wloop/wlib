#ifndef EMBEDDEDCPLUSPLUS_SHAREDPTR_H
#define EMBEDDEDCPLUSPLUS_SHAREDPTR_H

#include "UniquePtr.h"

#include "../Types.h"
#include "../utility/Tmp.h"
#include "../utility/Utility.h"
//#include "../utility/Utility.h"

namespace wlp {

    typedef int16_t ptr_use_count;

    static inline ptr_use_count __exchange_add(ptr_use_count *mem, int16_t val) {
        ptr_use_count res = *mem;
        *mem += val;
        return res;
    }

    static inline void __add(ptr_use_count *mem, int16_t val) {
        *mem += val;
    }

    class __BaseCount {
        ptr_use_count m_use_count;
        ptr_use_count m_weak_count;

    public:
        __BaseCount()
                : m_use_count(1),
                  m_weak_count(1) {
        }

        virtual ~__BaseCount() {
        }

        virtual void dispose() = 0;

        virtual void destroy() {
            delete this;
        }

        virtual void *get_deleter() = 0;

        void add_ref_copy() {
            __add(&m_use_count, 1);
        }

        void add_ref_lock() {
            if (__exchange_add(&m_use_count, 1) == 0) {
                m_use_count = 0;
                // Bad weak pointer
            }
        }

        void release() {
            if (__exchange_add(&m_use_count, -1) == 1) {
                dispose();
                if (__exchange_add(&m_weak_count, -1) == 1) {
                    destroy();
                }
            }
        }

        void add_weak_ref() {
            __add(&m_weak_count, 1);
        }

        void weak_release() {
            if (__exchange_add(&m_weak_count, -1) == 1) {
                destroy();
            }
        }

        int16_t get_use_count() const {
            return m_use_count;
        }

    private:
        __BaseCount(const __BaseCount &) = delete;

        __BaseCount &operator=(const __BaseCount &) = delete;
    };

    template<typename T>
    class __SharedPtr;

    template<typename T>
    class __WeakPtr;

    template<typename T>
    class __EnableSharedFromThis;

    template<typename T>
    class SharedPtr;

    template<typename T>
    class WeakPtr;

    template<typename T>
    struct Ownerless;

    template<typename T>
    struct EnableSharedFromThis;

    class __WeakCount;

    class __SharedCount;

    template<typename Ptr>
    class __CountedPtr : public __BaseCount {
    public:
        typedef Ptr pointer;

    protected:
        pointer m_ptr;

    public:
        explicit __CountedPtr(pointer ptr)
                : m_ptr(ptr) {
        }

        virtual void dispose() {
            delete m_ptr;
        }

        virtual void destroy() {
            delete this;
        }

        virtual void *get_deleter() {
            return nullptr;
        }

    private:
        __CountedPtr(const __CountedPtr &) = delete;

        __CountedPtr *operator=(const __CountedPtr &) = delete;
    };

    template<>
    inline void __CountedPtr<nullptr_t>::dispose() {
    }

    template<typename Ptr, typename DeleterType, typename AllocType>
    class __CountedDeleter : public __BaseCount {
    public:
        typedef Ptr pointer;
        typedef DeleterType deleter_type;
        typedef AllocType alloc_type;

    protected:
        pointer m_ptr;
        deleter_type m_deleter;
        alloc_type m_alloc;

    public:
        __CountedDeleter(pointer ptr, deleter_type deleter)
                : m_ptr(ptr),
                  m_deleter(deleter),
                  m_alloc(alloc_type()) {
        }

        __CountedDeleter(pointer ptr, deleter_type deleter, const alloc_type &alloc)
                : m_ptr(ptr),
                  m_deleter(deleter),
                  m_alloc(alloc) {
        }

        virtual void dispose() {
            m_deleter(m_ptr);
        }

        virtual void destroy() {
            alloc_type alloc(m_alloc);
            this->~__CountedDeleter();
            alloc.deallocate(this, 1);
        }

        virtual void *get_deleter() {
            return &m_deleter;
        }
    };

    template<typename T>
    struct __DestroyInplace {
        void operator()(T *ptr) const {
            if (ptr) {
                ptr->~T();
            }
        }
    };

    struct __MakeSharedTag {
    };

    template<typename T, typename Alloc>
    class __CountedPtrInplace : public __CountedDeleter<T *, __DestroyInplace<T>, Alloc> {
    public:
        typedef T val_type;
        typedef Alloc alloc_type;

    private:
        typedef __CountedDeleter<T *, __DestroyInplace<T>, Alloc> base_type;

        typename aligned_storage<sizeof(T), alignment_of<T>::value>::type m_storage;

    public:
        explicit __CountedPtrInplace(alloc_type alloc)
                : base_type(nullptr, __DestroyInplace<T>(), alloc),
                  m_storage() {
            void *ptr = &m_storage;
            ::new(ptr) T();
            base_type::m_ptr = static_cast<T *>(ptr);
        }

        template<typename... Args>
        __CountedPtrInplace(alloc_type alloc, Args &&... args)
                : base_type(nullptr, __DestroyInplace<T>(), alloc),
                  m_storage() {
            void *ptr = &m_storage;
            ::new(ptr) T(forward<Args>(args)...);
            base_type::m_ptr = static_cast<T *>(ptr);
        }

        virtual void destroy() {
            alloc_type alloc(base_type::m_alloc);
            this->~__CountedPtrInplace();
            alloc.deallocate(this, 1);
        }

    };

    class __SharedCount {
        friend class __WeakCount;

        template<typename T, typename Del>
        static __BaseCount *create_from_unique(
                UniquePtr<T, Del> &&up,
                typename enable_if<!is_referenceable<Del>::value>::type * = 0
        ) {
            return new __CountedDeleter<T *, Del, DefaultAlloc<T>>(up.get(), up.get_deleter());
        };

        __BaseCount *m_pi;

    public:
        constexpr __SharedCount()
                : m_pi(nullptr) {
        }

        template<typename Ptr>
        explicit __SharedCount(Ptr ptr)
                : m_pi(nullptr) {
            m_pi = new __CountedPtr<Ptr>(ptr);
        }

        template<typename Ptr, typename Del>
        __SharedCount(Ptr ptr, Del deleter)
                : m_pi(nullptr) {
            typedef DefaultAlloc<int> base_alloc;
            typedef __CountedDeleter<Ptr, Del, base_alloc> cd_type;
            typedef DefaultAlloc<cd_type> alloc_type;
            alloc_type alloc;
            m_pi = alloc(1);
            ::new(static_cast<void *>(m_pi)) cd_type(ptr, deleter);
        };

        template<typename Ptr, typename Del, typename Alloc>
        __SharedCount(Ptr ptr, Del deleter, Alloc alloc)
                : m_pi(nullptr) {
            typedef __CountedDeleter<Ptr, Del, Alloc> cd_type;
            m_pi = alloc(1);
            ::new(static_cast<void *>(m_pi)) cd_type(ptr, deleter, alloc);
        };

        template<typename T, typename Alloc, typename... Args>
        __SharedCount(__MakeSharedTag, T *, const Alloc &alloc, Args &&... args)
                : m_pi(nullptr) {
            typedef __CountedPtrInplace<T, Alloc> cp_type;
            m_pi = alloc(1);
            ::new(static_cast<void *>(m_pi)) cp_type(alloc, forward<Args>(args)...);
        };

        template<typename T, typename Del>
        __SharedCount(UniquePtr<T, Del> &&up)
                : m_pi(create_from_unique(move(up))) {
            up.release();
        };

        explicit __SharedCount(const __WeakCount &wc)
                : m_pi(wc.m_pi) {
            if (m_pi != nullptr) {
                m_pi->add_ref_lock();
            } // else bad weak pointer
        }

        ~__SharedCount() {
            if (m_pi != nullptr) {
                m_pi->release();
            }
        }

        __SharedCount(const __SharedCount &sc)
                : m_pi(sc.m_pi) {
            if (m_pi != nullptr) {
                m_pi->add_ref_copy();
            }
        }

        __SharedCount &operator=(const __SharedCount &sc) {
            __BaseCount *tmp = sc.m_pi;
            if (tmp != sc.m_pi) {
                if (tmp != nullptr) {
                    tmp->add_ref_copy();
                }
                if (m_pi != nullptr) {
                    m_pi->release();
                }
                m_pi = tmp;
            }
            return *this;
        }

        void swap(__SharedCount &sc) {
            __BaseCount *tmp = sc.m_pi;
            sc.m_pi = m_pi;
            m_pi = tmp;
        }

        int16_t get_use_count() const {
            return m_pi != nullptr ? m_pi->get_use_count() : static_cast<int16_t>(0);
        }

        bool unique() const {
            return this->get_use_count() == 1;
        }

        void *get_deleter() const {
            return m_pi ? m_pi->get_deleter() : nullptr;
        }

        bool less(const __SharedCount &sc) const {
            return m_pi < sc.m_pi;
        }

        bool less(const __WeakCount &wc) const {
            return m_pi < wc.m_pi;
        }

        friend inline bool operator==(const __SharedCount& sc1, const __SharedCount& sc2) {
            return sc1.m_pi == sc2.m_pi;
        }

    };

    class __WeakCount {
        friend class __SharedCount;

        __BaseCount *m_pi;

    public:
        constexpr __WeakCount()
                : m_pi(nullptr) {
        }

        __WeakCount(const __SharedCount &sc)
                : m_pi(sc.m_pi) {
            if (m_pi != nullptr) {
                m_pi->add_weak_ref();
            }
        }

        __WeakCount(const __WeakCount &wc)
                : m_pi(wc.m_pi) {
            if (m_pi != nullptr) {
                m_pi->add_weak_ref();
            }
        }

        ~__WeakCount() {
            if (m_pi != nullptr) {
                m_pi->weak_release();
            }
        }

        __WeakCount &operator=(const __SharedCount &sc) {
            __BaseCount *tmp = sc.m_pi;
            if (tmp != nullptr) {
                tmp->add_weak_ref();
            }
            if (m_pi != nullptr) {
                m_pi->weak_release();
            }
            m_pi = tmp;
            return *this;
        }

        __WeakCount &operator=(const __WeakCount &wc) {
            __BaseCount *tmp = wc.m_pi;
            if (tmp != nullptr) {
                tmp->add_weak_ref();
            }
            if (m_pi != nullptr) {
                m_pi->weak_release();
            }
            m_pi = tmp;
            return *this;
        }

        void swap(__WeakCount &wc) {
            __BaseCount *tmp = wc.m_pi;
            wc.m_pi = m_pi;
            m_pi = tmp;
        }

        int16_t get_use_count() const {
            return m_pi != nullptr ? m_pi->get_use_count() : static_cast<int16_t>(0);
        }

        bool less(const __SharedCount &sc) const {
            return m_pi < sc.m_pi;
        }

        bool less(const __WeakCount &wc) const {
            return m_pi < wc.m_pi;
        }
    };

}

#endif //EMBEDDEDCPLUSPLUS_SHAREDPTR_H