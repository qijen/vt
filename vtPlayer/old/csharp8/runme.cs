
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
		SWIGTYPE_p_long l  = new SWIGTYPE_p_long(win,true);
		vtWin.CreateAsChild(l);   
		
	}
	protected override void OnClosing(CancelEventArgs e)
	{
		base.OnClosing (e);
		if (vtWin!=null)
		{
			vtWin.Destroy();
		}
	}

	private System.Windows.Forms.Panel panel1;
	private System.Windows.Forms.Button sendmessage;

	//some temp test values for messaging : 
	private int m_intMTest=0;
	private float m_floatMTest=0;
	private String m_strMTest="";
	private System.Windows.Forms.Button sendMsgFloat;
	private System.Windows.Forms.Button sendMsgStr;
	

	public vtWindow vtWin  = null;
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

		//we retrieve the players render bounds : 
		int playerWidth = vtWin.GetWidth();
		int playerHeight = vtWin.GetHeight();

		// and we re-adjust our panel ! 
		this.panel1.Size = new System.Drawing.Size(playerWidth, playerHeight);
		this.panel1.Size=new System.Drawing.Size(playerWidth, playerHeight);
		vtWin.UpdateRenderSize(playerWidth, playerHeight);
		
		
		//and go !
		vtWin.Run();

		
		//vtWin.Show(1);
		
		this.PerformLayout();



	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	protected override void Dispose( bool disposing )
	{
		if( disposing )
		{
			if (components != null) 
			{
				components.Dispose();
			}
		}
		base.Dispose( disposing );
	}

		#region Windows Form Designer generated code
				/// <summary>
				/// Required method for Designer support - do not modify
				/// the contents of this method with the code editor.
				/// </summary>
		private void InitializeComponent()
		{
			this.panel1 = new System.Windows.Forms.Panel();
			this.sendmessage = new System.Windows.Forms.Button();
			this.sendMsgFloat = new System.Windows.Forms.Button();
			this.sendMsgStr = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Location = new System.Drawing.Point(120, 16);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(328, 232);
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
			// Form1
			// 
			//this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(888, 645);
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

	private void sendmessage_Click(object sender, System.EventArgs e)
	{
	
		if (vtWin!=null)
		{

			//we send a message to our player : 

			//when the player could'nt found the object by this name, then it sends
			//a broadcast message !


			String objectName = "masterDummy";

			//this message name should exist inside vt !
			String messageName  = "csharp";


			//the function takes 3 reserved parameters. you can do what ever you want  with it
			int id0 =0;
			int id1 = 1;
			int id2 = 2;
			m_intMTest++;
			vtWin.SendMessage(objectName,messageName,id0,id1,id2,m_intMTest);

			//just a test
			this.panel1.Size=new System.Drawing.Size(400, 300);
			vtWin.UpdateRenderSize(400,300);
			

			//inside vt you only have to attach a MessageWaiter to the object and also 
			//a GetMessageDataBB after the waiter with the appropriate output parameters : 
			// int,int,int,int in this case here !
		}
	}

	private void sendMsgFloat_Click(object sender, System.EventArgs e)
	{
	
		if (vtWin!=null)
		{
			//we send a message to our player : 

			//when the player could'nt found the object by this name, then it sends
			//a broadcast message !
			String objectName = "masterDummy";

			//this message name should exist inside vt !
			String messageName  = "csharpFloat";

			//the function takes 3 reserved parameters. you can do what ever you want  with it
			int id0 =0;
			int id1 = 1;
			int id2 = 2;
			m_floatMTest+=0.23f;
			vtWin.SendMessage(objectName,messageName,id0,id1,id2,m_floatMTest);
			
			
			//inside vt you only have to attach a MessageWaiter to the object and also 
			//a GetMessageDataBB after the waiter with the appropriate output parameters : 
			// int,int,int,int in this case here !
		}
	}

	private void sendMsgStr_Click(object sender, System.EventArgs e)
	{
		if (vtWin!=null)
		{
			//we send a message to our player : 

			//when the player could'nt found the object by this name, then it sends
			//a broadcast message !
			String objectName = "masterDummy";

			//this message name should exist inside vt !
			String messageName  = "csharpString";

			//the function takes 3 reserved parameters. you can do what ever you want  with it
			int id0 =0;
			int id1 = 1;
			int id2 = 2;
			m_strMTest+="asdasd";
			vtWin.SendMessage(objectName,messageName,id0,id1,id2,m_strMTest);
			//inside vt you only have to attach a MessageWaiter to the object and also 
			//a GetMessageDataBB after the waiter with the appropriate output parameters : 
			// int,int,int,int in this case here !
		}
	}

	}
}