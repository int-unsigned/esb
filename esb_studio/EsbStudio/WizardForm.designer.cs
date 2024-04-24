
namespace EsbStudio
{
    partial class WizardForm
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
            this.components = new System.ComponentModel.Container();
            this.lbEsbPath = new System.Windows.Forms.Label();
            this.tbEsbPath = new System.Windows.Forms.TextBox();
            this.bnEsbPathSelect = new System.Windows.Forms.Button();
            this.FolderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.bnSelPath1CChange = new System.Windows.Forms.Button();
            this.OneEsAddButton = new System.Windows.Forms.Button();
            this.swEsbPathSave = new System.Windows.Forms.CheckBox();
            this.lvOneEs = new System.Windows.Forms.ListView();
            this.colOneEs = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colOEsVersion = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colOEsArch = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colEsb = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.bnDone = new System.Windows.Forms.Button();
            this.bnCancel = new System.Windows.Forms.Button();
            this.lbEsbInfoList = new System.Windows.Forms.ListBox();
            this.lbEsbInfoText = new System.Windows.Forms.Label();
            this.InfoBox = new System.Windows.Forms.TextBox();
            this.bnDevTest = new System.Windows.Forms.Button();
            this.Sel1CPathBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.swShowUnsupported = new System.Windows.Forms.CheckBox();
            this.ChooseEsbMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.ChooseEsbMenuItemPrev = new System.Windows.Forms.ToolStripMenuItem();
            this.ChooseEsbMenuItemNext = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.ChooseEsbMenuItemCancel = new System.Windows.Forms.ToolStripMenuItem();
            this.ChooseEsbMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbEsbPath
            // 
            this.lbEsbPath.AutoSize = true;
            this.lbEsbPath.Location = new System.Drawing.Point(13, 15);
            this.lbEsbPath.Name = "lbEsbPath";
            this.lbEsbPath.Size = new System.Drawing.Size(133, 17);
            this.lbEsbPath.TabIndex = 0;
            this.lbEsbPath.Text = "Расположение Есб";
            // 
            // tbEsbPath
            // 
            this.tbEsbPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbEsbPath.BackColor = System.Drawing.SystemColors.Control;
            this.tbEsbPath.Location = new System.Drawing.Point(166, 13);
            this.tbEsbPath.Name = "tbEsbPath";
            this.tbEsbPath.ReadOnly = true;
            this.tbEsbPath.Size = new System.Drawing.Size(347, 22);
            this.tbEsbPath.TabIndex = 1;
            this.tbEsbPath.TextChanged += new System.EventHandler(this.tbEsbPath_TextChanged);
            // 
            // bnEsbPathSelect
            // 
            this.bnEsbPathSelect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bnEsbPathSelect.Location = new System.Drawing.Point(514, 13);
            this.bnEsbPathSelect.Name = "bnEsbPathSelect";
            this.bnEsbPathSelect.Size = new System.Drawing.Size(21, 22);
            this.bnEsbPathSelect.TabIndex = 2;
            this.bnEsbPathSelect.Text = "…";
            this.toolTip1.SetToolTip(this.bnEsbPathSelect, "Изменить расположение библиотеки Есб по умолчанию");
            this.bnEsbPathSelect.UseVisualStyleBackColor = true;
            this.bnEsbPathSelect.Click += new System.EventHandler(this.bnEsbPathSelect_Click);
            // 
            // FolderBrowserDialog
            // 
            this.FolderBrowserDialog.Description = "Укажите расположение библиотеки Есб";
            this.FolderBrowserDialog.RootFolder = System.Environment.SpecialFolder.MyComputer;
            this.FolderBrowserDialog.ShowNewFolderButton = false;
            // 
            // bnSelPath1CChange
            // 
            this.bnSelPath1CChange.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bnSelPath1CChange.Location = new System.Drawing.Point(514, 99);
            this.bnSelPath1CChange.Name = "bnSelPath1CChange";
            this.bnSelPath1CChange.Size = new System.Drawing.Size(21, 22);
            this.bnSelPath1CChange.TabIndex = 15;
            this.bnSelPath1CChange.Text = "…";
            this.toolTip1.SetToolTip(this.bnSelPath1CChange, "Изменить расположение инсталляций 1С по умолчанию");
            this.bnSelPath1CChange.UseVisualStyleBackColor = true;
            this.bnSelPath1CChange.Click += new System.EventHandler(this.bnSelPath1CChange_Click);
            // 
            // OneEsAddButton
            // 
            this.OneEsAddButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.OneEsAddButton.Location = new System.Drawing.Point(453, 125);
            this.OneEsAddButton.Name = "OneEsAddButton";
            this.OneEsAddButton.Size = new System.Drawing.Size(82, 26);
            this.OneEsAddButton.TabIndex = 17;
            this.OneEsAddButton.Text = "Добавить";
            this.toolTip1.SetToolTip(this.OneEsAddButton, "Добавить еще одну инсталляцию 1С в список");
            this.OneEsAddButton.UseVisualStyleBackColor = true;
            this.OneEsAddButton.Visible = false;
            // 
            // swEsbPathSave
            // 
            this.swEsbPathSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.swEsbPathSave.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.swEsbPathSave.Enabled = false;
            this.swEsbPathSave.Location = new System.Drawing.Point(13, 396);
            this.swEsbPathSave.Name = "swEsbPathSave";
            this.swEsbPathSave.Size = new System.Drawing.Size(268, 21);
            this.swEsbPathSave.TabIndex = 9;
            this.swEsbPathSave.Text = "Сохранить настройки в в реестре ?";
            this.toolTip1.SetToolTip(this.swEsbPathSave, "Настройки построителя сохраняются в ветке реестра \'HKEY_CURRENT_USER\\Software\\Int" +
        "Unsigned\\Esb\'");
            this.swEsbPathSave.UseCompatibleTextRendering = true;
            this.swEsbPathSave.UseVisualStyleBackColor = true;
            // 
            // lvOneEs
            // 
            this.lvOneEs.Alignment = System.Windows.Forms.ListViewAlignment.Default;
            this.lvOneEs.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvOneEs.CheckBoxes = true;
            this.lvOneEs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colOneEs,
            this.colOEsVersion,
            this.colOEsArch,
            this.colEsb});
            this.lvOneEs.FullRowSelect = true;
            this.lvOneEs.GridLines = true;
            this.lvOneEs.HideSelection = false;
            this.lvOneEs.Location = new System.Drawing.Point(16, 152);
            this.lvOneEs.MultiSelect = false;
            this.lvOneEs.Name = "lvOneEs";
            this.lvOneEs.Size = new System.Drawing.Size(519, 137);
            this.lvOneEs.TabIndex = 3;
            this.lvOneEs.UseCompatibleStateImageBehavior = false;
            this.lvOneEs.View = System.Windows.Forms.View.Details;
            this.lvOneEs.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lvOneEs_MouseDown);
            this.lvOneEs.Resize += new System.EventHandler(this.lvOneEs_Resize);
            // 
            // colOneEs
            // 
            this.colOneEs.Text = "Установленные 1С";
            this.colOneEs.Width = 312;
            // 
            // colOEsVersion
            // 
            this.colOEsVersion.Text = "Версия";
            this.colOEsVersion.Width = 81;
            // 
            // colOEsArch
            // 
            this.colOEsArch.Text = "Арх";
            this.colOEsArch.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.colOEsArch.Width = 43;
            // 
            // colEsb
            // 
            this.colEsb.Text = "Есб";
            this.colEsb.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // bnDone
            // 
            this.bnDone.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bnDone.Enabled = false;
            this.bnDone.Location = new System.Drawing.Point(460, 390);
            this.bnDone.Name = "bnDone";
            this.bnDone.Size = new System.Drawing.Size(75, 28);
            this.bnDone.TabIndex = 4;
            this.bnDone.Text = "Готово";
            this.bnDone.UseVisualStyleBackColor = true;
            this.bnDone.Click += new System.EventHandler(this.bnDone_Click);
            // 
            // bnCancel
            // 
            this.bnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bnCancel.Location = new System.Drawing.Point(379, 390);
            this.bnCancel.Name = "bnCancel";
            this.bnCancel.Size = new System.Drawing.Size(75, 28);
            this.bnCancel.TabIndex = 5;
            this.bnCancel.Text = "Отмена";
            this.bnCancel.UseVisualStyleBackColor = true;
            this.bnCancel.Click += new System.EventHandler(this.bnCancel_Click);
            // 
            // lbEsbInfoList
            // 
            this.lbEsbInfoList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbEsbInfoList.BackColor = System.Drawing.SystemColors.Control;
            this.lbEsbInfoList.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbEsbInfoList.FormattingEnabled = true;
            this.lbEsbInfoList.ItemHeight = 16;
            this.lbEsbInfoList.Items.AddRange(new object[] {
            "v8300Debug",
            "v8300Release"});
            this.lbEsbInfoList.Location = new System.Drawing.Point(16, 59);
            this.lbEsbInfoList.MultiColumn = true;
            this.lbEsbInfoList.Name = "lbEsbInfoList";
            this.lbEsbInfoList.Size = new System.Drawing.Size(519, 34);
            this.lbEsbInfoList.Sorted = true;
            this.lbEsbInfoList.TabIndex = 6;
            // 
            // lbEsbInfoText
            // 
            this.lbEsbInfoText.Location = new System.Drawing.Point(13, 38);
            this.lbEsbInfoText.Name = "lbEsbInfoText";
            this.lbEsbInfoText.Size = new System.Drawing.Size(490, 17);
            this.lbEsbInfoText.TabIndex = 7;
            this.lbEsbInfoText.Text = "В указанном месте Есб не обнаружена или ее структура повреждена.";
            // 
            // InfoBox
            // 
            this.InfoBox.AcceptsReturn = true;
            this.InfoBox.AcceptsTab = true;
            this.InfoBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.InfoBox.BackColor = System.Drawing.SystemColors.Control;
            this.InfoBox.Location = new System.Drawing.Point(16, 295);
            this.InfoBox.Multiline = true;
            this.InfoBox.Name = "InfoBox";
            this.InfoBox.ReadOnly = true;
            this.InfoBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.InfoBox.Size = new System.Drawing.Size(523, 76);
            this.InfoBox.TabIndex = 11;
            // 
            // bnDevTest
            // 
            this.bnDevTest.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bnDevTest.Location = new System.Drawing.Point(298, 393);
            this.bnDevTest.Name = "bnDevTest";
            this.bnDevTest.Size = new System.Drawing.Size(75, 23);
            this.bnDevTest.TabIndex = 12;
            this.bnDevTest.Text = "DevTest";
            this.bnDevTest.UseVisualStyleBackColor = true;
            this.bnDevTest.Visible = false;
            this.bnDevTest.Click += new System.EventHandler(this.button1_Click);
            // 
            // Sel1CPathBox
            // 
            this.Sel1CPathBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Sel1CPathBox.BackColor = System.Drawing.SystemColors.Control;
            this.Sel1CPathBox.Location = new System.Drawing.Point(166, 99);
            this.Sel1CPathBox.Name = "Sel1CPathBox";
            this.Sel1CPathBox.ReadOnly = true;
            this.Sel1CPathBox.Size = new System.Drawing.Size(347, 22);
            this.Sel1CPathBox.TabIndex = 14;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 101);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(126, 17);
            this.label1.TabIndex = 13;
            this.label1.Text = "Расположение 1C";
            // 
            // swShowUnsupported
            // 
            this.swShowUnsupported.AutoSize = true;
            this.swShowUnsupported.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.swShowUnsupported.Location = new System.Drawing.Point(10, 128);
            this.swShowUnsupported.Name = "swShowUnsupported";
            this.swShowUnsupported.Size = new System.Drawing.Size(333, 21);
            this.swShowUnsupported.TabIndex = 16;
            this.swShowUnsupported.Text = "Показывать неподдерживаемые платформы?";
            this.swShowUnsupported.UseVisualStyleBackColor = true;
            this.swShowUnsupported.CheckedChanged += new System.EventHandler(this.swShowUnsupported_CheckedChanged);
            // 
            // ChooseEsbMenu
            // 
            this.ChooseEsbMenu.AutoSize = false;
            this.ChooseEsbMenu.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.ChooseEsbMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1,
            this.toolStripSeparator1,
            this.ChooseEsbMenuItemPrev,
            this.ChooseEsbMenuItemNext,
            this.toolStripSeparator2,
            this.ChooseEsbMenuItemCancel});
            this.ChooseEsbMenu.Name = "ChooseEsbMenu";
            this.ChooseEsbMenu.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.ChooseEsbMenu.ShowImageMargin = false;
            this.ChooseEsbMenu.Size = new System.Drawing.Size(446, 132);
            this.ChooseEsbMenu.Closed += new System.Windows.Forms.ToolStripDropDownClosedEventHandler(this.ChooseEsbMenu_Closed);
            this.ChooseEsbMenu.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.ChooseEsbMenu_ItemClicked);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Font = new System.Drawing.Font("Tahoma", 7.2F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.toolStripMenuItem1.ForeColor = System.Drawing.Color.Red;
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(410, 24);
            this.toolStripMenuItem1.Text = "(построитель пытается проверить только ABI-совместимость)";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(442, 6);
            // 
            // ChooseEsbMenuItemPrev
            // 
            this.ChooseEsbMenuItemPrev.Enabled = false;
            this.ChooseEsbMenuItemPrev.Font = new System.Drawing.Font("Consolas", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ChooseEsbMenuItemPrev.Name = "ChooseEsbMenuItemPrev";
            this.ChooseEsbMenuItemPrev.Size = new System.Drawing.Size(410, 24);
            this.ChooseEsbMenuItemPrev.Text = "Предыдущая: <отсутствует>";
            // 
            // ChooseEsbMenuItemNext
            // 
            this.ChooseEsbMenuItemNext.Enabled = false;
            this.ChooseEsbMenuItemNext.Font = new System.Drawing.Font("Consolas", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ChooseEsbMenuItemNext.Name = "ChooseEsbMenuItemNext";
            this.ChooseEsbMenuItemNext.Size = new System.Drawing.Size(410, 24);
            this.ChooseEsbMenuItemNext.Text = "Следующая:  <отсутствует>";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(442, 6);
            // 
            // ChooseEsbMenuItemCancel
            // 
            this.ChooseEsbMenuItemCancel.Name = "ChooseEsbMenuItemCancel";
            this.ChooseEsbMenuItemCancel.Size = new System.Drawing.Size(410, 24);
            this.ChooseEsbMenuItemCancel.Text = "Отмена";
            // 
            // WizardForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bnCancel;
            this.ClientSize = new System.Drawing.Size(552, 424);
            this.Controls.Add(this.OneEsAddButton);
            this.Controls.Add(this.swShowUnsupported);
            this.Controls.Add(this.bnSelPath1CChange);
            this.Controls.Add(this.Sel1CPathBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bnDevTest);
            this.Controls.Add(this.InfoBox);
            this.Controls.Add(this.swEsbPathSave);
            this.Controls.Add(this.lbEsbInfoText);
            this.Controls.Add(this.lbEsbInfoList);
            this.Controls.Add(this.bnCancel);
            this.Controls.Add(this.bnDone);
            this.Controls.Add(this.lvOneEs);
            this.Controls.Add(this.bnEsbPathSelect);
            this.Controls.Add(this.tbEsbPath);
            this.Controls.Add(this.lbEsbPath);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "WizardForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Настройка проекта Есб";
            this.Load += new System.EventHandler(this.WizardForm_Load);
            this.ChooseEsbMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbEsbPath;
        private System.Windows.Forms.TextBox tbEsbPath;
        private System.Windows.Forms.Button bnEsbPathSelect;
        private System.Windows.Forms.FolderBrowserDialog FolderBrowserDialog;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ListView lvOneEs;
        private System.Windows.Forms.ColumnHeader colOneEs;
        private System.Windows.Forms.Button bnDone;
        private System.Windows.Forms.Button bnCancel;
        private System.Windows.Forms.ColumnHeader colOEsArch;
        private System.Windows.Forms.ListBox lbEsbInfoList;
        private System.Windows.Forms.Label lbEsbInfoText;
        private System.Windows.Forms.CheckBox swEsbPathSave;
        private System.Windows.Forms.TextBox InfoBox;
        private System.Windows.Forms.Button bnDevTest;
        private System.Windows.Forms.Button bnSelPath1CChange;
        private System.Windows.Forms.TextBox Sel1CPathBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox swShowUnsupported;
        private System.Windows.Forms.Button OneEsAddButton;
        private System.Windows.Forms.ColumnHeader colOEsVersion;
        private System.Windows.Forms.ColumnHeader colEsb;
        private System.Windows.Forms.ContextMenuStrip ChooseEsbMenu;
        private System.Windows.Forms.ToolStripMenuItem ChooseEsbMenuItemPrev;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem ChooseEsbMenuItemNext;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem ChooseEsbMenuItemCancel;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
    }
}

