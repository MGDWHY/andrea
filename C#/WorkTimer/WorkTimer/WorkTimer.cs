using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace WorkTimer
{
    [Serializable]

    public delegate void WorkTimerTick();

    class WorkTimer
    {
        private Thread thread;
        private string name;
        private int time;
        private volatile bool active;

        public WorkTimerTick Tick;
        
        public string Name
        {
            get { return name; }
        }

        public int WorkTime
        {
            get { return time; }
            set { time = value; }
        }

        public bool Active
        {
            get { return active; }
            set { active = value;}
        }


        public WorkTimer(string name)
        {
            this.time = 0;
            this.name = name;
            this.active = false;
            this.thread = new Thread(TimerLoop);
            this.thread.Start();
        }

        ~WorkTimer()
        {
            thread.Interrupt();
        }

        private void TimerLoop()
        {
            while (thread.IsAlive)
            {
                Thread.Sleep(1000);
                if (active)
                {
                    time++;
                    Tick();
                }
            }
        }

    }
}
