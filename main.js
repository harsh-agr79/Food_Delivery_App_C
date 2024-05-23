// main.js
const electron = require('electron')
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

global.mainWindow = null;

require('./IPCloginReg');
require('./IPCrestaurant');
require('./IPCcustomer');
require('./IPCdm');

function createWindow() {
    const display = electron.screen.getPrimaryDisplay()
    const maxiSize = display.workAreaSize
    mainWindow = new BrowserWindow({
        // resizable: false,
        height: maxiSize.height,
        width: maxiSize.width,
        show: false,
        webPreferences: {
            preload: path.join(__dirname, 'preload.js'),
            nodeIntegration: true
        }
    });

    mainWindow.loadFile('login.html');
}

ipcMain.on('register', (event) => {
    mainWindow.loadFile('register.html');
})
ipcMain.on('gotologin', (event) => {
    mainWindow.loadFile('login.html');
})
ipcMain.on('mainMenu', (event, { res }) => {
    mainWindow.loadFile( `${res[2]}.html`);
})

app.whenReady().then(()=>{
    createWindow();
    mainWindow.maximize();
    mainWindow.setResizable(false);
    mainWindow.show();
});


app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
        createWindow();
    }
});
