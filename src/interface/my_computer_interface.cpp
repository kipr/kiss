#include "my_computer_interface.hpp"
#include "kovan_proto_target.hpp"
#include "application_finder.hpp"
#include "process_target.hpp"
#include <kovanserial/tcp_serial.hpp>

#include <QtPlugin>

using namespace kiss;
using namespace kiss::target;

MyComputerInterface::MyComputerInterface()
    : Interface("My Computer")
{
}

kiss::target::TargetPtr MyComputerInterface::createTarget(const QString &address)
{
    return TargetPtr();
}

const bool MyComputerInterface::scan(kiss::target::InterfaceResponder *responder)
{
    responder->targetScanStarted(this);
    TcpSerial *serial = new TcpSerial("127.0.0.1", KOVAN_SERIAL_PORT + 1);
    KovanProtoTarget *proto = new KovanProtoTarget(serial, this);
    proto->fillDisplayName("My Computer");
    proto->fillCommPort("localhost");
    
    responder->targetFound(this, TargetPtr(new ProcessTarget(TargetPtr(proto), ApplicationFinder::platform().applicationPath("cs2"), QStringList() << "-c")));
    emitScanFinished();
    return true;
}

void MyComputerInterface::invalidateResponder()
{
    
}

