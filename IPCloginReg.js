const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('login', (event, { tryfunc, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('loginResponse', { result });
        console.log(result);
    });
    console.log(`${tryfunc} ${dataset}`);

    backendProcess.stdin.write(`${tryfunc} ${dataset}\n`);
});

ipcMain.on('registersubmit', (event, { tryfunc , dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        console.log(result);
        global.mainWindow.webContents.send('registerResponse', { result });
        console.log(result);
    });
    console.log(`${tryfunc} ${dataset}`);

    backendProcess.stdin.write(`${tryfunc} ${dataset}\n`);
});

ipcMain.on("logout", (event) => {
    global.mainWindow.webContents.send('clearSession');
  })