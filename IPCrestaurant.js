const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('getMenu', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getMenuResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('addMenuItem', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('getAddItemResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('editMenuItem', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('getEditItemResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('deleteMenuItem', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('getDeleteItemResponse', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('setRestaurantLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('restaurantLocationSet', { result });
    });

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getRestaurantLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('restaurantLocationGet', { result });
    });
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});