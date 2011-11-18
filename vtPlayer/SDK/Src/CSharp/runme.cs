using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;


namespace WindowsApplication
{
    /// <summary>
    /// Summary description for Form1.
    /// </summary>
    public class Form1 : System.Windows.Forms.Form
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;

        public void CreateVTWindow(IntPtr win)
        {


            vtWin = new vtWindow();
            SWIGTYPE_p_long l = new SWIGTYPE_p_long(win, true);
            vtWin.CreateAsChild(l);


        }


        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            if (vtWin != null)
            {
                vtWin.Terminate(0);
            }
        }

        //private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button sendmessage;

        private runme.vtPanel panel1;
        //some temp test values for messaging : 
        private int m_intMTest = 0;
        private float m_floatMTest = 0;
        private String m_strMTest = "";

        public int startWidth = 400;
        public int startHeight = 300;

        private System.Windows.Forms.Button sendMsgFloat;
        private System.Windows.Forms.Button sendMsgStr;
        public System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button CheckVirtoolsMsgBtn;
        private System.Windows.Forms.Button Resize;
        private System.Windows.Forms.Button PauseBtn;
        private System.Windows.Forms.Button PlayBtn;


        public vtWindow vtWin = null;
        public vtWindow VtWin
        {
            get { return vtWin; }
            set { vtWin = value; }
        }

        public Form1()
        {

            InitializeComponent();
            this.SuspendLayout();

            //we init our player, be aware about "OwnerDrawed" entry in the player.ini!
            //you must set this to 1, otherwise its not correct displayed in this form !
            CreateVTWindow(panel1.Handle);
            panel1.Init(this, vtWin);


            //we retrieve the players render bounds : 
            int playerWidth = vtWin.GetWidth();
            int playerHeight = vtWin.GetHeight();



            // and we re-adjust our panel ! 

            //this.panel1.Size = new System.Drawing.Size(playerWidth, playerHeight);

            vtWin.Run();

            //System.Threading.Thread.Sleep(100);

            vtWin.UpdateRenderSize(playerWidth, playerHeight);

            this.PerformLayout();





        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new runme.vtPanel();
            this.sendmessage = new System.Windows.Forms.Button();
            this.sendMsgFloat = new System.Windows.Forms.Button();
            this.sendMsgStr = new System.Windows.Forms.Button();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.CheckVirtoolsMsgBtn = new System.Windows.Forms.Button();
            this.Resize = new System.Windows.Forms.Button();
            this.PauseBtn = new System.Windows.Forms.Button();
            this.PlayBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Location = new System.Drawing.Point(344, 8);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(400, 300);
            this.panel1.TabIndex = 0;
            // 
            // sendmessage
            // 
            this.sendmessage.Location = new System.Drawing.Point(8, 8);
            this.sendmessage.Name = "sendmessage";
            this.sendmessage.Size = new System.Drawing.Size(96, 24);
            this.sendmessage.TabIndex = 1;
            this.sendmessage.Text = "sendmessageInt";
            this.sendmessage.Click += new System.EventHandler(this.sendmessage_Click);
            // 
            // sendMsgFloat
            // 
            this.sendMsgFloat.Location = new System.Drawing.Point(8, 40);
            this.sendMsgFloat.Name = "sendMsgFloat";
            this.sendMsgFloat.Size = new System.Drawing.Size(96, 24);
            this.sendMsgFloat.TabIndex = 2;
            this.sendMsgFloat.Text = "sendMsgFloat";
            this.sendMsgFloat.Click += new System.EventHandler(this.sendMsgFloat_Click);
            // 
            // sendMsgStr
            // 
            this.sendMsgStr.Location = new System.Drawing.Point(8, 72);
            this.sendMsgStr.Name = "sendMsgStr";
            this.sendMsgStr.Size = new System.Drawing.Size(96, 24);
            this.sendMsgStr.TabIndex = 3;
            this.sendMsgStr.Text = "sendMsgStr";
            this.sendMsgStr.Click += new System.EventHandler(this.sendMsgStr_Click);
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(8, 144);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(320, 160);
            this.richTextBox1.TabIndex = 4;
            this.richTextBox1.Text = "playerout";
            // 
            // CheckVirtoolsMsgBtn
            // 
            this.CheckVirtoolsMsgBtn.Location = new System.Drawing.Point(120, 8);
            this.CheckVirtoolsMsgBtn.Name = "CheckVirtoolsMsgBtn";
            this.CheckVirtoolsMsgBtn.Size = new System.Drawing.Size(120, 24);
            this.CheckVirtoolsMsgBtn.TabIndex = 5;
            this.CheckVirtoolsMsgBtn.Text = "CheckVirtoolsMsg";
            this.CheckVirtoolsMsgBtn.Click += new System.EventHandler(this.button1_Click);
            // 
            // Resize
            // 
            this.Resize.Location = new System.Drawing.Point(120, 40);
            this.Resize.Name = "Resize";
            this.Resize.Size = new System.Drawing.Size(120, 24);
            this.Resize.TabIndex = 6;
            this.Resize.Text = "Resize";
            this.Resize.Click += new System.EventHandler(this.Resize_Click);
            // 
            // PauseBtn
            // 
            this.PauseBtn.Location = new System.Drawing.Point(120, 72);
            this.PauseBtn.Name = "PauseBtn";
            this.PauseBtn.Size = new System.Drawing.Size(48, 24);
            this.PauseBtn.TabIndex = 7;
            this.PauseBtn.Text = "Pause";
            this.PauseBtn.Click += new System.EventHandler(this.PauseBtn_Click);
            // 
            // PlayBtn
            // 
            this.PlayBtn.Location = new System.Drawing.Point(176, 72);
            this.PlayBtn.Name = "PlayBtn";
            this.PlayBtn.Size = new System.Drawing.Size(40, 24);
            this.PlayBtn.TabIndex = 8;
            this.PlayBtn.Text = "Play";
            this.PlayBtn.Click += new System.EventHandler(this.PlayBtn_Click);
            // 
            // Form1
            // 
            this.AutoScale = false;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(768, 325);
            this.Controls.Add(this.PlayBtn);
            this.Controls.Add(this.PauseBtn);
            this.Controls.Add(this.Resize);
            this.Controls.Add(this.CheckVirtoolsMsgBtn);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.sendMsgStr);
            this.Controls.Add(this.sendMsgFloat);
            this.Controls.Add(this.sendmessage);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.ResumeLayout(false);

        }
        #endregion
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        /// 
        [STAThread]
        static void Main()
        {
            Application.Run(new Form1());
        }


        /*
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (vtWin!=null)
            {
                SWIGTYPE_p_long win  = new SWIGTYPE_p_long(m.HWnd,true);
                SWIGTYPE_p_long wp  = new SWIGTYPE_p_long(m.WParam,true);


                SWIGTYPE_p_long lp  = new SWIGTYPE_p_long(m.LParam ,true);

                vtWin.WndProc(win,m.Msg,wp,lp);

            }
        }*/
        private void sendmessage_Click(object sender, System.EventArgs e)
        {

            if (vtWin != null)
            {

                //we send a message to our player : 

                //when the player could'nt found the object by this name, then it sends
                //a broadcast message !


                String objectName = "masterDummy";

                //this message name should exist inside vt !
                String messageName = "csharp";


                //the function takes 3 reserved parameters. you can do what ever you want  with it
                int id0 = 0;
                int id1 = 1;
                int id2 = 2;
                m_intMTest++;
                vtWin.SendMessage(objectName, messageName, id0, id1, id2, m_intMTest);

                //inside vt you only have to attach a MessageWaiter to the object and also 
                //a GetMessageDataBB after the waiter with the appropriate output parameters : 
                // int,int,int,int in this case here !
            }
        }

        private void sendMsgFloat_Click(object sender, System.EventArgs e)
        {

            if (vtWin != null)
            {
                //we send a message to our player : 

                //when the player could'nt found the object by this name, then it sends
                //a broadcast message !
                String objectName = "masterDummy";

                //this message name should exist inside vt !
                String messageName = "csharpFloat";

                //the function takes 3 reserved parameters. you can do what ever you want  with it
                int id0 = 0;
                int id1 = 1;
                int id2 = 2;
                m_floatMTest += 0.23f;
                vtWin.SendMessage(objectName, messageName, id0, id1, id2, m_floatMTest);


                //inside vt you only have to attach a MessageWaiter to the object and also 
                //a GetMessageDataBB after the waiter with the appropriate output parameters : 
                // int,int,int,int in this case here !
            }
        }

        private void sendMsgStr_Click(object sender, System.EventArgs e)
        {
            if (vtWin != null)
            {
                //we send a message to our player : 

                //when the player could'nt found the object by this name, then it sends
                //a broadcast message !
                String objectName = "masterDummy";

                //this message name should exist inside vt !
                String messageName = "csharpString";

                //the function takes 3 reserved parameters. you can do what ever you want  with it
                int id0 = 0;
                int id1 = 1;
                int id2 = 2;
                m_strMTest += "asdasd";
                vtWin.SendMessage(objectName, messageName, id0, id1, id2, m_strMTest);
                //inside vt you only have to attach a MessageWaiter to the object and also 
                //a GetMessageDataBB after the waiter with the appropriate output parameters : 
                // int,int,int,int in this case here !
            }
        }


        //check vtWin for new messages : 
        private void button1_Click(object sender, System.EventArgs e)
        {
            if (vtWin.HasMessages() == 1)
            {
                String console = richTextBox1.Text;

                for (int nbMsg = 0; nbMsg < vtWin.GetNumMessages(); nbMsg++)
                {
                    console += "\n message recieved : " + nbMsg + "  name : " + vtWin.GetMessageName(nbMsg);
                    for (int nbPar = 0; nbPar < vtWin.GetNumParameters(nbMsg); nbPar++)
                    {
                        int parType = vtWin.GetMessageParameterType(nbMsg, nbPar);
                        switch (parType)
                        {
                            //string : 
                            case 1:
                                {
                                    String value = vtWin.GetMessageValueStr(nbMsg, nbPar);
                                    console += "\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg, nbPar) + " value : " + value;
                                    break;
                                }
                            //float :
                            case 2:
                                {
                                    float value = vtWin.GetMessageValueFloat(nbMsg, nbPar);
                                    console += "\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg, nbPar) + " value : " + value;
                                    break;
                                }
                            //integer :
                            case 3:
                                {
                                    int value = vtWin.GetMessageValueInt(nbMsg, nbPar);
                                    console += "\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg, nbPar) + " value : " + value;
                                    break;
                                }
                            default:
                                break;
                        }
                    }
                }

                //vtWin.DeleteMessage(0);	//cleans incoming message by index

                vtWin.CleanMessages();	// pop everything !
                richTextBox1.Text = console;
            }

        }

        private void Resize_Click(object sender, System.EventArgs e)
        {

            if (vtWin != null)
            {

                vtWin.Pause();
                System.Threading.Thread.Sleep(50);
                this.SuspendLayout();

                if (startWidth == 400)
                {
                    startWidth = 640;
                    startHeight = 480;
                }
                else
                {

                    startWidth = 400;
                    startHeight = 300;
                }

                //just a test
                this.panel1.Size = new System.Drawing.Size(startWidth, startHeight);

                vtWin.UpdateRenderSize(startWidth, startHeight);
                System.Threading.Thread.Sleep(2);
                vtWin.Play();
                System.Threading.Thread.Sleep(2);
                this.PerformLayout();


            }
        }

        private void PauseBtn_Click(object sender, System.EventArgs e)
        {

            if (vtWin != null)
            {
                vtWin.Pause();
            }
        }

        private void PlayBtn_Click(object sender, System.EventArgs e)
        {

            if (vtWin != null)
            {
                vtWin.Play();
            }
        }

    }
}