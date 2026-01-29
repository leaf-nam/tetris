class IMultiRenderer : IRenderer
{
     virtual void render_game_over() = 0;

     virtual void render_other_game_over(Packet& pkt) = 0;

     virtual void render_win() = 0;

     virtual void render_other_win(Packet& pkt) = 0;

     virtual void render_ip_recv() = 0;

     virtual void render_char(char c) = 0;
}