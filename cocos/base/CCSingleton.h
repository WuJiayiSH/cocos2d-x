#ifndef __CC_SINGLETON_H__
#define __CC_SINGLETON_H__

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

template <typename T>
class Singleton
{
public:
    /**
     *  Gets the instance.
     */
    static T* getInstance()
    {
        if (s_sharedInstance == nullptr)
            s_sharedInstance = new (std::nothrow) T();

        return s_sharedInstance;
    }

    /**
     *  Destroys the instance.
     */
    static void destroyInstance()
    {
        delete s_sharedInstance;
        s_sharedInstance = nullptr;
    }

private:
    inline static T* s_sharedInstance = nullptr;
};

NS_CC_END
#endif
