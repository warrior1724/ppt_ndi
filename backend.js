const electron = require('electron');
const ipc = electron.ipcMain;
const {
	app,
	BrowserWindow,
	Menu
} = require('electron');
const fs = require("fs");

let mainWindow;

app.on('window-all-closed', (e) => {
	if (process.platform != 'darwin')
		app.quit();
});

app.on('ready', function() {
	mainWindow = new BrowserWindow({
		width: 1200,
		height: 680,
		title: "",
		icon: __dirname + '/icon.png',
		resize: true,
		frame: false
	});

	mainWindow.loadURL('file://' + __dirname + '/index.html');
	mainWindow.focus();

	//For debugging:
	//mainWindow.webContents.openDevTools();

	mainWindow.on('close', function(e) {
		e.preventDefault();
		mainWindow.webContents.send('remote', {
			msg: 'exit'
		});
	});

	mainWindow.on('closed', function() {
		mainWindow = null;
	});

	ipc.on('remote', (event, data) => {
		if (data == "exit") {
			mainWindow.destroy();
		}
	});
});
