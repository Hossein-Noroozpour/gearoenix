#ifndef GEAROENIX_CORE_SYNC_END_CALLER_HPP
#define GEAROENIX_CORE_SYNC_END_CALLER_HPP
#include "../../system/sys-log.hpp"
#include "../cr-build-configuration.hpp"
#include <functional>
#include <memory>
#include <type_traits>

namespace gearoenix {
namespace core {
    namespace sync {
        template <typename T>
        class EndCaller {
        private:
            class Caller {
            public:
                std::function<void(std::shared_ptr<T>)> f;
                std::shared_ptr<T> data = nullptr;
                Caller(std::function<void(std::shared_ptr<T>)> f)
                    : f(f)
                {
                }
                ~Caller()
                {
#ifdef GX_DEBUG_MODE
                    if (nullptr == data)
                        GXUNEXPECTED;
#endif
                    f(data);
                }
            };
            std::shared_ptr<Caller> caller;

        public:
            EndCaller(std::function<void(std::shared_ptr<T>)> f)
                : caller(new Caller(f))
            {
            }
            EndCaller(const EndCaller& o)
                : caller(o.caller)
            {
            }
            void set_data(const std::shared_ptr<T>& data)
            {
                caller->data = data;
            }
        };
        typedef struct {
        } EndCallerIgnore;
#ifdef GX_DEBUG_MODE
        template <>
        class EndCaller<EndCallerIgnore> {
        private:
            class Caller {
            public:
                std::function<void()> f;
                Caller(std::function<void()> f)
                    : f(f)
                {
                }
                ~Caller()
                {
                    f();
                }
            };
            std::shared_ptr<Caller> caller;

        public:
            EndCaller(std::function<void()> f)
                : caller(new Caller(f))
            {
            }

            EndCaller(const EndCaller& o)
                : caller(o.caller)
            {
            }
        };
#endif
    }
}
}
#endif