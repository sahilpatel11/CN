// Include necessary header files
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
// Use ns3 namespace
using namespace ns3;
// Enable log for this program
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
// Main function
int
main (int argc, char *argv[])
{
// Enable this program to read and parse command line arguments
CommandLine cmd;
cmd.Parse (argc, argv);
// Enable Log of echo applications
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
// Create nodes
NodeContainer nodes;
nodes.Create (2);
// Create a point-to-point channel and configure its attributes
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
// Create a Network Device Container
NetDeviceContainer devices;
devices = pointToPoint.Install (nodes);
// Install network stack on nodes
InternetStackHelper stack;
stack.Install (nodes);

// Set network address and subnet mask
Ipv4AddressHelper address;
address.SetBase ("10.1.1.0", "255.255.255.0");
// Assign IP address to every interface
Ipv4InterfaceContainer interfaces = address.Assign (devices);
// Configure a Server application
UdpEchoServerHelper echoServer (9);
// Install Server application on a specific node
ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
// Set start and stop time for Server application
serverApps.Start (Seconds (1.0));
serverApps.Stop (Seconds (10.0));
// Configure a Client application
UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
// Install Client application on a specific node
ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
// Set start and stop time for Client application
clientApps.Start (Seconds (2.0));
clientApps.Stop (Seconds (10.0));

pointToPoint.EnablePcapAll ("first");

MobilityHelper mobility;
mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
mobility.Install (nodes);
AnimationInterface anim ("first.xml");
AnimationInterface::SetConstantPosition (nodes.Get (0), 10, 25);
AnimationInterface::SetConstantPosition (nodes.Get (1), 40, 25);
anim.EnablePacketMetadata (true);
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("first.tr"));
// Run the simulation
Simulator::Run ();
// Release resources at the end of the simulation
Simulator::Destroy ();
return 0;
}
