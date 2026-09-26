#pragma once

#include <memory>

namespace sdl {

    template<auto Fn>
    struct CustomDeleter {
        template<typename T>
        void operator()(T* ptr) const noexcept {
            if (ptr != nullptr) {
                Fn(ptr);
            }
        }
    };

    template<typename T, auto DestroyFn>
    using UniqueResource = std::unique_ptr<T, CustomDeleter<DestroyFn>>;

} // namespace sdl
