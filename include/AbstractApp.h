#pragma once
#include <ServiceContainer.h>

class AbstractApp {
    public:
        static ServiceContainer* sc; 
        virtual void execute(void) = 0;
};