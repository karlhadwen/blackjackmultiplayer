Blackjack
=========

Local multiplayer blackjack built using Qt Creator 5.1.1 (C++); Runs by using a server and client. This project also has Doxygen documentation included for both client and server applications.

How to load, run and setup the build directory
=========

An important note: If you’re running Qt 4 you may find that the header files may need to remove “QWidgets/” before the widgets are declared, the reason for this is because Qt Creator 5 thought it was a good idea to modularize everything by putting all widgets in the “QWidgets” folder.

To load in, make sure you load the Qt project file in BlackjackProject>Blackjack> Blackjack> Blackjack.pro and for the server do: BlackjackProject>BlackjackServer> BlackjackServer > BlackjackServer.pro

To run this program through QtCreator you must first go to the ‘Projects’ tab and then go into ‘General’. Once you’re on the general page you must change the ‘Build Directory’ to where the project is located. If you’ve extracted the files to your desktop then point the Build directory to folder on the desktop. You may get prompted to do this as soon you launch the program but if you do not, make sure you change the directory so that the objects link up correctly.

How to run and use the program
=========

This program was created on Windows using Qt Creator 5.1.1

It’s imperative that whilst reading these instructions that you keep in mind that you must start the server first, not the client. You can start the client first but it won’t connect to any server.

Once you’ve set the correct build directory whether that be through the build settings or when you started the program, you must then go into ‘Edit’, right click on the project you want to run, and make sure it’s set as the active project, once it’s set as the active project, right click again and click on ‘Run’—alternatively if you already have the project set as your active project you can click on the green play button on the bottom left of the screen. You will have to do this for both Blackjack & the server (IMPORTANT note: You must start the server first.)
