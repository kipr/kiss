#ifndef _MY_KS2_INTERFACE_HPP_
#define _MY_KS2_INTERFACE_HPP_

#include "interface.hpp"

namespace kiss
{
    namespace target
    {
        class MyKs2Interface : public QObject, public kiss::target::Interface
        {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "com.kipr.kiss.Interface/1.1" FILE "my_ks2_interface.json")
        Q_INTERFACES(kiss::target::Interface)
        public:
            MyKs2Interface();
            virtual kiss::target::TargetPtr createTarget(const QString &address);
            virtual const bool scan(kiss::target::InterfaceResponder *responder);
            virtual void invalidateResponder();
        };
    }
}

#endif
