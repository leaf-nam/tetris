using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Threading;

namespace wpf.network
{
    class CNetwork
    {
        public TcpClient client;
        public NetworkStream stream;
        public StreamReader reader;
        public StreamWriter writer;

        public CNetwork(TcpClient client)
        {
            this.client = client;
            this.stream = client.GetStream();
            this.reader = new StreamReader(stream);
            this.writer = new StreamWriter(stream);
        }
    }

    public class NetworkService
    {
        private CNetwork connection;

        private Thread networkThread;

        private ConcurrentQueue<string> sendQueue = new();
        private ConcurrentQueue<string> recvQueue = new();

        private AutoResetEvent sendSignal = new(false);
        private volatile bool running = true;

        public static NetworkService instance;

        public static NetworkService GetInstance()
        {
            if (instance == null) instance = new NetworkService();
            return instance;
        }

        public void Open()
        {
            networkThread = new Thread(() =>
            {
                IPAddress addr = IPAddress.Any;
                TcpListener listener = new TcpListener(addr, 7777);
                listener.Start();

                var tcp = listener.AcceptTcpClient();
                connection = new CNetwork(tcp);

                ServerRun();
            });

            networkThread.IsBackground = true;
            networkThread.Start();
        }

        public void Connect(string ip, int port)
        {
            TcpClient client_ = new TcpClient();
            client_.Connect(ip, port);
            connection = new CNetwork(client_);

            networkThread = new Thread(new ThreadStart(SendLoop));
            networkThread.IsBackground = true;
            networkThread.Start();
        }

        public void Send(string msg)
        {
            sendQueue.Enqueue(msg);
            sendSignal.Set();
        }

        private void ServerRun()
        {
            try
            {
                while (running)
                {
                    string msg = connection.reader.ReadLine();
                    if (msg == null) break;
                    recvQueue.Enqueue(msg);
                }
            }
            catch (Exception e)
            {

            }
        }

        public bool TryReceive(out string msg)
        {
            return recvQueue.TryDequeue(out msg);
        }

        private void SendLoop()
        {
            while (running)
            {
                sendSignal.WaitOne();

                while (sendQueue.TryDequeue(out var msg))
                {
                    connection.writer.WriteLine(msg);
                    connection.writer.Flush();
                }
            }
        }

        public void Disconnect()
        {
            running = false;
            sendSignal.Set();

            try
            {
                connection?.stream?.Close();
                connection?.client?.Close();
            }
            catch { }

            if (networkThread?.IsAlive == true)
                networkThread.Join();
        }
    }
}
