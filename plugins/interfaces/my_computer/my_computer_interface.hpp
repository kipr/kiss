#ifndef _MY_COMPUTER_INTERFACE_HPP_
#define _MY_COMPUTER_INTERFACE_HPP_

#include "interface.hpp"

namespace kiss
{
    namespace target
    {
        class MyComputerInterface : public QObject, public kiss::target::Interface
        {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "com.kipr.kiss.Interface/1.1" FILE "my_computer_interface.json")
        Q_INTERFACES(kiss::target::Interface)
        public:
            MyComputerInterface();
            virtual kiss::target::TargetPtr createTarget(const QString &address);
            virtual const bool scan(kiss::target::InterfaceResponder *responder);
            virtual void invalidateResponder();
        };
    }
}

#endif
