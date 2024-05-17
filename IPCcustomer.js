const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('getRestaurants', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getRestaurantResponse', { result });
    });
    
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});