const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('setDeliveryManLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('deliveryManLocationSet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getDeliveryManLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('deliveryManLocationGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});