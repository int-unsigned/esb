
namespace EsbStudio
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
			this.bnMake = new System.Windows.Forms.Button();
			this.bnZip = new System.Windows.Forms.Button();
			this.bnMakeAndZip = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// bnMake
			// 
			this.bnMake.Location = new System.Drawing.Point(218, 28);
			this.bnMake.Name = "bnMake";
			this.bnMake.Size = new System.Drawing.Size(115, 61);
			this.bnMake.TabIndex = 0;
			this.bnMake.Text = "Make";
			this.bnMake.UseVisualStyleBackColor = true;
			this.bnMake.Click += new System.EventHandler(this.bnMake_Click);
			// 
			// bnZip
			// 
			this.bnZip.Location = new System.Drawing.Point(339, 28);
			this.bnZip.Name = "bnZip";
			this.bnZip.Size = new System.Drawing.Size(110, 61);
			this.bnZip.TabIndex = 1;
			this.bnZip.Text = "Zip";
			this.bnZip.UseVisualStyleBackColor = true;
			this.bnZip.Click += new System.EventHandler(this.bnZip_Click);
			// 
			// bnMakeAndZip
			// 
			this.bnMakeAndZip.Location = new System.Drawing.Point(218, 95);
			this.bnMakeAndZip.Name = "bnMakeAndZip";
			this.bnMakeAndZip.Size = new System.Drawing.Size(231, 61);
			this.bnMakeAndZip.TabIndex = 2;
			this.bnMakeAndZip.Text = "MakeAndZip";
			this.bnMakeAndZip.UseVisualStyleBackColor = true;
			this.bnMakeAndZip.Click += new System.EventHandler(this.bnMakeAndZip_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(737, 198);
			this.Controls.Add(this.bnMakeAndZip);
			this.Controls.Add(this.bnZip);
			this.Controls.Add(this.bnMake);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button bnMake;
        private System.Windows.Forms.Button bnZip;
		private System.Windows.Forms.Button bnMakeAndZip;
	}
}

