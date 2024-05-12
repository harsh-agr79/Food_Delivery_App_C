const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('getMenu', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        // console.log(result);
        global.mainWindow.webContents.send('getMenuResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('addMenuItem', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        // console.log(result);
        global.mainWindow.webContents.send('getAddItemResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});