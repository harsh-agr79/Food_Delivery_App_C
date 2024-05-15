// main.js
const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

global.mainWindow = null;

require('./IPCloginReg');
require('./IPCrestaurant');

function createWindow() {
    mainWindow = new BrowserWindow({
        // width: 500,
        // height: 1000,
        fullscreen: true,  // Open the window in fullscreen mode
        fullscreenable: true,
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
