#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <fstream>
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/tcp-header.h"
#include "ns3/traffic-control-module.h"
#include  <string>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PieTests");

std::stringstream filePlotQueueSize;
std::stringstream filePlotQueueDelay;
std::stringstream filePlotThroughput;

Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();

float startTime = 0.0;
float simDuration = 101;      // in seconds
float stopTime = startTime + simDuration;

void
SojournTimeTrace (Time sojournTime)
{
  double qDelay = sojournTime.ToDouble (Time::MS);
  std::ofstream fPlotQueueDisc (filePlotQueueDelay.str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueueDisc << Simulator::Now ().GetSeconds () << " " << qDelay << std::endl;
  fPlotQueueDisc.close ();
}

void
CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.1), &CheckQueueSize, queue);

  std::ofstream fPlotQueue (filePlotQueueSize.str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void
CheckThroughPut (Ptr<QueueDisc> queue)
{
  double now = Simulator::Now ().GetSeconds ();
  double sentBytes = queue->GetStats().nTotalDequeuedBytes - 
              (queue->GetStats().nTotalRequeuedBytes + queue->GetStats().nTotalDroppedBytesAfterDequeue);

  double thp = ((((sentBytes*8)/1000000)/now)/10)*100; // 5 because bottelenect bandwidth is 5Mbps
  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.1), &CheckThroughPut, queue);

  std::ofstream fPlotQueue3 (filePlotThroughput.str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue3 << now << " " << thp << std::endl;
  fPlotQueue3.close ();
}

int main (int argc, char *argv[])
{
  bool printPieStats = true;
  bool  isPcapEnabled = false;
  std::string  pathOut = "/home/siddeshlc8/siddeshlc/NS3/ns-allinone-3.29/ns-3.29/results/pie/";
  bool writeForPlot = true;
  std::string pcapFileName = "first-bulksend.pcap";

  uint32_t useTsp=1;

  

  CommandLine cmd;
  cmd.Parse (argc,argv);

  LogComponentEnable ("PieQueueDisc", LOG_LEVEL_INFO);

  std::string bottleneckBandwidth = "10Mbps";
  std::string bottleneckDelay = "50ms";

  std::string accessBandwidth = "10Mbps";
  std::string accessDelay = "5ms";

  NodeContainer source;
  source.Create (5);

  NodeContainer gateway;
  gateway.Create (2);

  NodeContainer sink;
  sink.Create (1);


  // Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue ("13p"));
  // Config::SetDefault ("ns3::PfifoFastQueueDisc::Limit", StringValue ("50p"));

  Config::SetDefault ("ns3::TcpSocket::DelAckTimeout", TimeValue(Seconds (0)));
  Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (10));
  Config::SetDefault ("ns3::TcpSocketBase::LimitedTransmit", BooleanValue (false));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000));
  Config::SetDefault ("ns3::TcpSocketBase::WindowScaling", BooleanValue (true));

  Config::SetDefault ("ns3::PieQueueDisc::MaxSize", StringValue ("100p"));
  Config::SetDefault ("ns3::PieQueueDisc::MeanPktSize", UintegerValue (1000));
  Config::SetDefault ("ns3::PieQueueDisc::DequeueThreshold", UintegerValue (10000));
  Config::SetDefault ("ns3::PieQueueDisc::QueueDelayReference", TimeValue (Seconds (0.015)));
  Config::SetDefault ("ns3::PieQueueDisc::MaxBurstAllowance", TimeValue (Seconds (0.1)));
  Config::SetDefault ("ns3::PieQueueDisc::UseTimeStamp", UintegerValue (useTsp));

  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  internet.InstallAll ();

  TrafficControlHelper tchPfifo;
  uint16_t handle = tchPfifo.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  tchPfifo.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxSize", StringValue ("1000p"));

  TrafficControlHelper tchPie;
  tchPie.SetRootQueueDisc ("ns3::PieQueueDisc");

  // Create and configure access link and bottleneck link
  PointToPointHelper accessLink;
  accessLink.SetQueue ("ns3::DropTailQueue");
  accessLink.SetDeviceAttribute ("DataRate", StringValue (accessBandwidth));
  accessLink.SetChannelAttribute ("Delay", StringValue (accessDelay));

  NetDeviceContainer devices[5];
  for (int i = 0; i < 5; i++)
    {
      devices[i] = accessLink.Install (source.Get (i), gateway.Get (0));
      tchPfifo.Install (devices[i]);
    }

  NetDeviceContainer devices_sink;
  devices_sink = accessLink.Install (gateway.Get (1), sink.Get (0));
  tchPfifo.Install (devices_sink);

  PointToPointHelper bottleneckLink;
  bottleneckLink.SetQueue ("ns3::DropTailQueue");
  bottleneckLink.SetDeviceAttribute ("DataRate", StringValue (bottleneckBandwidth));
  bottleneckLink.SetChannelAttribute ("Delay", StringValue (bottleneckDelay));

  NetDeviceContainer devices_gateway;
  devices_gateway = bottleneckLink.Install (gateway.Get (0), gateway.Get (1));
  // only backbone link has Pi queue disc
  QueueDiscContainer queueDiscs = tchPie.Install (devices_gateway);

  NS_LOG_INFO ("Assign IP Addresses");
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.255.255.0");

  // Configure the source and sink net devices
  // and the channels between the source/sink and the gateway
  // Ipv4InterfaceContainer sink_Interfaces;
  Ipv4InterfaceContainer interfaces[5];
  Ipv4InterfaceContainer interfaces_sink;
  Ipv4InterfaceContainer interfaces_gateway;

  for (int i = 0; i < 5; i++)
    {
      address.NewNetwork ();
      interfaces[i] = address.Assign (devices[i]);
    }

  address.NewNetwork ();
  interfaces_sink = address.Assign (devices_sink);

  address.NewNetwork ();
  interfaces_gateway = address.Assign (devices_gateway);


  uint16_t port = 50000;
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);

  // Configure application
  // AddressValue remoteAddress (InetSocketAddress (sink_Interfaces.GetAddress (0, 0), port));
  AddressValue remoteAddress (InetSocketAddress (interfaces_sink.GetAddress (1), port));
  for (uint16_t i = 0; i < source.GetN (); i++)
    {
      BulkSendHelper ftp ("ns3::TcpSocketFactory", Address ());
      ftp.SetAttribute ("Remote", remoteAddress);
      ftp.SetAttribute ("SendSize", UintegerValue (1000));

      ApplicationContainer sourceApp = ftp.Install (source.Get (i));
      sourceApp.Start (Seconds (0));
      sourceApp.Stop (Seconds (stopTime - 1));
    }
      sinkHelper.SetAttribute ("Protocol", TypeIdValue (TcpSocketFactory::GetTypeId ()));
      ApplicationContainer sinkApp = sinkHelper.Install (sink);
      sinkApp.Start (Seconds (0));
      sinkApp.Stop (Seconds (stopTime));

  NS_LOG_INFO ("Initialize Global Routing.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  if (writeForPlot)
    {
      std::string ext = "-UseTsp";

      if(useTsp == 0){
        ext = "-DepRate";
      }

      filePlotQueueDelay << pathOut << "pie-queue-delay-first"+ext+".plotme";
      filePlotQueueSize << pathOut  << "pie-queue-size-first"+ext+".plotme";
      filePlotThroughput << pathOut  << "pie-throughput-first"+ext+".plotme";

      remove (filePlotQueueDelay.str ().c_str ());
      remove (filePlotQueueSize.str ().c_str ());
      remove (filePlotThroughput.str ().c_str ());
      Ptr<QueueDisc> queue = queueDiscs.Get (0);

      Simulator::ScheduleNow (&CheckQueueSize, queue);
      Simulator::ScheduleNow (&CheckThroughPut, queue);

      queue->TraceConnectWithoutContext ("SojournTime", MakeCallback (&SojournTimeTrace));
    }

  if (isPcapEnabled)
    {
      bottleneckLink.EnablePcap (pcapFileName, gateway, false);
    }

  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();

  if (printPieStats)
    {
      PieQueueDisc::Stats st = StaticCast<PieQueueDisc> (queueDiscs.Get (0))->GetStats ();
      std::cout << "*** PIE stats from Node 2 queue ***" << std::endl;
      std::cout << "\t " << st.GetNDroppedPackets (PieQueueDisc::UNFORCED_DROP)
                << " drops due to prob mark" << std::endl;
      std::cout << "\t " << st.GetNDroppedPackets (PieQueueDisc::FORCED_DROP)
                << " drops due to queue limits" << std::endl;
    }

  Simulator::Destroy ();
  return 0;
}