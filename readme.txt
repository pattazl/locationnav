NewFileBrowser is a plugin for Notepad++, and it can define 2 new file's text and have a inner webbrowser which could run current file
v0.1.5
add new file type and fixed sometimes crash on x64

v0.1.4
support x64

v0.1.3
1. Can define and name 20 templates for new files.
2. Fix the bug that create new HTML file and browse directly will be error.

v0.1.2
1. Add icon on toolbar
2. Add function, auto show in browser when saved
3. Add could define the new file's encode by template file

v0.1.1
1. Fix the bug when open more than one defined newfile
2. Add docking dialog's icon
3. System shortcut such as ctrl+A/ctrl+C doesn't work in webbrowser, but can't fix it, because NPP can't support define PreTranslateMessage

V0.1.0
1. Use Ctrl+Shift+N or Ctrl+Shift+Alt+N to open a new file with default text which is defined yourself in template.
2. In template ^! means the default cursor position.
3. It has innner webbrowser with IE kernel, use Ctrl+E to browse current file in it

------------------------------------------------------------------------
LocationNavigate is a plugin for Notepad++ , and it is usefull for code/text edit and view, especially for many and large text files.
V0.4.8.1
fix the bug that marker will not work when keep undo

V0.4.8.0
improve the algorithm of marker ,make it work better 

V0.4.7.9
fix the bug that sometimes crash when save on x64

V0.4.7.8
support x64

v0.4.7.7
1. Add function, make it vertically centered when jump
2. Fix the bug, recorded too may entries when kept pressed arrow down/up
3. Fix the bug, filename in record will not be changed right now when save file.

v0.4.7.6
1. Fixed the bug that it will change the marker of file which before opened if open an exist file.

v0.4.7.5
1. Add AutoRecord, ManualRecord, ClearAllRecords functions in the menu, AutoRecord checked is default.

V0.4.7.4
1. Fix the bug, marker is not changed as expected in sometimes when file saved

V0.4.7.3
1. Improve method of making and showing marker, when other plugin auto-modify will skip(e.g. compare plugin)
2. Enhance marker performance when multi-line paste and modify.

V0.4.7.2
1. Fix the bug that it will crash when exit NPP in some PC
2. Fix the marker style will be changed when use compare plugin
3. Add docking dialog's icon

V0.4.7
1. Add some mark symbol, such as underline,rectangle,roundrect,circle,triangle,arrow.

V0.4.6
1. Add 'Save Color' for mark line when file saved
2. Fix mark line bug when searching in files.

V0.4.5.1
1. Try to add multi-line marked when paste multi-lines

V0.4.5
1. Add selfmark when mark edited line

V0.4.4.1_Alpha
1. Try to remove Markline when Undo action

V0.4.4(v0.4.3.2) Feature list
1.Add 'Mark' function can mark the modified line by color or bookmark
2.Add work thread to manage the data, so will be more quickly

V0.4.2 Feature list
1.Add 'In Curr' for goto position only current file
2.If 'In Curr', it will auto omit the same position at same file
V0.4 Feature list
1. Add 'Save record when App exit', Record data when application exit and it will be loaded in next run
2. Fixed the bug ( When NPP is start with none file and then open some file, the 'new 1' record will appear in record)

V0.3 Feature list 
#Automatic record the cursor position, you can set each position offset.
#You can jump to any position that your cursor have been visited.
#Can use shortcut( ctrl + - for back position and ctrl + shift+ - for forward position ) to jump code back and forward
#History position can automatic adjusted when text modify.
#Support jump between open files, if files closed ,it will open it automatic.
#Location history can be set to whether cleaned when file is closed
#Can jump to any modified location ( ctrl+atl + z ) back and forward ( ctrl+atl + y ) in the history list it is marked by '!'
#Will always record the position even after you jumped by setting 'Always record'.
#Toolbar's button will auto be diabled or enabled in different state 