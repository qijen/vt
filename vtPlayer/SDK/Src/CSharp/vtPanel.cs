using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace runme
{
    /// <summary>
    /// Summary description for vtPanel.
    /// </summary>
    public class vtPanel : System.Windows.Forms.Panel
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.Container components = null;
        public vtWindow vtWin = null;
        public WindowsApplication.Form1 parent;
        public void Init(WindowsApplication.Form1 _parent, vtWindow vtw)
        {
            parent = _parent;
            vtWin = vtw;
            //this.OnSizeChanged()
        }


        protected override void OnSizeChanged(EventArgs e)
        {
            System.Drawing.Size panelSize = this.Size;
            base.OnSizeChanged(e);

        }



        public vtPanel()
        {
            // This call is required by the Windows.Forms Form Designer.
            InitializeComponent();

            // TODO: Add any initialization after the InitComponent call
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (components != null)
                    components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
        }
        #endregion

        //protected override void OnPaintBackground(PaintEventArgs pevent) {
        /*protected override void OnPaintBackground(PaintEventArgs pevent)
        {
			
            if(vtWin.HasMessages()==1)
            {

                String console =  parent.richTextBox1.Text;
				
                for (int nbMsg = 0 ; nbMsg < vtWin.GetNumMessages() ; nbMsg ++)
                {
                    console +="\n message recieved : " + nbMsg + "  name : "  + vtWin.GetMessageName(nbMsg);
                    for (int nbPar = 0 ; nbPar < vtWin.GetNumParameters(nbMsg) ; nbPar ++ )
                    {
                        int parType = vtWin.GetMessageParameterType(nbMsg,nbPar);
                        switch (parType)
                        {
                                //string : 
                            case 1:
                            {
                                String value = vtWin.GetMessageValueStr(nbMsg,nbPar);
                                console +="\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg,nbPar) + " value : "  + value;
                                break;
                            }
                                //float :
                            case 2:
                            {
                                float value = vtWin.GetMessageValueFloat(nbMsg,nbPar);
                                console +="\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg,nbPar) + " value : "  + value;
                                break;
                            }
                                //integer :
                            case 3:
                            {
                                int value = vtWin.GetMessageValueInt(nbMsg,nbPar);
                                console +="\n\t parameter attached : " + nbPar + " : type : " + vtWin.GetMessageParameterType(nbMsg,nbPar) + " value : "  + value;
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                //vtWin.DeleteMessage(0);
                vtWin.CleanMessages();
                parent.richTextBox1.Text = console;
            }

            // Calling the base class OnPaint
            base.OnPaint(pevent);
        }*/
    }
}
