#pragma once

#define NONCOPYABLE(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;


