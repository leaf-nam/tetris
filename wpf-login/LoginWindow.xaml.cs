using System.Diagnostics;
using System.IO;
using System.Net.Sockets;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
        }

        

        private void BtnConnect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                TcpClient tcp = new TcpClient();
                tcp.Connect(TxtIp.Text, int.Parse(TxtPort.Text));

                // 메인 윈도우 생성 시 통신 객체 전달
                MainWindow gameWindow = new MainWindow(tcp, TxtId.Text);
                gameWindow.Show();
                this.Close();
            }
            catch
            {
                MessageBox.Show("서버를 찾을 수 없습니다!");
            }
        }

        private void BtnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string serverPath = "tetris_server.exe";

                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = serverPath,
                    Arguments = TxtPort.Text,
                    UseShellExecute = true, // 새 콘솔 창에서 실행
                    CreateNoWindow = false
                };

                Process.Start(startInfo);
                MessageBox.Show("서버가 실행되었습니다.\n"+"IP: "+TxtIp.Text+"\n"+"Port: "+TxtPort.Text);

                try
                {
                    TcpClient tcp = new TcpClient();
                    tcp.Connect(TxtIp.Text, int.Parse(TxtPort.Text));

                    // 닉네임 전송 및 게임 창 열기
                    MainWindow gameWindow = new MainWindow(tcp, TxtId.Text);
                    gameWindow.Show();
                    this.Close();
                }
                catch
                {
                    MessageBox.Show("접속에 실패했습니다.");
                }

            }
            catch
            {
                MessageBox.Show("서버 파일을 찾을 수 없습니다.");
            }
        }
    }
}