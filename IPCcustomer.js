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

ipcMain.on('setCustomerLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('customerLocationSet', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getCustomerLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        // console.log(result);
        global.mainWindow.webContents.send('customerLocationGet', { result });
    });
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('setUserCart', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        // console.log(result)
        global.mainWindow.webContents.send('userCartSet', { result });
    });
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getMenuCustomer', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        console.log(result);
        global.mainWindow.webContents.send('getMenuCustomerResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});