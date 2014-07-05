using System;
using System.Collections.Specialized;
using System.Net;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Kaisekey {

    class Kaisekey : Form {

        [DllImport("hook")]
        private static extern void hook();
        [DllImport("hook")]
        private static extern void set(long[] count);

        public Kaisekey() {
            hook();
            Timer t = new Timer {
                Enabled = true,
                Interval = 900000,
            };
            t.Tick += delegate {
                int i;
                long[] count = new long[256 << 4];
                set(count);
                NameValueCollection nvc = new NameValueCollection();
                for(i = 0; i < count.Length; ++i) {
                    if(count[i] != 0)
                        nvc.Add(i.ToString(), count[i].ToString());
                }
                if(nvc.Count != 0)
                    Upload(nvc);
            };
        }

        private static async void Upload(NameValueCollection nvc) {
            using(WebClient wc = new WebClient()) {
                try {
                    await wc.UploadValuesTaskAsync("http://localhost/keylog.php", nvc);
                } catch(WebException) {
                }
            }
        }

        [STAThread]
        private static void Main() {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            new Kaisekey();
            Application.Run();
        }
    }
}
