#include "my_ks2_interface.hpp"
#include "kovan_proto_target.hpp"
#include "application_finder.hpp"
#include "process_target.hpp"
#include <kovanserial/tcp_serial.hpp>

#include <QtPlugin>

using namespace kiss;
using namespace kiss::target;

MyKs2Interface::MyKs2Interface()
    : Interface("My Link Simulator")
{
}

kiss::target::TargetPtr MyKs2Interface::createTarget(const QString &address)
{
    return TargetPtr();
}

const bool MyKs2Interface::scan(kiss::target::InterfaceResponder *responder)
{
    responder->targetScanStarted(this);
    TcpSerial *serial = new TcpSerial("127.0.0.1", KOVAN_SERIAL_PORT + 2);
    KovanProtoTarget *proto = new KovanProtoTarget(serial, this);
    proto->fillDisplayName("My Link Simulator");
    proto->fillCommPort("localhost");
    
    responder->targetFound(this, TargetPtr(new ProcessTarget(TargetPtr(proto), ApplicationFinder::platform().applicationPath("ks2"))));
    return true;
}

void MyKs2Interface::invalidateResponder()
{
    
}
