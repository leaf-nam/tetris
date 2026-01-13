using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Windows;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private NetworkStream stream;
        private StreamReader reader;
        private StreamWriter writer;
        private TcpClient client;

        // 생성자를 수정하여 연결된 정보를 받습니다.
        public MainWindow(TcpClient connectedClient, string userId)
        {
            InitializeComponent();

            stream = connectedClient.GetStream();
            reader = new StreamReader(stream);
            writer = new StreamWriter(stream);

            // 첫 접속시 서버에 닉네임 전송
            sendMsg(userId);
            TxtId.Text = userId;

            // 수신 스레드 시작
            Thread receiveThread = new Thread(run);
            receiveThread.IsBackground = true;
            receiveThread.Start();
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

        // 100ms마다 상대방에게 전송하는 함수
        private void SendLoop()
        {
            while (true)
            {
                // 자신의 게임 데이터
                int[,] myBoardData = new int[22, 10];
                myBoardData[0, 0] = 1; // 테스트용
                int myScore = 999;
                sendBoard(myBoardData, myScore);
                Thread.Sleep(100);  // 0.1초
            }
        }

        // 상대방에게 메세지 전송 함수
        public void sendMsg(string msg)
        {
            writer.WriteLine(msg);
            writer.Flush();
        }

        // 통신 메인 로직
        public void run()
        {
            // 주기적 송신 스레드
            Thread updateThread = new Thread(SendLoop);
            updateThread.IsBackground = true;
            updateThread.Start();

            try
            {
                while (true)
                {
                    string msg = reader.ReadLine();
                    if (msg == null) break;
                    if (msg.StartsWith("/board"))
                    {
                        //HandleBoardData(msg);
                        Dispatcher.Invoke(() => HandleBoardData(msg));
                    }
                    else
                    {
                        // 채팅
                        //Console.WriteLine("\n" + msg);
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("클라이언트 통신오류남: " + e.Message);
            }
            finally
            {
                client.Close();
                Console.WriteLine("서버와 연결이 종료되었습니다.");
            }
        }
        private void HandleBoardData(string msg)
        {

            // [0]:태그 [1]:보드데이터 [2]:스코어
            string[] parts = msg.Split(' ');
            if (parts.Length != 3) return;

            string boardData = parts[1];
            int score = int.Parse(parts[2]);

            // 텍스트박스에 예쁘게 출력하기 위한 포맷팅
            StringBuilder display = new StringBuilder();
            display.AppendLine($"[상대방 스코어: {score}]");
            display.AppendLine("--------------------------");

            for (int i = 0; i < 22; i++)
            {
                // 220자 텍스트를 10글자씩 잘라서 한 줄씩 추가
                string row = boardData.Substring(i * 10, 10);
                // 가독성을 위해 숫자 사이에 공백 추가 (예: 0010 -> 0 0 1 0)
                display.AppendLine(string.Join(" ", row.ToCharArray()));
            }

            // UI 텍스트박스에 업데이트
            TxtIntArray.Text = display.ToString();
        }


    }
}