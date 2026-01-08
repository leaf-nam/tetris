
public class Callbacks
{
    public delegate string ScanCallback();
    public delegate void BackgroundCallback(int value);
    public delegate void BoardCallback(int value);
    public delegate void TimerCallback(int value);
    public delegate void NextBlockCallback(int value);
    public delegate void HoldCallback(int value);
    public delegate void ScoreCallback(int value);
    public delegate void LevelCallback(int value);

    public ScanCallback scan_callback {  get; set; }
    public BackgroundCallback background_callback { get; set; }
    public BoardCallback board_callback { get; set; }
    public TimerCallback timer_callback { get; set; }
    public NextBlockCallback next_block_callback { get; set; }
    public HoldCallback hold_callback { get; set; }
    public ScoreCallback score_callback { get; set; }
    public LevelCallback level_callback { get; set; }
}