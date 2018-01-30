#ifndef GEAROEMIX_CORE_EVENT_MOVEMENT_HPP
#define GEAROEMIX_CORE_EVENT_MOVEMENT_HPP

#include "cr-ev-event.hpp"

namespace gearoenix {
namespace core {
    namespace event {
        namespace movement {
            class Movement : public Event {
            public:
                typedef enum : core::Id {
                    MOUSE,
                    // todo others will be added whenever needed
                } MovementType;

            private:
                const MovementType movement_type;

            protected:
                Movement(MovementType t);

            public:
                virtual ~Movement();
                MovementType get_type() const;
            };
        }
    }
}
}
#endif