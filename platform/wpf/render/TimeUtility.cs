using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp1.render
{
    public static class TimeUtility
    {
        public static string ConvertSecondToString(int sec)
        {
            int minute = sec / 60;
            int second = sec % 60;
            return string.Format("{0:D2}:{1:D2}", minute % 100, second);
        }
    }
}
