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

ipcMain.on('getNewAllocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('newAllocationGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('acceptDelivery', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('acceptedDelivery', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('currentAllocData', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('currentAllocDataGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('changeDeliveryStatus', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('deliveryStatusChanged', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('getOldAlloc', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('oldAllocGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getDeliveryProfile', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('deliveryProfileGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('editDeliveryProfile', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('deliveryProfileEdited', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});