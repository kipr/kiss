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
