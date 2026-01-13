using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace tetris_server
{
    class CServer
    {
        private TcpClient client;
        private NetworkStream stream;
        private StreamReader reader;
        private StreamWriter writer;
        private string id;

        public CServer(TcpClient client)
        {
            this.client = client;
            this.stream = client.GetStream();
            this.reader = new StreamReader(stream);
            this.writer = new StreamWriter(stream);
        }

        // 보드 데이터와 스코어를 한 줄의 문자열로 만들어 전송하는 함수
        public void sendBoard(int[,] board, int score)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("/board ");

            for (int i = 0; i < 22; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    sb.Append(board[i, j]);
                }
            }
            sb.Append(" " + score);

            writer.WriteLine(sb.ToString());
            writer.Flush();
        }

        //담당 클라이언트에 메세지 전송
        public void sendMsg(string msg)
        {
            writer.WriteLine(msg);
            writer.Flush();
        }

        // 통신 메인 로직
        public void run()
        {
            // 클라이언트 아이디 받기
            id = reader.ReadLine();
            string msg = id + " 님이 입장했습니다";
            Console.WriteLine(msg);

            // 새로운 클라이언트 접속 알리기
            lock (server.clients)
            {
                foreach (CServer client in server.clients)
                {
                    client.sendMsg(msg);
                }
            }

            try
            {
                while (true)
                {
                    msg = reader.ReadLine();

                    if (msg.StartsWith("/board"))
                    {
                        HandleBoardData(msg);
                    }
                    else if (msg.StartsWith("/exit"))
                    {
                        HandleExitData(msg);
                        break;
                    }
                    else
                    {
                        lock (server.clients)
                        {
                            foreach (CServer client in server.clients)
                            {
                                client.sendMsg(id + ": \"" + msg + "\"");
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("서버플레이어 통신오류남: " + e.Message);
            }
            client.Close();
            lock (server.clients)
            {
                server.clients.Remove(this);
            }
        }

        private void HandleBoardData(string msg)
        {
            // [0]:태그 [1]:보드데이터 [2]:스코어
            string[] parts = msg.Split(' ');
            if (parts.Length != 3) return;

            // 서버 콘솔 확인용
             Console.WriteLine($"{id}의 데이터 수신 중...");

            // 나(this)를 제외한 모든 클라이언트에게 수신한 메시지 그대로 전달
            lock (server.clients)
            {
                foreach (CServer otherClient in server.clients)
                {
                    if (otherClient != this) // 나 자신에게는 보낼 필요 없음
                    {
                        otherClient.sendMsg(msg);
                    }
                }
            }
        }

        private void HandleExitData(string msg)
        {
            msg = id + " 님이 나갔습니다";
            Console.WriteLine(msg);
            lock (server.clients)  // 공유자원이기때문에 락으로 동기화
            {
                foreach (CServer client in server.clients)
                {
                    client.sendMsg(msg);
                }
            }
        }
    }

    internal class server
    {
        //채팅방: 접속한 클라이언트 객체를 저장
        //쓰레드들의 공유자원
        public static List<CServer> clients = new List<CServer>();
        static void Main(string[] args)
        {
            // 기본값 설정
            int port = 4321;
            if (args.Length > 0)
            {
                int.TryParse(args[0], out port);
            }

            IPAddress addr = new IPAddress(0);
            TcpListener listener = new TcpListener(addr, port);
            listener.Start();
            Console.WriteLine("테트리스 서버가 열렸습니다.");
            while (true)
            {
                TcpClient client = listener.AcceptTcpClient();
                CServer server = new CServer(client);
                clients.Add(server);

                Thread clientThread = new Thread(new ThreadStart(server.run));
                clientThread.IsBackground = true;
                clientThread.Start();
            }
        }
    }
}
