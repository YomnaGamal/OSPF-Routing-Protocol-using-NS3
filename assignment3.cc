#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Assignment3");

int 
main (int argc, char *argv[])
{

  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");

  NodeContainer c;
  c.Create (6);
  NodeContainer n0n1 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer n0n2 = NodeContainer (c.Get (0), c.Get (2));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer n1n3 = NodeContainer (c.Get (1), c.Get (3));
  NodeContainer n1n4 = NodeContainer (c.Get (1), c.Get (4));
  NodeContainer n2n4 = NodeContainer (c.Get (2), c.Get (4));
  NodeContainer n3n4 = NodeContainer (c.Get (3), c.Get (4));
  NodeContainer n4n5 = NodeContainer (c.Get (4), c.Get (5));
  
  InternetStackHelper internet;
  internet.Install (c);

  // As in Example of Global Routing Protocol in NS-3
  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  NetDeviceContainer d0d2 = p2p.Install (n0n2);
  NetDeviceContainer d1d2 = p2p.Install (n1n2);
  NetDeviceContainer d1d3 = p2p.Install (n1n3);
  NetDeviceContainer d1d4 = p2p.Install (n1n4);
  NetDeviceContainer d2d4 = p2p.Install (n2n4);
  NetDeviceContainer d3d4 = p2p.Install (n3n4);
  NetDeviceContainer d4d5 = p2p.Install (n4n5);

  // We create the channels first without any IP addressing information
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));
  //NetDeviceContainer d2345 = csma.Install (n2345);


  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
 /*
  ipv4.SetBase ("172.16.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i6 = ipv4.Assign (d1d6);
*/

  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i3 = ipv4.Assign (d1d3);

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);

  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i4 = ipv4.Assign (d2d4);

  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i4 = ipv4.Assign (d3d4);

  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);


  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



  // Create a server and 5 clients
  // As in Example of tutorail for NS-3 in https://www.nsnam.org/docs/release/3.18/doxygen/first_8cc_source.html

  NS_LOG_INFO ("Create Applications.");

  
  uint16_t port = 9;
  UdpEchoServerHelper echoServer (port);

  ApplicationContainer server = echoServer.Install (c.Get (5));
  server.Start (Seconds (1.0));
  server.Stop (Seconds (15.0));

  UdpEchoClientHelper echoClient (i4i5.GetAddress (1), port);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer client0 = echoClient.Install (c.Get (0));
  client0.Start (Seconds (2.0));
  client0.Stop (Seconds (15.0));

  ApplicationContainer client1 = echoClient.Install (c.Get (1));
  client1.Start (Seconds (4.0));
  client1.Stop (Seconds (15.0));

  ApplicationContainer client2 = echoClient.Install (c.Get (2));
  client2.Start (Seconds (6.0));
  client2.Stop (Seconds (15.0));

  ApplicationContainer client3 = echoClient.Install (c.Get (3));
  client3.Start (Seconds (8.0));
  client3.Stop (Seconds (15.0));

  ApplicationContainer client4 = echoClient.Install (c.Get (4));
  client4.Start (Seconds (10.0));
  client4.Stop (Seconds (15.0));

  // Trace routing tables 
  Ipv4GlobalRoutingHelper g;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("Assignment3.routes", std::ios::out);
  g.PrintRoutingTableAllAt (Seconds (12), routingStream);

  // make animation for our topology
  // From this tutorial of NS-3 https://www.nsnam.org/docs/release/3.13/models/html/animation.html
  AnimationInterface anim ("assignment3.xml");

  // set positions as in assignment pdf

  anim.SetConstantPosition(c.Get(0), 10,20);
  anim.SetConstantPosition(c.Get(1), 20,30);
  anim.SetConstantPosition(c.Get(2), 20,10);
  anim.SetConstantPosition(c.Get(3), 40,30);
  anim.SetConstantPosition(c.Get(4), 40,10);
  anim.SetConstantPosition(c.Get(5), 50,10);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
